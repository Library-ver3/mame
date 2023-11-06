#pragma once

// 行動処理基底クラス
template <class T>
class ActionBase
{
public:// 定数
    enum class State
    {
        Run,        // 実行中
        Failed,     // 実行失敗
        Complete,   // 実行成功
    };

public:
    ActionBase(T* enemy) : owner_(enemy) {}

    // 実行処理 ( 純粋仮想関数 )
    virtual ActionBase::State Run(float elapsedTime) = 0;

protected:
    T* owner_;
    int step_ = 0;
};