#include "SceneLoadGame.h"
#include "SceneManager.h"
#include "SceneTitle.h"

#include "../Core/framework.h"
#include "../Graphics/Graphics.h"

#include "SceneLoadGameState.h"
#include "../Other/Easing.h"

#include "../Resource/NoiseTexture.h"

#include "SceneLoading.h"
#include "SceneGame.h"

SceneLoadGame::SceneLoadGame()
{
}

// リソース生成
void SceneLoadGame::CreateResource()
{
    Graphics& graphics = Graphics::Instance();

    // Sprite
    {
        bookSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/UI/book.png");

        whiteSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/UI/white.png");

        backSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/LoadGame/back.png");

        for (int i = 0; i < static_cast<UINT>(GameDataBase::Max); ++i)
        {
            gameDataBaseSprite_[i] = std::make_unique<Sprite>(graphics.GetDevice(),
                L"./Resources/Image/LoadGame/base.png");
        }

        gameDataChoseSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/LoadGame/chose.png");

        gameDataWordSprite_[0] = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/LoadGame/gameData1.png");
        gameDataWordSprite_[1] = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/LoadGame/gameData2.png");
        gameDataWordSprite_[2] = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/LoadGame/gameData2.png");

        blackBeltSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/UI/blackBelt.png");
        loadGameWordSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/LoadGame/loadGameWord.png");
        choseSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/UI/chose.png");

        halfBlackBeltSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/UI/blackBelt.png");
        loadingWordSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/LoadGame/loadingWord.png");
        completeWordSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/LoadGame/complete.png");
        loadArrowSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/LoadGame/arrow.png");

        choseLoadDataWordSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/LoadGame/choseLoadDataWord.png");
        pointWakuSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/UI/waku.png");
        pointRhombusSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/UI/rhombus.png");

        gameDataNumSprite_[0] = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/LoadGame/01.png");
        gameDataNumSprite_[1] = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/LoadGame/02.png");
        gameDataNumSprite_[2] = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/LoadGame/03.png");
    }

    // ステートマシン
    {
        stateMachine_.reset(new StateMachine<State<SceneLoadGame>>);

        GetStateMachine()->RegisterState(new SceneLoadGameState::SlideUiState(this));
        GetStateMachine()->RegisterState(new SceneLoadGameState::SelectState(this));
        GetStateMachine()->RegisterState(new SceneLoadGameState::LoadGameChoseState(this));
        GetStateMachine()->RegisterState(new SceneLoadGameState::SlideUiFadeOutState(this));
        GetStateMachine()->RegisterState(new SceneLoadGameState::LoadGameState(this));
        GetStateMachine()->RegisterState(new SceneLoadGameState::LoadTitleState(this));
        GetStateMachine()->RegisterState(new SceneLoadGameState::ChangeSceneState(this));

        GetStateMachine()->SetState(static_cast<UINT>(STATE::SlideUi));
    }

    CreatePsFromCso(graphics.GetDevice(), "./Resources/Shader/SpriteDissolvePS.cso",
        spriteDissolvePS_.GetAddressOf());

    // SceneConstant
    sceneConstants_ = std::make_unique<ConstantBuffer<Shader::SceneConstants>>(graphics.GetDevice());
}

// 初期化
void SceneLoadGame::Initialize()
{
    Camera::Instance().FixedInitialize();

    // Sprite初期化
    {
        bookSprite_->GetSpriteTransform()->SetPos(0, -720);

        whiteSprite_->GetSpriteTransform()->SetColor(0, 0, 0, 1);

        backSprite_->GetSpriteTransform()->SetSize(1280, 720);

        for (int i = 0; i < static_cast<UINT>(GameDataBase::Max); ++i)
        {
            gameDataBaseSprite_[i]->GetSpriteTransform()->SetPos(spritePos[i]);
            gameDataWordSprite_[i]->GetSpriteTransform()->SetPos(spritePos[i]);
        }

        // ゲームデータがないので暗くしてる
        gameDataBaseSprite_[2]->GetSpriteTransform()->SetColor(0.4f, 0.4f, 0.4f);
        gameDataWordSprite_[2]->GetSpriteTransform()->SetColorA(0.0f);

        gameDataChoseSprite_->GetSpriteTransform()->SetPos(spritePos[0]);

#pragma region choseSprite_
        choseSprite_->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(450, 375));
        choseSprite_->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(100, 100));
        choseSprite_->GetSpriteTransform()->SetTexSize(DirectX::XMFLOAT2(100, 100));
