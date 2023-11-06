#include "NodeBase.h"
#include "JudgmentBase.h"
#include "BehaviorData.h"
#include "ActionBase.h"
#include "EnemyBlueSlime.h"

// デストラクタ
template <class T>
NodeBase<T>::~NodeBase()
{
    delete judgment_;
    delete action_;
}

// 子ノード Getter
template <class T>
NodeBase<T>* NodeBase<T>::GetChild(const int index) const
{
    if (children_.size() > index)
    {
        return nullptr;
    }
    return children_.at(index);
}

// 子ノード Getter ( 末尾 )
template <class T>
NodeBase<T>* NodeBase<T>::GetLastChild() const
{
    if (children_.size() == 0)
    {
        return nullptr;
    }

    return children_.at(children_.size() - 1);
}

// 子ノード Getter ( 先頭 )
template <class T>
NodeBase<T>* NodeBase<T>::GetTopChild() const
{
    if (children_.size() == 0)
    {
        return nullptr;
    }

    return children_.at(0);
}

// 判断
template <class T>
const bool NodeBase<T>::Judgment() const
{
    // judgmentがあるか判断。あればメンバ関数Judgment()実行した結果を返す
    if (judgment_ != nullptr)
    {
        return judgment_->Judgment();
    }

    return true;
}

// 優先順位選択
template <class T>
NodeBase<T>* NodeBase<T>::SelectPriority(std::vector<NodeBase*>* list)
{
    NodeBase* selectNode = nullptr;
    int priority = INT_MAX;

    // 一番優先順位が高い(値が小さい)ノードを探してselectNodeに格納
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

// ランダム
template <class T>
NodeBase<T>* NodeBase<T>::SelectRandom(std::vector<NodeBase*>* list)
{
    // listのサイズで乱数を取得してselectNoに格納
    const size_t lastNode = (*list).size() - 1;
    const size_t selectNo = static_cast<size_t>(std::rand()) % lastNode;

    // listのselectNo番目の実体を返す
    return (*list).at(selectNo);
}

// シーケンス・シーケンシャルルーピングでノード選択
template <class T>
NodeBase<T>* NodeBase<T>::SelectSequence(std::vector<NodeBase*>* list, BehaviorData<T>* data)
{
    int step = 0;

    // 指定されている中間ノードがどのシーケンスがどこまで実行されたか取得する
    step = data->GetSequenceStep(name_);

    // 中間ノードに登録されているノード数以上の場合
    if (step >= children_.size())
    {
        // ルールがSequenceLoopingの時は最初から実行するため、stepに0を代入
        if (children_.at(step)->selectRule_ == BehaviorTree::SelectRule::SequentialLooping)
        {
            step = 0;
        }
        // ルールがSequenceの時は次に実行できるノードがないため、nullptrを返す
        if (children_.at(step)->selectRule_ == BehaviorTree::SelectRule::Sequence)
        {
            return nullptr;
        }
    }

    // 実行可能リストに登録されているデータの数だけループを行う
    for (std::vector<NodeBase*>::iterator it = list->begin(); it != list->end(); ++it)
    {
        // 子ノードが実行可能リストに含まれているか
        NodeBase* childNode = children_.at(step);

        if (childNode->GetName() == (*it)->GetName())
        {
            // 現在の実行ノードの保存
            data->PushSequenceNode(this);

            // 次に実行する中間ノードとステップ数を保存する
            data->SetSequenceStep(childNode->GetName(), step + 1);

            // 現在のステップ番号のノードを返す
            return childNode;
        }
    }

    // 指定された中間ノードに実行可能ノードがないのでnullptrを返す
    return nullptr;
}

// ノード検索
template <class T>
NodeBase<T>* NodeBase<T>::SearchNode(const std::string& searchName)
{
    if (name_ == searchName)
    {   // 名前が一致
        return this;
    }
    else
    {   // 子ノードで検索
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

// ノード推論
template <class T>
NodeBase<T>* NodeBase<T>::Inference(BehaviorData<T>* data)
{
    std::vector<NodeBase*> list;
    NodeBase* result = nullptr;

    // childrenの数だけループを行う
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

    // 選択ルールでノード決め
    switch (selectRule_)
    {
        // 優先順位
    case BehaviorTree::SelectRule::Priority:
        result = SelectPriority(&list);
        break;

        //ランダム
    case BehaviorTree::SelectRule::Random:
        result = SelectRandom(&list);
        break;

        // シークエンス
    case BehaviorTree::SelectRule::Sequence:
    case BehaviorTree::SelectRule::SequentialLooping:
        result = SelectSequence(&list, data);
        break;
    }

    if (result != nullptr)
    {
        if (result->HasAction())
        {   // 行動があれば終了
            return result;
        }
        else
        {   // 決まったノードで推論開始
            result = result->Inference(data);
        }
    }
}

