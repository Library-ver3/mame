#pragma once

#include <DirectXMath.h>
#include <stdlib.h>

// ポインタ消去関数
template <class T>
inline void SafeDeletePtr(T*& p)
{
    if (p == nullptr) return;

    delete (p);
    p = nullptr;
}