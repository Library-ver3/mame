#pragma once
#include "../GameObject.h"

// �X�e�[�W
class Stage : public GameObject
{
public:
    Stage(const char* fbxFileName, bool triangulate = true, float samplingRate = 0);
    Stage(const char* fbxFileName, std::vector<std::string>& animationFileName, bool triangulate = true, float samplingRate = 0);
    virtual ~Stage() {}

    void Initialize()                       override {} // ������
    void Finalize()                         override {} // �I����
    void Update(const float& elapsedTime)   override {} // �X�V
    void Render(const float& scale,
        ID3D11PixelShader* psShader)        override;   // �`��
    void DrawDebug()                        override;   // ImGui�p

protected:// ----- ImGui�p ----- //
    const int GetNameNum() const { return nameNum_++; }

private:// ----- ImGui�p ----- //
    static int nameNum_;    // ���O���Ԃ�Ȃ����߂̐���
};

