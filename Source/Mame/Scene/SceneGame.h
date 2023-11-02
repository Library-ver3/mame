#pragma once
#include "BaseScene.h"

#include <d3d11.h>
#include <wrl.h>
#include <memory>

class SceneGame : public BaseScene
{
public: // ���N���X�ɂ���֐�
    SceneGame() {}
    ~SceneGame() override {}

    void CreateResource()                   override; // ���\�[�X����
    void Initialize()                       override; // ������
    void Finalize()                         override; // �I����
    void Update(const float& elapsedTime)   override; // �X�V����
    void Render()                           override; // �`�揈��
    void DrawDebug()                        override; // ImGui�p

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer_ = nullptr;

#ifdef _DEBUG
    bool isDebugCamera = false;
#endif // _DEBUG
};

