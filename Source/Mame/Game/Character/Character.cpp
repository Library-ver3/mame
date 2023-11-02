#include "Character.h"

// �R���X�g���N�^ ( �f�t�H���g )
Character::Character(const char* fbxFileName, bool triangulate, float samplingRate)
    :GameObject(fbxFileName, triangulate, samplingRate)
{
}

// �R���X�g���N�^ ( �ʃA�j���[�V�����t�@�C������ )
Character::Character(const char* fbxFileName, std::vector<std::string>& animationFileName, bool triangulate, float samplingRate)
    :GameObject(fbxFileName, animationFileName, triangulate, samplingRate)
{
}

// �f�X�g���N�^
Character::~Character()
{
}

// ������
void Character::Initialize()
{
}

// �I����
void Character::Finalize()
{
}

// �X�V
void Character::Update(const float& elapsedTime)
{
}

// �`��
void Character::Render(const float& scale, ID3D11PixelShader* psShader)
{
    // �`��
    GameObject::Render(scale, psShader);
}

// ImGui�p
void Character::DrawDebug()
{
}

// ConstantBuffer
void Character::UpdateConstants()
{
}
