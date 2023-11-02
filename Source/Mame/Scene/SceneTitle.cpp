#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneLoading.h"

#include "../Graphics/Graphics.h"
#include "../Input/Input.h"

#include "../Resource/AudioManager.h"

#include "../Other/misc.h"
#include "../Resource/texture.h"

#include "../Core/framework.h"

// リソース生成
void SceneTitle::CreateResource()
{
    Graphics& graphics = Graphics::Instance();

    {
        framebuffers[0] = std::make_unique<FrameBuffer>(graphics.GetDevice(), 1280, 720, false);
        bitBlockTransfer = std::make_unique<FullscreenQuad>(graphics.GetDevice());
        CreatePsFromCso(graphics.GetDevice(), "./Resources/Shader/finalPassPs.cso", finalPassPS.GetAddressOf());

        bloomer = std::make_unique<Bloom>(graphics.GetDevice(), 1280 / 1, 720 / 1);
        CreatePsFromCso(graphics.GetDevice(), "./Resources/Shader/finalPassPs.cso", bloomPS.GetAddressOf());

        framebuffers[1] = std::make_unique<FrameBuffer>(graphics.GetDevice(), 1280 / 1, 720 / 1, false);
        CreatePsFromCso(graphics.GetDevice(), "./Resources/Shader/FogPS.cso", fogPS.GetAddressOf());
    }

    // shadow
    {
        shadow.shadowMap = std::make_unique<ShadowMap>(graphics.GetDevice(),
            shadow.shadowMapWidth, shadow.shadowMapHeight);

        HRESULT hr{ S_OK };
        D3D11_BUFFER_DESC desc{};
        desc.ByteWidth = sizeof(Shader::SceneConstants);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;
        hr = graphics.GetDevice()->CreateBuffer(&desc, nullptr, shadowConstantBuffer.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    // 定数バッファー
    {
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
            ConstantBuffer.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
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
}


// デバッグ用
void SceneTitle::DrawDebug()
{
}
