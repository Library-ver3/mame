#include "SceneLoading.h"
#include "SceneManager.h"

#include "../Graphics/Graphics.h"

#include "../Other/Easing.h"
#include "../Other/misc.h"

// コンストラクタ
SceneLoading::SceneLoading(BaseScene* nextScene) :nextScene(nextScene) 
{
}

// リソース生成
void SceneLoading::CreateResource()
{
}

// 初期化
void SceneLoading::Initialize()
{
    // 固定カメラ
    Camera::Instance().FixedInitialize();


    // スレッド開始
    // std::thread(LoadingThread, this);
    // 二個目の引数はLoadingThreadの引数になる
    thread = new std::thread(LoadingThread, this);
}

// 終了化
void SceneLoading::Finalize()
{
    // スレッド終了化
    thread->join();
    if (thread != nullptr)
    {
        delete thread;
        thread = nullptr;
    }
}


// 更新処理
void SceneLoading::Update(const float& elapsedTime)
{
    // シーン切り替え
    if (nextScene->IsReady())
    {
        Mame::Scene::SceneManager::Instance().ChangeScene(nextScene);
    }
}

// 描画処理
void SceneLoading::Render()
{
    // 描画の初期化
    {
        Graphics& graphics = Graphics::Instance();
        Shader* shader = graphics.GetShader();

        BaseScene::RenderInitialize();
    }
}


//// 描画処理
//void SceneLoading::Render(const float& elapsedTime)
//{
//    // 描画の初期化
//    {
//        Graphics& graphics = Graphics::Instance();
//        Shader* shader = graphics.GetShader();
//
//        Mame::Scene::BaseScene::RenderInitialize();
//
//        Camera& camera = Camera::Instance();
//        //Camera::Instance().TitleSetPerspectiveFov(graphics.GetDeviceContext());
//        Camera::Instance().ResultSetPerSpectiveFov(graphics.GetDeviceContext());
//
//        Shader::SceneConstants sceneConstants{};
//        DirectX::XMStoreFloat4x4(&sceneConstants.viewProjection, camera.GetViewMatrix() * camera.GetProjectionMatrix());
//        sceneConstants.lightDirection = shader->GetViewPosition();
//        sceneConstants.cameraPosition = shader->GetViewCamera();
//
//        DirectX::XMStoreFloat4x4(&sceneConstants.inverseProjection, DirectX::XMMatrixInverse(NULL, camera.GetProjectionMatrix()));
//        DirectX::XMStoreFloat4x4(&sceneConstants.inverseViewProjection, DirectX::XMMatrixInverse(NULL, camera.GetViewMatrix() * camera.GetProjectionMatrix()));
//        sceneConstants.time = framework::tictoc.time_stamp();
//
//
//    }
//}

void SceneLoading::DrawDebug()
{
#ifdef USE_IMGUI
    Graphics::Instance().GetShader()->DrawDebug();
#endif// USE_IMGUI
}

// ローディングスレッド
void SceneLoading::LoadingThread(SceneLoading* scene)
{
    // COM関連の初期化でスレッド毎に呼ぶ必要がある
    std::ignore = CoInitialize(nullptr); // std::ignoreで返り値警告解消

    // 次のシーンの初期化を行う
    scene->nextScene->CreateResource();
    scene->nextScene->Initialize();

    // スレッドが終わる前にCOM関連の終了化
    CoUninitialize();

    // 次のシーンの準備完了設定
    scene->nextScene->SetReady();
}