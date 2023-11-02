#include "Character.h"

// コンストラクタ ( デフォルト )
Character::Character(const char* fbxFileName, bool triangulate, float samplingRate)
    :GameObject(fbxFileName, triangulate, samplingRate)
{
}

// コンストラクタ ( 別アニメーションファイルあり )
Character::Character(const char* fbxFileName, std::vector<std::string>& animationFileName, bool triangulate, float samplingRate)
    :GameObject(fbxFileName, animationFileName, triangulate, samplingRate)
{
}

// デストラクタ
Character::~Character()
{
}

// 初期化
void Character::Initialize()
{
}

// 終了化
void Character::Finalize()
{
}

// 更新
void Character::Update(const float& elapsedTime)
{
}

// 描画
void Character::Render(const float& scale, ID3D11PixelShader* psShader)
{
    // 描画
    GameObject::Render(scale, psShader);
}

// ImGui用
void Character::DrawDebug()
{
}

// ConstantBuffer
void Character::UpdateConstants()
{
}
