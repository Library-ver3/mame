#include "SceneTitle.h"

#include "../Graphics/Graphics.h"
#include "../Graphics/shader.h"

#include "../Input/Input.h"

#include "../Other/Easing.h"

#include "SceneTitleState.h"

// ���\�[�X����
void SceneTitle::CreateResource()
{
    Graphics& graphics = Graphics::Instance();

    // Sprite
    {
        titleLogoSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/Title/Emma.png");
        pressAnyButtonSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/Title/PressAnyButton.png");
        LoadGameSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/Title/LoadGame.png");
        QuitGameSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/Title/QuitGame.png");
    }

    // �V�F�[�_�[�֘A
    {
        frameBuffer_ = std::make_unique<FrameBuffer>(graphics.GetDevice(), 1280, 720);
        bitBlockTransfer_ = std::make_unique<FullscreenQuad>(graphics.GetDevice());

        bloom_ = std::make_unique<Bloom>(graphics.GetDevice(), 1280, 720);
        CreatePsFromCso(graphics.GetDevice(),
            "./Resources/Shader/TitleFinalPassPS.cso",
            bloomPS_.GetAddressOf());

        CreatePsFromCso(graphics.GetDevice(),
            "./Resources/Shader/SpriteEmissivePS.cso",
            spriteEmissivePS_.GetAddressOf());
    }

    // �X�e�[�g�}�V��
    {
        stateMachine_.reset(new StateMachine<State<SceneTitle>>);

        GetStateMachine()->RegisterState(new SceneTitleState::PressAnyButtonState(this));
        GetStateMachine()->RegisterState(new SceneTitleState::PressAnyButtonFadeOutState(this));
        GetStateMachine()->RegisterState(new SceneTitleState::SelectFadeInState(this));
        GetStateMachine()->RegisterState(new SceneTitleState::SelectState(this));

        GetStateMachine()->SetState(static_cast<UINT>(STATE::PressAnyButton));
    }
}

// ������
void SceneTitle::Initialize()
{
#pragma region PressAnyButtonSprite_
    pressAnyButtonSprite_->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(480, 450));
    pressAnyButtonSprite_->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(300, 50));
    pressAnyButtonSprite_->GetSpriteTransform()->SetColorA(1.0f);
#pragma endregion// PressAnyButtonSprite_

#pragma region LoadGame
    LoadGameSprite_->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(475, 420));
    LoadGameSprite_->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(300, 50));
    LoadGameSprite_->GetSpriteTransform()->SetColorA(1.0f);
#pragma endregion

#pragma region QuitGame
    QuitGameSprite_->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(475, 480));
    QuitGameSprite_->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(300, 50));
    QuitGameSprite_->GetSpriteTransform()->SetColorA(1.0f);
#pragma endregion
}

// �I����
void SceneTitle::Finalize()
{
}


// �X�V����
void SceneTitle::Update(const float& elapsedTime)
{
    // �X�e�[�g�}�V���X�V
    GetStateMachine()->Update(elapsedTime);
}


// �`�揈��
void SceneTitle::Render()
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    Shader::SceneConstants sceneConstants{};

    // �`��̏����ݒ聦�K���ĂԂ��ƁI�I�I
    BaseScene::RenderInitialize();

    frameBuffer_->Clear(graphics.GetDeviceContext());
    frameBuffer_->Activate(graphics.GetDeviceContext());

    // �u���[���L Model
    {

    }

    // �u���[���L Sprite
    {
        shader->SetDepthStencileState(Shader::DEPTH_STATE::ZT_OFF_ZW_OFF);
        shader->SetBlendState(Shader::BLEND_STATE::ALPHA);
        shader->SetRasterizerState(Shader::RASTER_STATE::CULL_NONE);

        switch (currentState_)
        {
        case static_cast<UINT>(STATE::PressAnyButton):
            pressAnyButtonSprite_->Render(spriteEmissivePS_.Get(), "Emissive");
            break;
        case static_cast<UINT>(STATE::PressAnyButtonFadeOut):
            pressAnyButtonSprite_->Render(spriteEmissivePS_.Get(), "Emissive");
            break;
        case static_cast<UINT>(STATE::SelectFadeIn):
            LoadGameSprite_->Render(spriteEmissivePS_.Get(), "Emissive");
            QuitGameSprite_->Render(spriteEmissivePS_.Get(), "Emissive");
            break;
        case static_cast<UINT>(STATE::Select):
            LoadGameSprite_->Render(spriteEmissivePS_.Get(), "Emissive");
            QuitGameSprite_->Render(spriteEmissivePS_.Get(), "Emissive");
            break;
        }

    }

    frameBuffer_->Deactivate(graphics.GetDeviceContext());

    // bloom�쐬
    bloom_->Make(graphics.GetDeviceContext(), frameBuffer_->shaderResourceViews[0].Get());

    ID3D11ShaderResourceView* shaderResourceViews[] =
    {
        frameBuffer_->shaderResourceViews[0].Get(),
        bloom_->ShaderResourceView(),
    };

    // �u���[���L.�`��
    bitBlockTransfer_->Blit(graphics.GetDeviceContext(), shaderResourceViews, 0, _countof(shaderResourceViews), bloomPS_.Get());

    // �u���[���� Model
    {

    }

    // �u���[���� Sprite
    {
        titleLogoSprite_->Render();
    }
}


// �f�o�b�O�p
void SceneTitle::DrawDebug()
{
    ImGui::Begin("SceneTitle");

    titleLogoSprite_->DrawDebug();
    pressAnyButtonSprite_->DrawDebug();
    LoadGameSprite_->DrawDebug();
    QuitGameSprite_->DrawDebug();

    ImGui::End();
}
