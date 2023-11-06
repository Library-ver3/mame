#pragma once
#include <string>

template <class T>
class ActionBase;
template <class T>
class JudgmentBase;
template <class T>
class NodeBase;
template <class T>
class BehaviorData;

template <class T>
class BehaviorTree
{
public:// 定数
    // 選択ルール
    enum class SelectRule
    {
        Non,
        Priority,           // 優先順位
        Sequence,           // シークエンス
        SequentialLooping,  // シークエンシャルルーピング
        Random,             // ランダム
    };

public:
    BehaviorTree() : root_(nullptr), owner_(nullptr) {}
    BehaviorTree(T* enemy) : root_(nullptr), owner_(enemy) {}
    ~BehaviorTree();

    // 実行ノードを推論する
    NodeBase<T>* ActiveNodeInference(BehaviorData<T>* data);

    // シークエンスノード空推論開始
    NodeBase<T>* SequenceBack(NodeBase<T>* sequenceNode, BehaviorData<T>* data);

    // ノード追加
    void AddNode(
        std::string parentName,     // 親ノード名を指定。自分がどのノードに紐づくか、また兄弟ノードの探索に使用
        std::string entryName,      // 自分のノード名。実行ノードの探索などに使用
        int priority,               // 優先順位。ルールが Priority などの場合に使用
        SelectRule selectRule,      // 各中間ノードのルールを指定
        JudgmentBase<T>* judgment,  // ノードが実行できるか判断するオブジェクト
        ActionBase<T>* action       // 実際に行動処理のオブジェクト
    );

    // 実行
    NodeBase<T>* Run(NodeBase<T>* actionNode, BehaviorData<T>* data, float elapsedTime);

private:
    // ノード全削除
    void NodeAllClear(NodeBase<T>* delNode);

private:
    // ルートノード
    NodeBase<T>* root_;
    T* owner_;
};

