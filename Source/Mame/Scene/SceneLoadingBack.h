#pragma once

#include "BaseScene.h"

#include <thread>
#include <memory>

#include "../Resource/sprite.h"

class SceneLoadingBack : public BaseScene
{
public:
    SceneLoadingBack(BaseScene* nextScene);
    ~SceneLoadingBack() override {}

    void CreateResource()                   override; // ���\�[�X����
    void Initialize()                       override; // ������
    void Finalize()                         override; // �I����
    void Update(const float& elapsedTime)   override; // �X�V����
    void Render()                           override; // �`�揈��

    void DrawDebug()    override;

private:
    static void LoadingThread(SceneLoadingBack* scene);

private:
    BaseScene* nextScene_    = nullptr;
    std::thread* thread_     = nullptr;
};

