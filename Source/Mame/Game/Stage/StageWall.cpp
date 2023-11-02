#include "StageWall.h"

#include "../../Graphics/Graphics.h"
#include "../../Graphics/shader.h"

// �R���X�g���N�^
StageWall::StageWall(const char* fbxFileName) : Stage(fbxFileName)
{
    // �����f�������͊��N���X�ł���Ă���

    Graphics& graphics = Graphics::Instance();

    CreatePsFromCso(graphics.GetDevice(),
        "./Resources/Shader/StageWallPS.cso", psShader_.GetAddressOf());

    // ----- ImGui�p -----
    SetName("StageWall" + std::to_string(GetNameNum()));
}

// ������
void StageWall::Initialize()
{
}

// �I����
void StageWall::Finalize()
{
}

// �X�V
void StageWall::Update(const float& elapsedTime)
{
}

// �`��
void StageWall::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    shader->SetBlendState(Shader::BLEND_STATE::NONE);
    shader->SetDepthStencileState(Shader::DEPTH_STATE::ZT_ON_ZW_ON);
    shader->SetRasterizerState(Shader::RASTER_STATE::SOLID);

    // ���f���`��
    Stage::Render(0.1f, psShader_.Get());
}

// ImGui�p
void StageWall::DrawDebug()
{
    if (ImGui::Begin(GetName()))
    {
        // Transform
        Stage::DrawDebug();

        ImGui::End();
    }
}
