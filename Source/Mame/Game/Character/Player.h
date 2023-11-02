#pragma once
#include "Character.h"

class Player : public Character
{
public:// ่
#pragma region Constants
    enum class Animation
    {
        Idle,           // า@
        Dash,           // ่
        Attack_1,       // UP
        Attack_2,       // UQ
        Attack_3,       // UR
        Avoid,          // ๑๐
        Counter,        // JE^[
        CounterAttack,  // JE^[U
        SoftStagger,    // ํ_
        HardStagger,    // ม๒ัAS
        StandUp,        // Nซใช่
        HardAttack,     // ญU
        EnemyWalk,      // เซสver
    };
#pragma endregion // Constants
public:// ๎{Iศึ
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

