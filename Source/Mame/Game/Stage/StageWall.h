#pragma once
#include "Stage.h"

// �X�e�[�W�ɉe�̉f�荞�݂Ȃ��N���X
class StageWall : public Stage
{
public:
    StageWall(const char* fbxFileName);
    ~StageWall() {}

    void Initialize()                                               override;   // ������
    void Finalize()                                                 override;   // �I����
    void Update(const float& elapsedTime)                           override;   // �X�V
    void Render(const float& scale, ID3D11PixelShader* psShader)    override;   // �`��
    void DrawDebug()                                                override;   // ImGui�p

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> psShader_ = nullptr;
};

