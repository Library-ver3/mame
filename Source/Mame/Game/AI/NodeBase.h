#pragma once

#include <vector>
#include <string>
#include "BehaviorTree.h"
#include "ActionBase.h"

template <class T>
class JudgmentBase;
template <class T>
class BehaviorData;

// メモリリーク調査用
#define debug_new new(_NORMAL_BLOCK, __FILE__, __LINE__)

// ノード
template <class T>
class NodeBase
{
public:
    NodeBase(
        const std::string& name,
        NodeBase<T>* parent,
        NodeBase<T>* sibling,
        const unsigned int& priority,
        const BehaviorTree::SelectRule& selectRule,
        JudgmentBase<T>* judgment,
        ActionBase<T>* action,
        const int& hierarchyNo
    )
        :name_(name), parent_(parent), sibling_(sibling), priority_(priority),
        selectRule_(selectRule), judgment_(judgment), action_(action),
        hierarchyNo_(hierarchyNo), children_(NULL)
    {
    }
    ~NodeBase();

public:// 取得・設定
#pragma region [Set,Get..etc]Function
    // ----- 名前 -----
    const std::string GetName() const { return name_; }

    // ----- 親ノード -----
    NodeBase<T>* GetParent() const { return parent_; }
    void SetParent(NodeBase<T>* parent) { parent_ = parent; }

    // ----- 子ノード -----
    NodeBase<T>* GetChild(const int index) const;
    NodeBase<T>* GetLastChild() const;   // ( 末尾 )
    NodeBase<T>* GetTopChild() const;    // ( 先頭 )

    void AddChild(NodeBase<T>* child) { children_.push_back(child); }

    // ----- 兄弟ノード -----
    NodeBase<T>* GetSibling() const { return sibling_; }
    void SetSibling(NodeBase<T>* sibling) { sibling_ = sibling; }

    // 階層番号
    const int GetHirerchyNo() const { return hierarchyNo_; } 
    
    // 優先順位
    const int GetPriority() const { return priority_; }

    // 行動データを持っているか
    const bool HasAction() const { return action_ != nullptr ? true : false; }
#pragma endregion// [Set,Get..etc]Function

    // 実行可否判定
    const bool Judgment() const;

    // 優先順位選択
    NodeBase<T>* SelectPriority(std::vector<NodeBase<T>*>* list);

    // ランダム選択
    NodeBase<T>* SelectRandom(std::vector<NodeBase<T>*>* list);

    // シーケンス選択
    NodeBase<T>* SelectSequence(std::vector<NodeBase<T>*>* list, BehaviorData<T>* data);

    // ノード検索
    NodeBase<T>* SearchNode(const std::string& searchName);

    // ノード推論
    NodeBase<T>* Inference(BehaviorData<T>* data);

    // 実行
    ActionBase::State Run(const float& elapsedTime)
    {
        // actionがあるか判断。あればメンバ関数Run()実行した結果を返す
        if (action_ != nullptr)
        {
            return action_->Run(elapsedTime);
        }

        return ActionBase::State::Failed;
    }

public:
    std::vector<NodeBase*> children_;

protected:
    std::string                 name_;          // 名前
    BehaviorTree::SelectRule selectRule_;    // 選択ルール
    JudgmentBase<T>*            judgment_;      // 判定クラス
    ActionBase<T>*              action_;        // 実行クラス
    unsigned int                priority_;      // 優先順位
    NodeBase<T>*                parent_;        // 親ノード
    NodeBase<T>*                sibling_;       // 兄弟ノード
    int                         hierarchyNo_;   // 階層番号
};