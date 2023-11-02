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
