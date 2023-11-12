#pragma once
#include "Character.h"

#include "../AI/StateMachine.h"
#include "../../Input/Input.h"

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

    enum class STATE
    {
        Idle,
        Walk,
        Jump,
    };
#pragma endregion // Constants
public:// ��{�I�Ȋ֐� ( override ���ꂽ�֐� )
    Player();
    ~Player() override;

    void Initialize()                           override;
    void Finalize()                             override;
    void Update(const float& elapsedTime)       override;
    void Render(const float& scale, 
        ID3D11PixelShader* psShader = nullptr)  override;
    void DrawDebug()                            override;

public:
    // ----- ���͒l���J�������猩���x�N�g���ɕϊ����� -----
    void ConvertToCameraMoveVec(float ax, float ay);

public:// �擾�E�ݒ�
    // ---------- Getter,Setter,etc... ----------
#pragma region [Get,Set]Function
    // ----- �X�e�[�g�}�V�� -----
    StateMachine<State<Player>>* GetStateMachine() { return stateMachine_.get(); }
    // --- �X�e�[�g�؂�ւ� ---
    void ChangeState(STATE state);



#pragma endregion// [Get,Set]Function

    // ---------- ���͊֐� ----------
#pragma region [Input]Function
    static bool InputLockOn()
    {
        return (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_RIGHT_THUMB);
    }
#pragma endregion// [Input]Function

private:// �����o�ϐ�
    // ---------- �ړ� ---------------------------------------------
    DirectX::XMFLOAT3 velocity = {}; // ���x
    
    // --- �J�����̌����Ă��������O�Ƃ����ړ������x�N�g�� ---
    DirectX::XMFLOAT3 moveVec  = {};

    // -------------------------------------------------------------

    // ---------- �X�e�[�g�}�V�� ------------------------------------
    std::unique_ptr<StateMachine<State<Player>>> stateMachine_;
    // -------------------------------------------------------------

    // ---------- �V�F�[�_�[ ----------------------------------------
    Microsoft::WRL::ComPtr<ID3D11PixelShader> psShader_ = nullptr;
    // -------------------------------------------------------------
};

