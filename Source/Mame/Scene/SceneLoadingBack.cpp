#include "SceneLoadingBack.h"
#include "SceneManager.h"

#include "../Graphics/Graphics.h"

#include "../Other/Easing.h"

// �R���X�g���N�^
SceneLoadingBack::SceneLoadingBack(BaseScene* nextScene)
    : nextScene_(nextScene)
{
}

// ���\�[�X����
void SceneLoadingBack::CreateResource()
{
}

// ������
void SceneLoadingBack::Initialize()
{
    // �Œ�J����
    Camera::Instance().FixedInitialize();

    // �X���b�h�J�n
    thread_ = new std::thread(LoadingThread, this);
}

// �I����
void SceneLoadingBack::Finalize()
{
    // �X���b�h�I����
    thread_->join();
    if (thread_ != nullptr)
    {
        delete thread_;
        thread_ = nullptr;
    }
}

// �X�V����
void SceneLoadingBack::Update(const float& elapsedTime)
{
    if (nextScene_->IsReady())
    {
        SceneManager::Instance().ChangeScene(nextScene_);
    }
}

// �`��
void SceneLoadingBack::Render()
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    // �`��̏�����
    BaseScene::RenderInitialize();
}

// ImGui�p
void SceneLoadingBack::DrawDebug()
{
    Graphics::Instance().GetShader()->DrawDebug();
}

// �X���b�h
void SceneLoadingBack::LoadingThread(SceneLoadingBack* scene)
{
    // COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
    std::ignore = CoInitialize(nullptr); // std::ignore�ŕԂ�l�x������

    // ���̃V�[���̏��������s��
    scene->nextScene_->CreateResource();
    scene->nextScene_->Initialize();

    // �X���b�h���I���O��COM�֘A�̏I����
    CoUninitialize();

    // ���̃V�[���̏��������ݒ�
    scene->nextScene_->SetReady();
}