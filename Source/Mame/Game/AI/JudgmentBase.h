#pragma once

// ���s����
template <class T>
class JudgmentBase
{
public:
    JudgmentBase(T* enemy) : owner(enemy) {}
    virtual bool Judgment() = 0;

protected:
    T* owner;
};