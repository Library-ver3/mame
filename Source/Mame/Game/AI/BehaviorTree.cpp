#include "BehaviorTree.h"
#include "ActionBase.h"
#include "NodeBase.h"
#include "JudgmentBase.h"
#include "EnemyBlueSlime.h"
#include "BehaviorData.h"

// �f�X�g���N�^
template <class T>
BehaviorTree<T>::~BehaviorTree()
{
    NodeAllClear(root_);
}

// ���_
template <class T>
NodeBase<T>* BehaviorTree<T>::ActiveNodeInference(BehaviorData<T>* data)
{
    // �f�[�^�����Z�b�g���ĊJ�n
    data->Initialize();
    return root_->Inference(data);
}

// �V�[�P���X�m�[�h���琄�_�J�n
template <class T>
NodeBase<T>* BehaviorTree<T>::SequenceBack(NodeBase<T>* sequenceNode, BehaviorData<T>* data)
{
    return sequenceNode->Inference(data);
}

// �m�[�h�ǉ�
template <class T>
void BehaviorTree<T>::AddNode(std::string parentName, std::string entryName, int priority, SelectRule selectRule, JudgmentBase<T>* judgment, ActionBase<T>* action)
{
    if (parentName != "")
    {
        NodeBase* parentNode = root_->SearchNode(parentName);

        if (parentNode != nullptr)
        {
            NodeBase* sibling = parentNode->GetLastChild();
            NodeBase* addNode = new NodeBase(entryName, parentNode, sibling, priority, selectRule, judgment, action, parentNode->GetHirerchyNo() + 1);

            parentNode->AddChild(addNode);
        }
    }
    else
    {
        if (root_ == nullptr)
        {
            root_ = new NodeBase(entryName, nullptr, nullptr, priority, selectRule, judgment, action, 1);
        }
    }
}

// �m�[�h���s
template <class T>
NodeBase<T>* BehaviorTree<T>::Run(NodeBase<T>* actionNode, BehaviorData<T>* data, float elapsedTime)
{
    // �m�[�h���s
    ActionBase::State state = actionNode->Run(elapsedTime);
        
    if (state == ActionBase::State::Complete)
    {   // ����I��

        // �V�[�P���X�̓r�����𔻒f
        NodeBase* sequenceNode = data->PopSequenceNode();

        if (sequenceNode == nullptr)
        {   // �r������Ȃ��Ȃ�I��
            return nullptr;
        }
        else
        {   // �r���Ȃ炻������n�߂�
            return SequenceBack(sequenceNode, data);
        }
    }
    else if (state == ActionBase::State::Failed)
    {   // ���s�͏I��
        return nullptr;
    }

    // ����ێ�
    return actionNode;
}

// �o�^���ꂽ�m�[�h��S�č폜����
template <class T>
void BehaviorTree<T>::NodeAllClear(NodeBase<T>* delNode)
{
    size_t count = delNode->children_.size();
    if (count > 0)
    {
        for (NodeBase* node : delNode->children_)
        {
            NodeAllClear(node);
        }
        delete delNode;
    }
    else
    {
        delete delNode;
    }
}
