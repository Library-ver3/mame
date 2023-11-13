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

void Character::Turn(const float& elapsedTime, float vx, float vz, float rotSpeed)
{
    // �ړ��l���Ȃ��ꍇ�������Ȃ�
    if (vx < 0.01f && vx > -0.01f && vz < 0.01f && vz > -0.01f) return;

    rotSpeed = DirectX::XMConvertToRadians(rotSpeed * elapsedTime);

    float length = sqrtf(vx * vx + vz * vz);
    vx /= length;
    vz /= length;

    DirectX::XMFLOAT3 front = GetTransform()->CalcForward();

    float dot = (vx * front.x) + (vz * front.z);
    float rot = 1.0f - dot;
    if (rot < 0.005f)return;
    if (rot < 0.3f) rot = 0.3f;

    rot += 0.5f;
    float rotationSpeed = rotSpeed * rot;

    // ���Z���I�[�o�[�t���[�����Ƃ��͏������Ȃ�
    if (rotationSpeed > 100.0f || rotationSpeed < -100.0f) return;

    // ���E����
    float cross = (vx * front.z) - (vz * front.x);

    DirectX::XMFLOAT4 rotation = GetTransform()->GetRotation();
    rotation.y += cross < 0.0f ? -rotationSpeed : rotationSpeed;

    rotationValue_ = cross < 0.0f ? -rotationSpeed : rotationSpeed;

    GetTransform()->SetRotationY(rotation.y);
}
