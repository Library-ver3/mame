#include "BehaviorData.h"
#include "NodeBase.h"

// 初期化
template <class T>
void BehaviorData<T>::Initialize()
{
    runSequenceStepMap_.clear();
    while (sequenceStack_.size() > 0)
    {
        sequenceStack_.pop();
    }
}

// シーケンスノードのポップ
template <class T>
NodeBase<T>* BehaviorData<T>::PopSequenceNode()
{
    // 空ならNULL
    if (sequenceStack_.empty() != 0)
    {
        return nullptr;
    }

    NodeBase* node = sequenceStack_.top();
    if (node != nullptr)
    {
        // 取り出したデータを削除
        sequenceStack_.pop();
    }
    return node;
}

// シーケンスステップ Getter
template <class T>
int BehaviorData<T>::GetSequenceStep(std::string name)
{
    if (runSequenceStepMap_.count(name) == 0)
    {
        runSequenceStepMap_.insert(std::make_pair(name, 0));
    }

    return runSequenceStepMap_.at(name);
}

// シーケンスステップ Setter
template <class T>
void BehaviorData<T>::SetSequenceStep(std::string name, int step)
{
    runSequenceStepMap_.at(name) = step;
}
