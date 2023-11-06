#include "LoadGameBook.h"

#include "../../Graphics/Graphics.h"

// �R���X�g���N�^
LoadGameBook::LoadGameBook()
    : GameObject("./Resources/Model/UI/book.fbx")
{
    Graphics& graphics = Graphics::Instance();

    CreatePsFromCso(graphics.GetDevice(),
        "./Resources/Shader/LoadGameBookPS.cso",
        psShader_.GetAddressOf());
}

// �f�X�g���N�^
LoadGameBook::~LoadGameBook()
{
}

// ������
void LoadGameBook::Initialize()
{
}

// �I����
void LoadGameBook::Finalize()
{
}

// �X�V
void LoadGameBook::Update(const float& elapsedTime)
{
}

// �`��
void LoadGameBook::Render()
{
    // �`��
    GameObject::Render(0.1f, psShader_.Get());
}

// ImGui�p
void LoadGameBook::DrawDebug()
{
    GameObject::DrawDebug();
}
