#include "SceneTitle.h"
#include "SceneManager.h"
#include "SceneLoadGame.h"

#include "../Graphics/Graphics.h"
#include "../Graphics/shader.h"

#include "../Input/Input.h"

#include "../Other/Easing.h"

#include "SceneTitleState.h"

// コンストラクタ
SceneTitle::SceneTitle(BaseScene* nextScene) : nextScene_(nextScene)
{
}

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
        loadGameSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/Title/LoadGame.png");
        quitGameSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/Title/QuitGame.png");
        blackBeltSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/UI/blackBelt.png");
        quitGameWordSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/Title/quitGameWord.png");
        choseSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/UI/chose.png");
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

        GetStateMachine()->RegisterState(new SceneTitleState::FadeInState(this));
        GetStateMachine()->RegisterState(new SceneTitleState::PressAnyButtonState(this));
        GetStateMachine()->RegisterState(new SceneTitleState::PressAnyButtonFadeOutState(this));
        GetStateMachine()->RegisterState(new SceneTitleState::SelectFadeInState(this));
        GetStateMachine()->RegisterState(new SceneTitleState::SelectState(this));
        GetStateMachine()->RegisterState(new SceneTitleState::QuitGameChoseState(this));
        GetStateMachine()->RegisterState(new SceneTitleState::LoadGameFadeOut(this));

        GetStateMachine()->SetState(static_cast<UINT>(STATE::FadeIn));
    }
}

// 初期化
void SceneTitle::Initialize()
{
    titleLogoSprite_->GetSpriteTransform()->SetPosY(-70);

#pragma region PressAnyButtonSprite_
    pressAnyButtonSprite_->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(480, 450));
    pressAnyButtonSprite_->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(300, 50));
    pressAnyButtonSprite_->GetSpriteTransform()->SetColorA(1.0f);
#pragma endregion// PressAnyButtonSprite_

#pragma region loadGameSprite_
    loadGameSprite_->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(475, 420));
    loadGameSprite_->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(300, 50));
    loadGameSprite_->GetSpriteTransform()->SetColorA(1.0f);
#pragma endregion// loadGameSprite_

#pragma region quitGameSprite_
    quitGameSprite_->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(475, 480));
    quitGameSprite_->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(300, 50));
    quitGameSprite_->GetSpriteTransform()->SetColorA(1.0f);
#pragma endregion// quitGameSprite_

    blackBeltSprite_->GetSpriteTransform()->SetColorA(0.91f);
    blackBeltSprite_->GetEmissive()->SetEmissiveIntensity(2.5f);

    choseSprite_->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(450, 375));
    choseSprite_->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(100, 100));
    choseSprite_->GetSpriteTransform()->SetTexSize(DirectX::XMFLOAT2(100, 100));

    // ----- スレッド開始 -----
    thread_ = new std::thread(LoadingThread, this);
}

// 終了化
void SceneTitle::Finalize()
{
    // スレッド終了
    thread_->join();
    if (thread_ != nullptr)
    {
        delete thread_;
        thread_ = nullptr;
    }
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
        case static_cast<UINT>(STATE::FadeIn):
            pressAnyButtonSprite_->Render(spriteEmissivePS_.Get(), "Emissive");
            break;
        case static_cast<UINT>(STATE::PressAnyButton):
            pressAnyButtonSprite_->Render(spriteEmissivePS_.Get(), "Emissive");
            break;
        case static_cast<UINT>(STATE::PressAnyButtonFadeOut):
            pressAnyButtonSprite_->Render(spriteEmissivePS_.Get(), "Emissive");
            break;
        case static_cast<UINT>(STATE::SelectFadeIn):
            loadGameSprite_->Render(spriteEmissivePS_.Get(), "Emissive");
            quitGameSprite_->Render(spriteEmissivePS_.Get(), "Emissive");
            break;
        case static_cast<UINT>(STATE::Select):
            loadGameSprite_->Render(spriteEmissivePS_.Get(), "Emissive");
            quitGameSprite_->Render(spriteEmissivePS_.Get(), "Emissive");
            break;
        case static_cast<UINT>(STATE::QuitGameChose):
            loadGameSprite_->Render(spriteEmissivePS_.Get(), "Emissive");
            quitGameSprite_->Render(spriteEmissivePS_.Get(), "Emissive");
            blackBeltSprite_->Render(spriteEmissivePS_.Get(), "Emissive");
            break;
        case static_cast<UINT>(STATE::LoadGameFadeOut):
            loadGameSprite_->Render(spriteEmissivePS_.Get(), "Emissive");
            quitGameSprite_->Render(spriteEmissivePS_.Get(), "Emissive");
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


        switch (currentState_)
        {
        case static_cast<UINT>(STATE::QuitGameChose):
            blackBeltSprite_->Render();
            choseSprite_->Render();
            quitGameWordSprite_->Render();
            break;
        }

    }
}


// デバッグ用
void SceneTitle::DrawDebug()
{
    ImGui::Begin("SceneTitle");

    Mouse& mouse = Input::Instance().GetMouse();
    DirectX::XMFLOAT2 mousePos = { static_cast<float>(mouse.GetPositionX()), static_cast<float>(mouse.GetPositionY()) };
    ImGui::InputFloat2("mouse", &mousePos.x);

    titleLogoSprite_->DrawDebug();
    blackBeltSprite_->DrawDebug();
    choseSprite_->DrawDebug();

    ImGui::End();
}

// シーン切り替え
void SceneTitle::ChangeScene()
{
    SceneManager::Instance().ChangeScene(nextScene_);
}

// スレッド
void SceneTitle::LoadingThread(SceneTitle* scene)
{
    // COM関連の初期化でスレッド毎に呼ぶ必要がある
    std::ignore = CoInitialize(nullptr); // std::ignoreで返り値警告解消

    // 次のシーンの初期化を行う
    scene->nextScene_->CreateResource();
    scene->nextScene_->Initialize();

    // スレッドが終わる前にCOM関連の終了化
    CoUninitialize();

    // 次のシーンの準備完了設定
    scene->nextScene_->SetReady();
}
