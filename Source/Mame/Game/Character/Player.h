#pragma once
#include "Character.h"

class Player : public Character
{
public:// �萔
#pragma region Constants
    enum class Animation
    {
        Idle,           // �ҋ@
        Dash,           // ����
        Attack_1,       // �U���P
        Attack_2,       // �U���Q
        Attack_3,       // �U���R
        Avoid,          // ���
        Counter,        // �J�E���^�[
        CounterAttack,  // �J�E���^�[�U��
        SoftStagger,    // ��_��
        HardStagger,    // ������сA���S
        StandUp,        // �N���オ��
        HardAttack,     // ���U��
        EnemyWalk,      // ������ver
    };
#pragma endregion // Constants
public:// ��{�I�Ȋ֐�
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

