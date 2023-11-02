#include "SceneGame.h"

#include "../Core/framework.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/shader.h"

#include "../Game/Stage/StageManager.h"
#include "../Game/Stage/StageWall.h"

#include "../Game/Character/PlayerManager.h"

// リソース生成
void SceneGame::CreateResource()
{
    Graphics& graphics = Graphics::Instance();

    StageManager::Instance().Register(new StageWall(
        "./Resources/Model/Stage/StageBase"));

    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

    // SceneConstant
    sceneConstants_ = std::make_unique<ConstantBuffer<Shader::SceneConstants>>(graphics.GetDevice());
}

// 初期化
void SceneGame::Initialize()
{
    Camera::Instance().GameCameraInitialize();
}

// 終了化
void SceneGame::Finalize()
{
}

// 更新処理
void SceneGame::Update(const float& elapsedTime)
{
    if (GetAsyncKeyState('U') & 0x01)isDebugCamera = !isDebugCamera;
    if (isDebugCamera)
    {
        int posX = 1980 / 2;
        int posY = 1080 / 2;

        POINT pos;
        GetCursorPos(&pos);

        DirectX::XMFLOAT2 nowPosition{ static_cast<float>(pos.x),static_cast<float>(pos.y) };
        DirectX::XMFLOAT2 oldPosition{ static_cast<float>(posX),static_cast<float>(posY) };
        DirectX::XMVECTOR nowVector = DirectX::XMLoadFloat2(&nowPosition);
        DirectX::XMVECTOR oldVector = DirectX::XMLoadFloat2(&oldPosition);
        DirectX::XMVECTOR moveVector = DirectX::XMVectorSubtract(nowVector, oldVector);
        DirectX::XMFLOAT2 moveVectorFloat2;
        DirectX::XMStoreFloat2(&moveVectorFloat2, moveVector);

        Camera::Instance().UpdateDebug(elapsedTime, moveVectorFloat2);

        SetCursorPos(posX, posY);
    }
}

// 描画処理
void SceneGame::Render()
{
    // ----- 描画の初期設定 -----
    // ※必ず最初に呼ぶ！！
    BaseScene::RenderInitialize();

    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    Camera& camera = Camera::Instance();

    camera.GameCameraSetPerSpectiveFov();
    //camera.FixedSetPerSpectiveFov();
    DirectX::XMStoreFloat4x4(&sceneConstants_->data.viewProjection, camera.GetViewMatrix() * camera.GetProjectionMatrix());
    sceneConstants_->data.lightDirection = shader->GetViewPosition();
    sceneConstants_->data.cameraPosition = shader->GetViewCamera();

    DirectX::XMStoreFloat4x4(&sceneConstants_->data.inverseProjection, DirectX::XMMatrixInverse(NULL, camera.GetProjectionMatrix()));
    DirectX::XMStoreFloat4x4(&sceneConstants_->data.inverseViewProjection, DirectX::XMMatrixInverse(NULL, camera.GetViewMatrix() * camera.GetProjectionMatrix()));
    sceneConstants_->data.time = framework::tictoc.time_stamp();

    sceneConstants_->Activate(graphics.GetDeviceContext(), 1);

    // 3Dモデル描画
    {
        // ステージ
        StageManager::Instance().Render();

        // player
        PlayerManager::Instance().Render();
    }
}

// ImGui
void SceneGame::DrawDebug()
{
    ImGui::Begin("SceneGame");

    Camera::Instance().DrawDebug();

    StageManager::Instance().DrawDebug();

    ImGui::End();
}