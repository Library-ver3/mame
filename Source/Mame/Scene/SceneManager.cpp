#include "SceneManager.h"


#include "../Graphics/Graphics.h"

#include "../Input/Input.h"

#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneLoading.h"

// ������
void SceneManager::Initialize()
{
}

// �X�V����
void SceneManager::Update(float elapsedTime)
{
    if (nextScene)
    {
        // �Â��V�[�����I��
        Clear();

        // �V�����V�[����ݒ�
        currentScene = nextScene;
        nextScene = nullptr;

        // �V�[������������(�}���`�X���b�h���������Ă��Ȃ��ꍇ�ɍs��)
        if (!currentScene->IsReady())
        {
            currentScene->CreateResource();
            currentScene->Initialize();
        }
    }

    if (currentScene)
    {
        currentScene->Update(elapsedTime);
#ifdef USE_IMGUI
        currentScene->DrawDebug();
        DrawDebug();
#endif
    }
}

// �`�揈��
void SceneManager::Render()
{
    if (!currentScene)return;

    currentScene->Render();
}

// �f�o�b�O�p
void SceneManager::DrawDebug()
{
}

// �V�[���N���A
void SceneManager::Clear()
{
    if (!currentScene)return;

    currentScene->Finalize();
    delete currentScene;
    currentScene = nullptr;
    //SafeDelete(currentScene);
}

// �V�[���؂�ւ�
void SceneManager::ChangeScene(BaseScene* scene)
{
    // �V�����V�[����ݒ�
    nextScene = scene;
}