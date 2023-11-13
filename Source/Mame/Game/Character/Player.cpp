#include "Player.h"
#include "PlayerState.h"

#include "../../Graphics/Graphics.h"

// �R���X�g���N�^
Player::Player()
    :Character("./Resources/Model/Character/Player/player.fbx")
{
    Graphics& graphics = Graphics::Instance();

    // pixelShader
    CreatePsFromCso(graphics.GetDevice(),
        "./Resources/Shader/playerPS.cso",
        psShader_.GetAddressOf());
}

// �f�X�g���N�^
Player::~Player()
{
}

// ������
void Player::Initialize()
{
    // ----- �A�j���[�V�����ݒ� -----
    Character::PlayAnimation(0, true);

    // ----- �X�e�[�g�}�V�� -----
#pragma region �X�e�[�g�}�V��
    stateMachine_ = std::make_unique<StateMachine<State<Player>>>();

    // --- �X�e�[�g�o�^ ---
    GetStateMachine()->RegisterState(new PlayerState::IdleState(this));
    GetStateMachine()->RegisterState(new PlayerState::WalkState(this));
    GetStateMachine()->RegisterState(new PlayerState::JumpState(this));

    // --- �ŏ��̃X�e�[�g���Z�b�g ---
    GetStateMachine()->SetState(static_cast<UINT>(STATE::Idle));
#pragma endregion// �X�e�[�g�}�V��



}

// �I����
void Player::Finalize()
{
}

// �X�V
void Player::Update(const float& elapsedTime)
{
    // ----- �X�e�[�g�}�V�� -----
    GetStateMachine()->Update(elapsedTime);


}

// �`��
void Player::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Character::Render(scale, psShader_.Get());
}

// ImGui�p
void Player::DrawDebug()
{
    if (ImGui::BeginMenu("player"))
    {
        Character::DrawDebug();

        ImGui::EndMenu();
    }
}

// ���͒l���J�������猩���x�N�g���ɕϊ�����
void Player::ConvertToCameraMoveVec(float ax, float ay)
{
    //  ���͂���������
    if (ax || ay)
    {
        DirectX::XMFLOAT3 forward = Camera::Instance().GetForward();
        DirectX::XMFLOAT3 right   = Camera::Instance().GetRight();

        forward.x *= ay;
        forward.z *= ay;
        right.x   *= ax;
        right.z   *= ax;

        

    }
}

void Player::MoveUpdate(const float& elapsedTime)
{
    
}

// �X�e�[�g�؂�ւ�
void Player::ChangeState(STATE state)
{
    GetStateMachine()->ChangeState(static_cast<UINT>(state));
}
