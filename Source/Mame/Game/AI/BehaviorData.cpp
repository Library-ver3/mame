#include "BehaviorData.h"
#include "NodeBase.h"

// ������
template <class T>
void BehaviorData<T>::Initialize()
{
    runSequenceStepMap_.clear();
    while (sequenceStack_.size() > 0)
    {
        sequenceStack_.pop();
    }
}

// �V�[�P���X�m�[�h�̃|�b�v
template <class T>
NodeBase<T>* BehaviorData<T>::PopSequenceNode()
{
    // ��Ȃ�NULL
    if (sequenceStack_.empty() != 0)
    {
        return nullptr;
    }

    NodeBase* node = sequenceStack_.top();
    if (node != nullptr)
    {
        // ���o�����f�[�^���폜
        sequenceStack_.pop();
    }
    return node;
}

// �V�[�P���X�X�e�b�v Getter
template <class T>
int BehaviorData<T>::GetSequenceStep(std::string name)
{
    if (runSequenceStepMap_.count(name) == 0)
    {
        runSequenceStepMap_.insert(std::make_pair(name, 0));
    }

    return runSequenceStepMap_.at(name);
}

// �V�[�P���X�X�e�b�v Setter
template <class T>
void BehaviorData<T>::SetSequenceStep(std::string name, int step)
{
    runSequenceStepMap_.at(name) = step;
}
