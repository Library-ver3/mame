#include "SceneTitle.h"

#include "../Graphics/Graphics.h"
#include "../Graphics/shader.h"

#include "../Other/Easing.h"

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
}

// 初期化
void SceneTitle::Initialize()
{
   
}

// 終了化
void SceneTitle::Finalize()
{
}


// 更新処理
void SceneTitle::Update(const float& elapsedTime)
{
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

        pressAnyButtonSprite_->Render(spriteEmissivePS_.Get(), "Emissive");
        LoadGameSprite_->Render(spriteEmissivePS_.Get(), "Emissive");
        QuitGameSprite_->Render(spriteEmissivePS_.Get(), "Emissive");
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
