#include "SceneLoadGame.h"

#include "../Core/framework.h"
#include "../Graphics/Graphics.h"

// リソース生成
void SceneLoadGame::CreateResource()
{
    Graphics& graphics = Graphics::Instance();

    loadGameBook_ = std::make_unique<LoadGameBook>();

    // SceneConstant
    sceneConstants_ = std::make_unique<ConstantBuffer<Shader::SceneConstants>>(graphics.GetDevice());
}

// 初期化
void SceneLoadGame::Initialize()
{
    Camera::Instance().GameCameraInitialize();
}

// 終了化
void SceneLoadGame::Finalize()
{
}

// 更新
void SceneLoadGame::Update(const float& elapsedTime)
{
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
        shader->SetBlendState(Shader::BLEND_STATE::NONE);
        shader->SetDepthStencileState(Shader::DEPTH_STATE::ZT_ON_ZW_ON);
        shader->SetRasterizerState(Shader::RASTER_STATE::SOLID);

        loadGameBook_->Render();
    }
}

// ImGui用
void SceneLoadGame::DrawDebug()
{
    ImGui::Begin("SceneLoadGame");

    loadGameBook_->DrawDebug();

    ImGui::End();
}
