#include "Player.h"

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
    Character::PlayAnimation(0, true);
}

// �I����
void Player::Finalize()
{
}

// �X�V
void Player::Update(const float& elapsedTime)
{
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

// �X�e�[�g�؂�ւ�
void Player::ChangeState(STATE state)
{
    GetStateMachine()->ChangeState(static_cast<UINT>(state));
}
