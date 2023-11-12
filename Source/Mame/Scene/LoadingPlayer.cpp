#include "LoadingPlayer.h"

#include "../Graphics/Graphics.h"

// �R���X�g���N�^
LoadingPlayer::LoadingPlayer()
    : GameObject("./Resources/Model/Character/Player/player.fbx")
{
    sword_ = std::make_unique<GameObject>
        ("./Resources/Model/Character/sword.fbx");

    CreatePsFromCso(Graphics::Instance().GetDevice(),
        "./Resources/Shader/LoadingPS.cso", psShader_.GetAddressOf());
}

// ������
void LoadingPlayer::Initialize()
{
    float scale = 0.8f;

    GetTransform()->SetPosition(DirectX::XMFLOAT3(3.5f, -2.1f, 0.0f));
    GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));
    GetTransform()->SetRotationY(DirectX::XMConvertToRadians(-90));

    int animationIndex = 1;

    sword_->GetTransform()->SetScaleFactor(scale);
    sword_->PlayAnimation(animationIndex, true);
    PlayAnimation(animationIndex, true);
}

// �I����
void LoadingPlayer::Finalize()
{
}

// �X�V
void LoadingPlayer::Update(const float& elapsedTime)
{
    sword_->GetTransform()->SetPosition(GetTransform()->GetPosition());
    sword_->GetTransform()->SetRotation(GetTransform()->GetRotation());
    sword_->UpdateAnimation(elapsedTime);

    UpdateAnimation(elapsedTime);
}

// �`��
void LoadingPlayer::Render(const float& scale, ID3D11PixelShader* psShader)
{
    sword_->Render(scale, psShader_.Get());
    GameObject::Render(scale, psShader_.Get());
}

// ImGui�p
void LoadingPlayer::DrawDebug()
{
    GameObject::DrawDebug();
    sword_->DrawDebug();
}
