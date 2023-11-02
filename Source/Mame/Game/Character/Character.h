#pragma once
#include "../GameObject.h"

// �L�����N�^�[
class Character : public GameObject
{
public:// ��{�I�Ȋ֐� ( override���Ă�֐��Ȃ� )
    Character(const char* fbxFileName, bool triangulate = true, float samplingRate = 0);
    Character(const char* fbxFileName, std::vector<std::string>& animationFileName, bool triangulate = true, float samplingRate = 0);
    ~Character() override;

    void Initialize()                           override; // ������
    void Finalize()                             override; // �I����
    void Update(const float& elapsedTime)       override; // �X�V
    void Render(const float& scale,
        ID3D11PixelShader* psShader = nullptr)  override; // �`��
    void DrawDebug()                            override; // ImGui�p

    void UpdateConstants()                      override; // �萔�o�b�t�@�[�X�V
};

