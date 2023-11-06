#include "BehaviorTree.h"
#include "ActionBase.h"
#include "NodeBase.h"
#include "JudgmentBase.h"
#include "EnemyBlueSlime.h"
#include "BehaviorData.h"

// デストラクタ
template <class T>
BehaviorTree<T>::~BehaviorTree()
{
    NodeAllClear(root_);
}

// 推論
template <class T>
NodeBase<T>* BehaviorTree<T>::ActiveNodeInference(BehaviorData<T>* data)
{
    // データをリセットして開始
    data->Initialize();
    return root_->Inference(data);
}

// シーケンスノードから推論開始
template <class T>
NodeBase<T>* BehaviorTree<T>::SequenceBack(NodeBase<T>* sequenceNode, BehaviorData<T>* data)
{
    return sequenceNode->Inference(data);
}

// ノード追加
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

// ノード実行
template <class T>
NodeBase<T>* BehaviorTree<T>::Run(NodeBase<T>* actionNode, BehaviorData<T>* data, float elapsedTime)
{
    // ノード実行
    ActionBase::State state = actionNode->Run(elapsedTime);
        
    if (state == ActionBase::State::Complete)
    {   // 正常終了

        // シーケンスの途中かを判断
        NodeBase* sequenceNode = data->PopSequenceNode();

        if (sequenceNode == nullptr)
        {   // 途中じゃないなら終了
            return nullptr;
        }
        else
        {   // 途中ならそこから始める
            return SequenceBack(sequenceNode, data);
        }
    }
    else if (state == ActionBase::State::Failed)
    {   // 失敗は終了
        return nullptr;
    }

    // 現状維持
    return actionNode;
}

// 登録されたノードを全て削除する
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
