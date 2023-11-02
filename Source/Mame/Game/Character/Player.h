#pragma once
#include "Character.h"

class Player : public Character
{
public:// 定数
#pragma region Constants
    enum class Animation
    {
        Idle,           // 待機
        Dash,           // 走り
        Attack_1,       // 攻撃１
        Attack_2,       // 攻撃２
        Attack_3,       // 攻撃３
        Avoid,          // 回避
        Counter,        // カウンター
        CounterAttack,  // カウンター攻撃
        SoftStagger,    // 被ダメ
        HardStagger,    // 吹っ飛び、死亡
        StandUp,        // 起き上がり
        HardAttack,     // 強攻撃
        EnemyWalk,      // 歩き別ver
    };
#pragma endregion // Constants
public:// 基本的な関数
    Player();
    ~Player() override;

    void Initialize()                           override;
    void Finalize()                             override;
    void Update(const float& elapsedTime)       override;
    void Render(const float& scale, 
        ID3D11PixelShader* psShader = nullptr)  override;
    void DrawDebug()                            override;

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> psShader_ = nullptr;
};

