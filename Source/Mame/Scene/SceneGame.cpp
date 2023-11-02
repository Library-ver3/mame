#include "SceneGame.h"

#include "../Core/framework.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/shader.h"

#include "../Game/Stage/StageManager.h"
#include "../Game/Stage/StageWall.h"

// リソース生成
void SceneGame::CreateResource()
{
    Graphics& graphics = Graphics::Instance();

    StageManager::Instance().Register(new StageWall(
        "./Resources/Model/Stage/StageBase"));

    // 定数バッファー
    HRESULT hr{ S_OK };
    D3D11_BUFFER_DESC bufferDesc{};

    bufferDesc.ByteWidth = sizeof(Shader::SceneConstants);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;
    // SceneConstants
    hr = graphics.GetDevice()->CreateBuffer(&bufferDesc, nullptr,
        ConstantBuffer_.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

// 初期化
void SceneGame::Initialize()
{
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

    Shader::SceneConstants sceneConstants{};

    Camera& camera = Camera::Instance();

    camera.DebugSetPerSpectiveFov();
    //camera.FixedSetPerSpectiveFov();
    DirectX::XMStoreFloat4x4(&sceneConstants.viewProjection, camera.GetViewMatrix() * camera.GetProjectionMatrix());
    sceneConstants.lightDirection = shader->GetViewPosition();
    sceneConstants.cameraPosition = shader->GetViewCamera();

    DirectX::XMStoreFloat4x4(&sceneConstants.inverseProjection, DirectX::XMMatrixInverse(NULL, camera.GetProjectionMatrix()));
    DirectX::XMStoreFloat4x4(&sceneConstants.inverseViewProjection, DirectX::XMMatrixInverse(NULL, camera.GetViewMatrix() * camera.GetProjectionMatrix()));
    sceneConstants.time = framework::tictoc.time_stamp();

    graphics.GetDeviceContext()->UpdateSubresource(ConstantBuffer_.Get(), 0, 0, &sceneConstants, 0, 0);
    graphics.GetDeviceContext()->VSSetConstantBuffers(1, 1, ConstantBuffer_.GetAddressOf());
    graphics.GetDeviceContext()->PSSetConstantBuffers(1, 1, ConstantBuffer_.GetAddressOf());

    // ステージ
    StageManager::Instance().Render();
}

// ImGui
void SceneGame::DrawDebug()
{
    ImGui::Begin("SceneGame");

    Camera::Instance().DrawDebug();

    StageManager::Instance().DrawDebug();

    ImGui::End();
}