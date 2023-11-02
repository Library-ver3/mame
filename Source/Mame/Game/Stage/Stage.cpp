#include "Stage.h"
#include "../../Graphics/Graphics.h"

// ImGui�p
int Stage::nameNum_ = 0;

// �R���X�g���N�^ ( �f�t�H���g )
Stage::Stage(const char* fbxFileName, bool triangulate, float samplingRate)
    : GameObject(fbxFileName, triangulate, samplingRate)
{
}

// �R���X�g���N�^ ( �ʃA�j���[�V�����t�@�C������ )
Stage::Stage(const char* fbxFileName, std::vector<std::string>& animationFileName, bool triangulate, float samplingRate)
    :GameObject(fbxFileName, animationFileName, triangulate, samplingRate)
{
}


// �`��
void Stage::Render(const float& scale, ID3D11PixelShader* psShader)
{
    // �`��
    GameObject::Render(scale, psShader);
}


// ImGui�p
void Stage::DrawDebug()
{
    // Transform
    GetTransform()->DrawDebug();
}
