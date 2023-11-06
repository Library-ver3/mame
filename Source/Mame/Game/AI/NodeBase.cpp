#include "NodeBase.h"
#include "JudgmentBase.h"
#include "BehaviorData.h"
#include "ActionBase.h"
#include "EnemyBlueSlime.h"

// �f�X�g���N�^
template <class T>
NodeBase<T>::~NodeBase()
{
    delete judgment_;
    delete action_;
}

// �q�m�[�h Getter
template <class T>
NodeBase<T>* NodeBase<T>::GetChild(const int index) const
{
    if (children_.size() > index)
    {
        return nullptr;
    }
    return children_.at(index);
}

// �q�m�[�h Getter ( ���� )
template <class T>
NodeBase<T>* NodeBase<T>::GetLastChild() const
{
    if (children_.size() == 0)
    {
        return nullptr;
    }

    return children_.at(children_.size() - 1);
}

// �q�m�[�h Getter ( �擪 )
template <class T>
NodeBase<T>* NodeBase<T>::GetTopChild() const
{
    if (children_.size() == 0)
    {
        return nullptr;
    }

    return children_.at(0);
}

// ���f
template <class T>
const bool NodeBase<T>::Judgment() const
{
    // judgment�����邩���f�B����΃����o�֐�Judgment()���s�������ʂ�Ԃ�
    if (judgment_ != nullptr)
    {
        return judgment_->Judgment();
    }

    return true;
}

// �D�揇�ʑI��
template <class T>
NodeBase<T>* NodeBase<T>::SelectPriority(std::vector<NodeBase*>* list)
{
    NodeBase* selectNode = nullptr;
    int priority = INT_MAX;

    // ��ԗD�揇�ʂ�����(�l��������)�m�[�h��T����selectNode�Ɋi�[
    for (size_t i = 0; i < (*list).size(); ++i)
    {
        NodeBase* node = (*list).at(i);
        if (node->priority_ < priority)
        {
            priority = node->priority_;
            selectNode = node;
        }
    }

    return selectNode;
}

// �����_��
template <class T>
NodeBase<T>* NodeBase<T>::SelectRandom(std::vector<NodeBase*>* list)
{
    // list�̃T�C�Y�ŗ������擾����selectNo�Ɋi�[
    const size_t lastNode = (*list).size() - 1;
    const size_t selectNo = static_cast<size_t>(std::rand()) % lastNode;

    // list��selectNo�Ԗڂ̎��̂�Ԃ�
    return (*list).at(selectNo);
}

// �V�[�P���X�E�V�[�P���V�������[�s���O�Ńm�[�h�I��
template <class T>
NodeBase<T>* NodeBase<T>::SelectSequence(std::vector<NodeBase*>* list, BehaviorData<T>* data)
{
    int step = 0;

    // �w�肳��Ă��钆�ԃm�[�h���ǂ̃V�[�P���X���ǂ��܂Ŏ��s���ꂽ���擾����
    step = data->GetSequenceStep(name_);

    // ���ԃm�[�h�ɓo�^����Ă���m�[�h���ȏ�̏ꍇ
    if (step >= children_.size())
    {
        // ���[����SequenceLooping�̎��͍ŏ�������s���邽�߁Astep��0����
        if (children_.at(step)->selectRule_ == BehaviorTree::SelectRule::SequentialLooping)
        {
            step = 0;
        }
        // ���[����Sequence�̎��͎��Ɏ��s�ł���m�[�h���Ȃ����߁Anullptr��Ԃ�
        if (children_.at(step)->selectRule_ == BehaviorTree::SelectRule::Sequence)
        {
            return nullptr;
        }
    }

    // ���s�\���X�g�ɓo�^����Ă���f�[�^�̐��������[�v���s��
    for (std::vector<NodeBase*>::iterator it = list->begin(); it != list->end(); ++it)
    {
        // �q�m�[�h�����s�\���X�g�Ɋ܂܂�Ă��邩
        NodeBase* childNode = children_.at(step);

        if (childNode->GetName() == (*it)->GetName())
        {
            // ���݂̎��s�m�[�h�̕ۑ�
            data->PushSequenceNode(this);

            // ���Ɏ��s���钆�ԃm�[�h�ƃX�e�b�v����ۑ�����
            data->SetSequenceStep(childNode->GetName(), step + 1);

            // ���݂̃X�e�b�v�ԍ��̃m�[�h��Ԃ�
            return childNode;
        }
    }

    // �w�肳�ꂽ���ԃm�[�h�Ɏ��s�\�m�[�h���Ȃ��̂�nullptr��Ԃ�
    return nullptr;
}

// �m�[�h����
template <class T>
NodeBase<T>* NodeBase<T>::SearchNode(const std::string& searchName)
{
    if (name_ == searchName)
    {   // ���O����v
        return this;
    }
    else
    {   // �q�m�[�h�Ō���
        for (std::vector<NodeBase*>::iterator it = children_.begin(); it != children_.end(); ++it)
        {
            NodeBase* ret = (*it)->SearchNode(searchName);

            if (ret != nullptr)
            {
                return ret;
            }
        }
    }

    return nullptr;
}

// �m�[�h���_
template <class T>
NodeBase<T>* NodeBase<T>::Inference(BehaviorData<T>* data)
{
    std::vector<NodeBase*> list;
    NodeBase* result = nullptr;

    // children�̐��������[�v���s��
    for (int i = 0; i < children_.size(); ++i)
    {
        if (children_.at(i)->judgment_ != nullptr)
        {
            if (children_.at(i)->Judgment())
            {
                list.emplace_back(children_.at(i));
            }
        }
        else
        {
            list.emplace_back(children_.at(i));
        }
    }

    // �I�����[���Ńm�[�h����
    switch (selectRule_)
    {
        // �D�揇��
    case BehaviorTree::SelectRule::Priority:
        result = SelectPriority(&list);
        break;

        //�����_��
    case BehaviorTree::SelectRule::Random:
        result = SelectRandom(&list);
        break;

        // �V�[�N�G���X
    case BehaviorTree::SelectRule::Sequence:
    case BehaviorTree::SelectRule::SequentialLooping:
        result = SelectSequence(&list, data);
        break;
    }

    if (result != nullptr)
    {
        if (result->HasAction())
        {   // �s��������ΏI��
            return result;
        }
        else
        {   // ���܂����m�[�h�Ő��_�J�n
            result = result->Inference(data);
        }
    }
}

