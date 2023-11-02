#include "Stage.h"
#include "../../Graphics/Graphics.h"

// ImGui用
int Stage::nameNum_ = 0;

// コンストラクタ ( デフォルト )
Stage::Stage(const char* fbxFileName, bool triangulate, float samplingRate)
    : GameObject(fbxFileName, triangulate, samplingRate)
{
}

// コンストラクタ ( 別アニメーションファイルあり )
Stage::Stage(const char* fbxFileName, std::vector<std::string>& animationFileName, bool triangulate, float samplingRate)
    :GameObject(fbxFileName, animationFileName, triangulate, samplingRate)
{
}


// 描画
void Stage::Render(const float& scale, ID3D11PixelShader* psShader)
{
    // 描画
    GameObject::Render(scale, psShader);
}


// ImGui用
void Stage::DrawDebug()
{
    // Transform
    GetTransform()->DrawDebug();
}