#pragma endregion// choseSprite_

        halfBlackBeltSprite_->GetSpriteTransform()->SetPos(0, 180);
        halfBlackBeltSprite_->GetSpriteTransform()->SetSize(1280, 380);

        completeWordSprite_->GetSpriteTransform()->SetPos(0, 5);

        loadArrowSprite_->GetSpriteTransform()->SetPos(625, 395);
        loadArrowSprite_->GetSpriteTransform()->SetSize(36, 36);

        pointWakuSprite_->GetSpriteTransform()->SetPos(105, 85);
        pointWakuSprite_->GetSpriteTransform()->SetSize(60, 60);
        pointRhombusSprite_->GetSpriteTransform()->SetPos(105, 85);
        pointRhombusSprite_->GetSpriteTransform()->SetSize(60, 60);

        // 01,02,03の数字初期化
        for (int i = 0; i < static_cast<UINT>(GameDataBase::Max); ++i)
        {
            gameDataNumSprite_[i]->GetSpriteTransform()->SetSize(64, 37.5f);
        }
        gameDataNumSprite_[0]->GetSpriteTransform()->SetPos(260, 200);
        gameDataNumSprite_[0]->GetSpriteTransform()->SetAngle(-18);
        gameDataNumSprite_[1]->GetSpriteTransform()->SetPos(290, 365);
        gameDataNumSprite_[1]->GetSpriteTransform()->SetAngle(22);
        gameDataNumSprite_[2]->GetSpriteTransform()->SetPos(245, 540);
        gameDataNumSprite_[2]->GetSpriteTransform()->SetAngle(-10);
        // ゲームデータがないので暗くしている
        gameDataNumSprite_[2]->GetSpriteTransform()->SetColor(0.4f, 0.4f, 0.4f);

    }

    // 変数初期化
    fadeInTimer_ = 0.0f;
    isFadeIn_ = true;
    isFadeOut_ = false;

    isPointExpansion_ = false;
    pointTimer_ = 0.0f;
}

// 終了化
void SceneLoadGame::Finalize()
{
    // スレッド終了化
    thread_->join();
    if (thread_ != nullptr)
    {
        delete thread_;
        thread_ = nullptr;
    }
}

// 更新
void SceneLoadGame::Update(const float& elapsedTime)
{
    // ステートマシン更新
    GetStateMachine()->Update(elapsedTime);

    UpdatePointRhombusSprite(elapsedTime);

    if (isFadeIn_ && !isFadeOut_)
    {   // フェードイン
        float maxTime = 0.4f;
        if (fadeInTimer_ <= maxTime)
        {
            float alpha = Easing::InSine(fadeInTimer_, maxTime, 0.0f, 1.0f);
            whiteSprite_->GetSpriteTransform()->SetColorA(alpha);
            fadeInTimer_ += elapsedTime;
        }
        else
        {
            whiteSprite_->GetSpriteTransform()->SetColorA(0.0f);
            isFadeIn_ = false;
        }
    }
    else if (!isFadeIn_ && isFadeOut_)
    {   // フェードアウト
        // フェードアウトはステートマシンの中でやった方がきれいになったので、
        // ここでは処理書かない
    }

    // [01,02,03文字振動]
    if (GetCurrentState() != static_cast<UINT>(STATE::SlideUi)
        || GetCurrentState() != static_cast<UINT>(STATE::SlideUiFadeOut)
        || GetCurrentState() != static_cast<UINT>(STATE::LoadTitle))
    {
        gameDataNumSprite_[GetSelectState()]->Vibration(elapsedTime, 1.5f, 0.07f);
    }

}

