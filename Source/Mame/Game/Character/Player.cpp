#include "Player.h"

#include "../../Graphics/Graphics.h"

// コンストラクタ
Player::Player()
    :Character("./Resources/Model/Character/Player/player.fbx")
{
    Graphics& graphics = Graphics::Instance();

    // pixelShader
    CreatePsFromCso(graphics.GetDevice(),
        "./Resources/Shader/playerPS.cso",
        psShader_.GetAddressOf());
}

// デストラクタ
Player::~Player()
{
}

// 初期化
void Player::Initialize()
{
    Character::PlayAnimation(0, true);
}

// 終了化
void Player::Finalize()
{
}

// 更新
void Player::Update(const float& elapsedTime)
{
}

// 描画
void Player::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Character::Render(scale, psShader_.Get());
}

// ImGui用
void Player::DrawDebug()
{
    if (ImGui::BeginMenu("player"))
    {
        Character::DrawDebug();

        ImGui::EndMenu();
    }
}
