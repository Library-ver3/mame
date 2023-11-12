#pragma once

#include "BaseScene.h"

#include <thread>
#include <memory>

#include "../Resource/sprite.h"

#include "LoadingPlayer.h"
#include "../Graphics/ConstantBuffer.h"

#include "../Graphics/shader.h"

class SceneLoading : public BaseScene
{
public:// ��{�I�Ȋ֐�
    SceneLoading(BaseScene* nextScene);
    ~SceneLoading() override {}

    void CreateResource()                   override; // ���\�[�X����
    void Initialize()                       override; // ������
    void Finalize()                         override; // �I����
    void Update(const float& elapsedTime)   override; // �X�V����
    void Render()                           override; // �`�揈��

    void DrawDebug()    override;

private:
    bool FadeOutUi(const float& elapsedTime);

private:
    // ---------- �V�[���؂�ւ��p ----------------------------
    float easingTimer_  = 0.0f;
    // -------------------------------------------------------

    // ---------- �V�F�[�_�[ -----------------------------------
    std::unique_ptr<LoadingPlayer> loadingPlayer_ = nullptr;

    struct PlayerConstants
    {
        DirectX::XMFLOAT4 color_ = { 0.3f, 0.3f, 0.3f, 1.0f };
    };

    std::unique_ptr<ConstantBuffer<PlayerConstants>> playerConstants_;

    std::unique_ptr<ConstantBuffer<Shader::SceneConstants>> sceneConstants_;
    // -------------------------------------------------------
    
private:// �X���b�h�֌W
    static void LoadingThread(SceneLoading* scene);
    BaseScene*      nextScene_   = nullptr;
    std::thread*    thread_      = nullptr;
};