// 描画
void SceneLoadGame::Render()
{
    // ----- 描画の初期設定 -----
    // ※必ず最初に呼ぶ！！
    BaseScene::RenderInitialize();

    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    Camera& camera = Camera::Instance();

    camera.FixedSetPerSpectiveFov();
    DirectX::XMStoreFloat4x4(&sceneConstants_->data.viewProjection, camera.GetViewMatrix() * camera.GetProjectionMatrix());
    sceneConstants_->data.lightDirection = shader->GetViewPosition();
    sceneConstants_->data.cameraPosition = shader->GetViewCamera();

    DirectX::XMStoreFloat4x4(&sceneConstants_->data.inverseProjection, DirectX::XMMatrixInverse(NULL, camera.GetProjectionMatrix()));
    DirectX::XMStoreFloat4x4(&sceneConstants_->data.inverseViewProjection, DirectX::XMMatrixInverse(NULL, camera.GetViewMatrix() * camera.GetProjectionMatrix()));
    sceneConstants_->data.time = framework::tictoc.time_stamp();

    sceneConstants_->Activate(graphics.GetDeviceContext(), 1);

    // Sprite描画 ( 背景 )
    {
        shader->SetBlendState(Shader::BLEND_STATE::ALPHA);
        shader->SetDepthStencileState(Shader::DEPTH_STATE::ZT_OFF_ZW_OFF);
        shader->SetRasterizerState(Shader::RASTER_STATE::CULL_NONE);

        backSprite_->Render();

        bookSprite_->Render();
    }

    // 3Dモデル描画
    {
        shader->SetBlendState(Shader::BLEND_STATE::NONE);
        shader->SetDepthStencileState(Shader::DEPTH_STATE::ZT_ON_ZW_ON);
        shader->SetRasterizerState(Shader::RASTER_STATE::SOLID);

        
    }

    // Sprite描画
    {
        shader->SetBlendState(Shader::BLEND_STATE::ALPHA);
        shader->SetDepthStencileState(Shader::DEPTH_STATE::ZT_OFF_ZW_OFF);
        shader->SetRasterizerState(Shader::RASTER_STATE::CULL_NONE);

        // マスク画像セット スロット3
        NoiseTexture::Instance().SetConstantBuffers(3);

        choseLoadDataWordSprite_->Render();
        pointWakuSprite_->Render();
        pointRhombusSprite_->Render();

        // ※描画順に注意
        // base -> chose -> num -> word
        {
            // base
            for (int i = 0; i < static_cast<UINT>(GameDataBase::Max); ++i)
            {
                gameDataBaseSprite_[i]->Render();
            }

            // chose
            gameDataChoseSprite_->Render(spriteDissolvePS_.Get(), "Dissolve");
            
            // num
            for (int i = 0; i < static_cast<UINT>(GameDataBase::Max); ++i)
            {
                gameDataNumSprite_[i]->Render();
            }

            // word
            for (int i = 0; i < static_cast<UINT>(GameDataBase::Max); ++i)
            {
                gameDataWordSprite_[i]->Render();
            }
        }

        // ゲームをロードします の文字を出す
        if (currentState_ == static_cast<UINT>(STATE::LoadGameChose))
        {
            blackBeltSprite_->Render();
            choseSprite_->Render();
            loadGameWordSprite_->Render();
        }

        if (currentState_ == static_cast<UINT>(STATE::LoadGame))
        {
            halfBlackBeltSprite_->Render();
            loadingWordSprite_->Render();
            completeWordSprite_->Render();
            loadArrowSprite_->Render();
        }
    }

    // ----- これより下に何も書かない -----
    whiteSprite_->Render();
    // ----- これより下に何も書かない -----
}

