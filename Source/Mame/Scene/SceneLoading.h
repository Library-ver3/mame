#pragma once

#include "BaseScene.h"

#include <thread>
#include <memory>

#include "../Resource/sprite.h"

class SceneLoading : public BaseScene
{
public:
    SceneLoading(BaseScene* nextScene);
    ~SceneLoading() override {}

    void CreateResource()                   override; // ���\�[�X����
    void Initialize()                       override; // ������
    void Finalize()                         override; // �I����
    void Update(const float& elapsedTime)   override; // �X�V����
    void Render()                           override; // �`�揈��

    void DrawDebug()    override;

private:
    // ���[�f�B���O�X���b�h
    static void LoadingThread(SceneLoading* scene);

private:
    BaseScene*      nextScene   = nullptr;
    std::thread*    thread      = nullptr;
};

