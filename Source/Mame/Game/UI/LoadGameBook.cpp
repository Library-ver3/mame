#include "LoadGameBook.h"

#include "../../Graphics/Graphics.h"

// コンストラクタ
LoadGameBook::LoadGameBook()
    : GameObject("./Resources/Model/UI/book.fbx")
{
    Graphics& graphics = Graphics::Instance();

    CreatePsFromCso(graphics.GetDevice(),
        "./Resources/Shader/LoadGameBookPS.cso",
        psShader_.GetAddressOf());
}

// デストラクタ
LoadGameBook::~LoadGameBook()
{
}

// 初期化
void LoadGameBook::Initialize()
{
}

// 終了化
void LoadGameBook::Finalize()
{
}

// 更新
void LoadGameBook::Update(const float& elapsedTime)
{
}

// 描画
void LoadGameBook::Render()
{
    // 描画
    GameObject::Render(0.1f, psShader_.Get());
}

// ImGui用
void LoadGameBook::DrawDebug()
{
    GameObject::DrawDebug();
}
