#pragma once

// �s���������N���X
template <class T>
class ActionBase
{
public:// �萔
    enum class State
    {
        Run,        // ���s��
        Failed,     // ���s���s
        Complete,   // ���s����
    };

public:
    ActionBase(T* enemy) : owner_(enemy) {}

    // ���s���� ( �������z�֐� )
    virtual ActionBase::State Run(float elapsedTime) = 0;

protected:
    T* owner_;
    int step_ = 0;
};