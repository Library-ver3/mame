#include "SceneLoading.h"
#include "SceneManager.h"

#include "../Graphics/Graphics.h"

#include "../Other/Easing.h"
#include "../Other/misc.h"

// �R���X�g���N�^
SceneLoading::SceneLoading(BaseScene* nextScene) :nextScene(nextScene) 
{
}

// ���\�[�X����
void SceneLoading::CreateResource()
{
}

// ������
void SceneLoading::Initialize()
{
    // �Œ�J����
    Camera::Instance().FixedInitialize();


    // �X���b�h�J�n
    // std::thread(LoadingThread, this);
    // ��ڂ̈�����LoadingThread�̈����ɂȂ�
    thread = new std::thread(LoadingThread, this);
}

// �I����
void SceneLoading::Finalize()
{
    // �X���b�h�I����
    thread->join();
    if (thread != nullptr)
    {
        delete thread;
        thread = nullptr;
    }
}


// �X�V����
void SceneLoading::Update(const float& elapsedTime)
{
    // �V�[���؂�ւ�
    if (nextScene->IsReady())
    {
        Mame::Scene::SceneManager::Instance().ChangeScene(nextScene);
    }
}

// �`�揈��
void SceneLoading::Render()
{
    // �`��̏�����
    {
        Graphics& graphics = Graphics::Instance();
        Shader* shader = graphics.GetShader();

        BaseScene::RenderInitialize();
    }
}


//// �`�揈��
//void SceneLoading::Render(const float& elapsedTime)
//{
//    // �`��̏�����
//    {
//        Graphics& graphics = Graphics::Instance();
//        Shader* shader = graphics.GetShader();
//
//        Mame::Scene::BaseScene::RenderInitialize();
//
//        Camera& camera = Camera::Instance();
//        //Camera::Instance().TitleSetPerspectiveFov(graphics.GetDeviceContext());
//        Camera::Instance().ResultSetPerSpectiveFov(graphics.GetDeviceContext());
//
//        Shader::SceneConstants sceneConstants{};
//        DirectX::XMStoreFloat4x4(&sceneConstants.viewProjection, camera.GetViewMatrix() * camera.GetProjectionMatrix());
//        sceneConstants.lightDirection = shader->GetViewPosition();
//        sceneConstants.cameraPosition = shader->GetViewCamera();
//
//        DirectX::XMStoreFloat4x4(&sceneConstants.inverseProjection, DirectX::XMMatrixInverse(NULL, camera.GetProjectionMatrix()));
//        DirectX::XMStoreFloat4x4(&sceneConstants.inverseViewProjection, DirectX::XMMatrixInverse(NULL, camera.GetViewMatrix() * camera.GetProjectionMatrix()));
//        sceneConstants.time = framework::tictoc.time_stamp();
//
//
//    }
//}

void SceneLoading::DrawDebug()
{
#ifdef USE_IMGUI
    Graphics::Instance().GetShader()->DrawDebug();
#endif// USE_IMGUI
}

// ���[�f�B���O�X���b�h
void SceneLoading::LoadingThread(SceneLoading* scene)
{
    // COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
    std::ignore = CoInitialize(nullptr); // std::ignore�ŕԂ�l�x������

    // ���̃V�[���̏��������s��
    scene->nextScene->CreateResource();
    scene->nextScene->Initialize();

    // �X���b�h���I���O��COM�֘A�̏I����
    CoUninitialize();

    // ���̃V�[���̏��������ݒ�
    scene->nextScene->SetReady();
}