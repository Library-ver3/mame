#pragma once

#include <vector>
#include <stack>
#include <map>
#include "BehaviorTree.h"

template <class T>
class NodeBase;

// Behavior保存データ
template <class T>
class BehaviorData
{
public:
    BehaviorData() { Initialize(); }

    // 初期化
    void Initialize();

    // シーケンスノードのプッシュ
    void PushSequenceNode(NodeBase<T>* node) { sequenceStack_.push(node); }

    // シーケンスノードのポップ
    NodeBase<T>* PopSequenceNode();

public:// 取得・設定
    int GetSequenceStep(std::string name);
    void SetSequenceStep(std::string name, int step);

private:
    std::stack<NodeBase<T>*> sequenceStack_;            // 実行する中間ノードをスタック
    std::map<std::string, int> runSequenceStepMap_;  // 実行中の中間ノードのステップを記録
};