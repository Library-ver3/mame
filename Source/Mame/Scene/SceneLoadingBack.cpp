#include "SceneLoadingBack.h"
#include "SceneManager.h"

#include "../Graphics/Graphics.h"

#include "../Other/Easing.h"

// コンストラクタ
SceneLoadingBack::SceneLoadingBack(BaseScene* nextScene)
    : nextScene_(nextScene)
{
}

// リソース生成
void SceneLoadingBack::CreateResource()
{
}

// 初期化
void SceneLoadingBack::Initialize()
{
    // 固定カメラ
    Camera::Instance().FixedInitialize();

    // スレッド開始
    thread_ = new std::thread(LoadingThread, this);
}

// 終了化
void SceneLoadingBack::Finalize()
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
void SceneLoadingBack::Update(const float& elapsedTime)
{
    if (nextScene_->IsReady())
    {
        SceneManager::Instance().ChangeScene(nextScene_);
    }
}

// 描画
void SceneLoadingBack::Render()
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    // 描画の初期化
    BaseScene::RenderInitialize();
}

// ImGui用
void SceneLoadingBack::DrawDebug()
{
    Graphics::Instance().GetShader()->DrawDebug();
}

// スレッド
void SceneLoadingBack::LoadingThread(SceneLoadingBack* scene)
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