#include "SceneLoadGame.h"

#include "../Core/framework.h"
#include "../Graphics/Graphics.h"

// ���\�[�X����
void SceneLoadGame::CreateResource()
{
    Graphics& graphics = Graphics::Instance();

    loadGameBook_ = std::make_unique<LoadGameBook>();

    // SceneConstant
    sceneConstants_ = std::make_unique<ConstantBuffer<Shader::SceneConstants>>(graphics.GetDevice());
}

// ������
void SceneLoadGame::Initialize()
{
    Camera::Instance().GameCameraInitialize();
}

// �I����
void SceneLoadGame::Finalize()
{
}

// �X�V
void SceneLoadGame::Update(const float& elapsedTime)
{
}

// �`��
void SceneLoadGame::Render()
{
    // ----- �`��̏����ݒ� -----
    // ���K���ŏ��ɌĂԁI�I
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

    // 3D���f���`��
    {
        shader->SetBlendState(Shader::BLEND_STATE::NONE);
        shader->SetDepthStencileState(Shader::DEPTH_STATE::ZT_ON_ZW_ON);
        shader->SetRasterizerState(Shader::RASTER_STATE::SOLID);

        loadGameBook_->Render();
    }
}

// ImGui�p
void SceneLoadGame::DrawDebug()
{
    ImGui::Begin("SceneLoadGame");

    loadGameBook_->DrawDebug();

    ImGui::End();
}
