#include "StageWall.h"

#include "../../Graphics/Graphics.h"
#include "../../Graphics/shader.h"

// コンストラクタ
StageWall::StageWall(const char* fbxFileName) : Stage(fbxFileName)
{
    // ※モデル生成は基底クラスでやっている

    Graphics& graphics = Graphics::Instance();

    CreatePsFromCso(graphics.GetDevice(),
        "./Resources/Shader/StageWallPS.cso", psShader_.GetAddressOf());

    // ----- ImGui用 -----
    SetName("StageWall" + std::to_string(GetNameNum()));
}

// 初期化
void StageWall::Initialize()
{
}

// 終了化
void StageWall::Finalize()
{
}

// 更新
void StageWall::Update(const float& elapsedTime)
{
}

// 描画
void StageWall::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    shader->SetBlendState(Shader::BLEND_STATE::NONE);
    shader->SetDepthStencileState(Shader::DEPTH_STATE::ZT_ON_ZW_ON);
    shader->SetRasterizerState(Shader::RASTER_STATE::SOLID);

    // モデル描画
    Stage::Render(0.1f, psShader_.Get());
}

// ImGui用
void StageWall::DrawDebug()
{
    if (ImGui::Begin(GetName()))
    {
        // Transform
        Stage::DrawDebug();

        ImGui::End();
    }
}