// ImGui用
void SceneLoadGame::DrawDebug()
{
    ImGui::Begin("SceneLoadGame");

    if (ImGui::Button("title"))
    {
        SceneManager::Instance().ChangeScene(new SceneTitle(new SceneLoadGame));
    }

    if (ImGui::Button("reset"))
    {
        GetStateMachine()->ChangeState(static_cast<UINT>(STATE::SlideUi));
    }

    Mouse& mouse = Input::Instance().GetMouse();
    DirectX::XMFLOAT2 mousePos = { static_cast<float>(mouse.GetPositionX()), static_cast<float>(mouse.GetPositionY()) };
    ImGui::InputFloat2("mouse", &mousePos.x);

    for (int i = 0; i < static_cast<UINT>(GameDataBase::Max); ++i)
    {
        gameDataNumSprite_[i]->DrawDebug();
    }

    choseLoadDataWordSprite_->DrawDebug();
    pointWakuSprite_->DrawDebug();
    pointRhombusSprite_->DrawDebug();

    bookSprite_->DrawDebug();

    gameDataChoseSprite_->DrawDebug();    

    backSprite_->DrawDebug();
    for (int i = 0; i < static_cast<UINT>(GameDataBase::Max); ++i)
    {
        gameDataBaseSprite_[i]->DrawDebug();
    }

    ImGui::End();
}

void SceneLoadGame::ChangeScene()
{
    if (loadingScene_->IsReady())
    {
        SceneManager::Instance().ChangeScene(loadingScene_);
    }
}

// 点滅更新
void SceneLoadGame::UpdatePointRhombusSprite(const float& elapsedTime)
{
    // UIがフェードイン。またはフェードアウトしているときは更新しない
    if (GetCurrentState() == static_cast<UINT>(STATE::SlideUi)
        || GetCurrentState() == static_cast<UINT>(STATE::SlideUiFadeOut)
        || GetCurrentState() == static_cast<UINT>(STATE::LoadTitle))
        return;

    pointRhombusSprite_->GetSpriteTransform()->SetSpriteCenterPos(105, 85, 60);

    float maxTime = 0.4f;
    float maxSize = 70.0f;
    float minSize = 45.0f;
    float maxAlpha = 1.0f;
    float minAlpha = 0.0f;
    if (isPointExpansion_)
    {
        if (pointTimer_ <= maxTime)
        {
            float size = Easing::InSine(pointTimer_, maxTime, maxSize, minSize);
            float alpha = Easing::InCubic(pointTimer_, maxTime, minAlpha, maxAlpha);

            pointRhombusSprite_->GetSpriteTransform()->SetSize(size);
            pointRhombusSprite_->GetSpriteTransform()->SetColorA(alpha);

            pointTimer_ += elapsedTime;
        }
        else
        {
            pointTimer_ = 0.0f;
            isPointExpansion_ = !isPointExpansion_;
        }
    }
    else
    {
        if (pointTimer_ <= maxTime)
        {
            float size = Easing::InSine(pointTimer_, maxTime, minSize, maxSize);
            float alpha = Easing::OutCubic(pointTimer_, maxTime, maxAlpha, minAlpha);

            pointRhombusSprite_->GetSpriteTransform()->SetSize(size);
            pointRhombusSprite_->GetSpriteTransform()->SetColorA(alpha);

            pointTimer_ += elapsedTime;
        }
        else
        {
            pointTimer_ = 0.0f;
            isPointExpansion_ = !isPointExpansion_;
        }
    }
}

void SceneLoadGame::LoadingThread(SceneLoadGame* scene)
{
    // COM関連の初期化でスレッド毎に呼ぶ必要がある
    std::ignore = CoInitialize(nullptr); // std::ignoreで返り値警告解消

    scene->loadingScene_->CreateResource();
    scene->loadingScene_->Initialize();
    
    // スレッドが終わる前にCOM関連の終了化
    CoUninitialize();

    // 次のシーンの準備完了設定
    scene->loadingScene_->SetReady();
}
