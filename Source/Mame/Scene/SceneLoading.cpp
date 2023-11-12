#include "SceneLoading.h"
#include "SceneManager.h"

#include "../Graphics/Graphics.h"

#include "../Other/Easing.h"
#include "../Other/misc.h"

#include "../Core/framework.h"

// コンストラクタ
SceneLoading::SceneLoading(BaseScene* nextScene) :nextScene_(nextScene)
{
}

// リソース生成
void SceneLoading::CreateResource()
{
    Graphics& graphics = Graphics::Instance();

    loadingPlayer_ = std::make_unique<LoadingPlayer>();

    playerConstants_ = std::make_unique<ConstantBuffer<PlayerConstants>>(graphics.GetDevice());

    sceneConstants_ = std::make_unique<ConstantBuffer<Shader::SceneConstants>>(graphics.GetDevice());
}

// 初期化
void SceneLoading::Initialize()
{
    // --- カメラ ( 固定カメラ ) ---
    Camera::Instance().FixedInitialize();
    
    // --- model初期化 ---
    loadingPlayer_->Initialize();

    // --- 変数初期化 ---
    easingTimer_ = 0.0f;

    playerConstants_->data.color_ = { 0.3f, 0.3f, 0.3f, 1.0f };

    // スレッド開始
    // std::thread(LoadingThread, this);
    // 二個目の引数はLoadingThreadの引数になる
    thread_ = new std::thread(LoadingThread, this);
}

// 終了化
void SceneLoading::Finalize()
{
    // スレッド終了化
    thread_->join();
    if (thread_ != nullptr)
    {
        delete thread_;
        thread_ = nullptr;
    }
}


// 更新処理
void SceneLoading::Update(const float& elapsedTime)
{
    loadingPlayer_->Update(elapsedTime);

    // 次のシーンが準備できたら
    if (nextScene_->IsReady())
    {
        // フェードアウトする
        if (FadeOutUi(elapsedTime))
        {   // フェードアウト終わったら
            // シーン切り替え
            SceneManager::Instance().ChangeScene(nextScene_);
            return;
        }
    }
}

// 描画処理
void SceneLoading::Render()
{
    // 描画の初期設定※必ず呼ぶこと！！！
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

    // モデル描画
    {
        playerConstants_->Activate(graphics.GetDeviceContext(), 5, false, true);
        loadingPlayer_->Render(0.01f);
    }
}


void SceneLoading::DrawDebug()
{
#ifdef USE_IMGUI
    Graphics::Instance().GetShader()->DrawDebug();

    loadingPlayer_->DrawDebug();

#endif// USE_IMGUI
}

// UIのフェイドアウト
bool SceneLoading::FadeOutUi(const float& elapsedTime)
{
    float maxTime = 1.0f;
    if (easingTimer_ <= maxTime)
    {
        float playerColor = Easing::InSine(easingTimer_, maxTime, 0.0f, 0.3f);

        playerConstants_->data.color_ = { playerColor, playerColor, playerColor, 1.0f };

        easingTimer_ += elapsedTime;
        return false; // フェードアウト中
    }
    else
    {
        playerConstants_->data.color_ = { 0.0f, 0.0f, 0.0f, 1.0f };
        return true; // フェードアウト終わった
    }
}

// ローディングスレッド
void SceneLoading::LoadingThread(SceneLoading* scene)
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