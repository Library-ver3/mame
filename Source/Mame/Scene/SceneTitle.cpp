#include "SceneTitle.h"

#include "../Graphics/Graphics.h"
#include "../Graphics/shader.h"

#include "../Input/Input.h"

#include "../Other/Easing.h"

#include "SceneTitleState.h"

// リソース生成
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

    // シェーダー関連
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

    // ステートマシン
    {
        stateMachine_.reset(new StateMachine<State<SceneTitle>>);

        GetStateMachine()->RegisterState(new SceneTitleState::PressAnyButtonState(this));
        GetStateMachine()->RegisterState(new SceneTitleState::PressAnyButtonFadeOutState(this));
        GetStateMachine()->RegisterState(new SceneTitleState::SelectFadeInState(this));
        GetStateMachine()->RegisterState(new SceneTitleState::SelectState(this));

        GetStateMachine()->SetState(static_cast<UINT>(STATE::PressAnyButton));
    }
}

// 初期化
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

// 終了化
void SceneTitle::Finalize()
{
}


// 更新処理
void SceneTitle::Update(const float& elapsedTime)
{
    // ステートマシン更新
    GetStateMachine()->Update(elapsedTime);
}


// 描画処理
void SceneTitle::Render()
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    Shader::SceneConstants sceneConstants{};

    // 描画の初期設定※必ず呼ぶこと！！！
    BaseScene::RenderInitialize();

    frameBuffer_->Clear(graphics.GetDeviceContext());
    frameBuffer_->Activate(graphics.GetDeviceContext());

    // ブルーム有 Model
    {

    }

    // ブルーム有 Sprite
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

    // bloom作成
    bloom_->Make(graphics.GetDeviceContext(), frameBuffer_->shaderResourceViews[0].Get());

    ID3D11ShaderResourceView* shaderResourceViews[] =
    {
        frameBuffer_->shaderResourceViews[0].Get(),
        bloom_->ShaderResourceView(),
    };

    // ブルーム有.描画
    bitBlockTransfer_->Blit(graphics.GetDeviceContext(), shaderResourceViews, 0, _countof(shaderResourceViews), bloomPS_.Get());

    // ブルーム無 Model
    {

    }

    // ブルーム無 Sprite
    {
        titleLogoSprite_->Render();
    }
}


// デバッグ用
void SceneTitle::DrawDebug()
{
    ImGui::Begin("SceneTitle");

    titleLogoSprite_->DrawDebug();
    pressAnyButtonSprite_->DrawDebug();
    LoadGameSprite_->DrawDebug();
    QuitGameSprite_->DrawDebug();

    ImGui::End();
}
