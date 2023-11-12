#include "SceneLoading.h"
#include "SceneManager.h"

#include "../Graphics/Graphics.h"

#include "../Other/Easing.h"
#include "../Other/misc.h"

#include "../Core/framework.h"

// �R���X�g���N�^
SceneLoading::SceneLoading(BaseScene* nextScene) :nextScene_(nextScene)
{
}

// ���\�[�X����
void SceneLoading::CreateResource()
{
    Graphics& graphics = Graphics::Instance();

    loadingPlayer_ = std::make_unique<LoadingPlayer>();

    playerConstants_ = std::make_unique<ConstantBuffer<PlayerConstants>>(graphics.GetDevice());

    sceneConstants_ = std::make_unique<ConstantBuffer<Shader::SceneConstants>>(graphics.GetDevice());
}

// ������
void SceneLoading::Initialize()
{
    // --- �J���� ( �Œ�J���� ) ---
    Camera::Instance().FixedInitialize();
    
    // --- model������ ---
    loadingPlayer_->Initialize();

    // --- �ϐ������� ---
    easingTimer_ = 0.0f;

    playerConstants_->data.color_ = { 0.3f, 0.3f, 0.3f, 1.0f };

    // �X���b�h�J�n
    // std::thread(LoadingThread, this);
    // ��ڂ̈�����LoadingThread�̈����ɂȂ�
    thread_ = new std::thread(LoadingThread, this);
}

// �I����
void SceneLoading::Finalize()
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
void SceneLoading::Update(const float& elapsedTime)
{
    loadingPlayer_->Update(elapsedTime);

    // ���̃V�[���������ł�����
    if (nextScene_->IsReady())
    {
        // �t�F�[�h�A�E�g����
        if (FadeOutUi(elapsedTime))
        {   // �t�F�[�h�A�E�g�I�������
            // �V�[���؂�ւ�
            SceneManager::Instance().ChangeScene(nextScene_);
            return;
        }
    }
}

// �`�揈��
void SceneLoading::Render()
{
    // �`��̏����ݒ聦�K���ĂԂ��ƁI�I�I
    BaseScene::RenderInitialize();

    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    Camera& camera = Camera::Instance();

    camera.FixedSetPerSpectiveFov();
    DirectX::XMStoreFloat4x4(&sceneConstants_->data.viewProjection, camera.GetViewMatrix() * camera.GetProjectionMatrix());
    sceneConstants_->data.lightDirection = shader->GetViewPosition();
    sceneConstants_->data.cameraPosition = shader->GetViewCamera();

    DirectX::XMStoreFloat4x4(&sceneConstants_->data.inverseProjection, DirectX::XMMatrixInverse(NULL, camera.GetProjectionMatrix()));
    DirectX::XMStoreFloat4x4(&sceneConstants_->data.inverseViewProjection, DirectX::XMMatrixInverse(NULL, camera.GetViewMatrix() * camera.GetProjectionMatrix()));
    sceneConstants_->data.time = framework::tictoc.time_stamp();

    sceneConstants_->Activate(graphics.GetDeviceContext(), 1);

    // ���f���`��
    {
        playerConstants_->Activate(graphics.GetDeviceContext(), 5, false, true);
        loadingPlayer_->Render(0.01f);
    }
}


void SceneLoading::DrawDebug()
{
#ifdef USE_IMGUI
    Graphics::Instance().GetShader()->DrawDebug();

    loadingPlayer_->DrawDebug();

#endif// USE_IMGUI
}

// UI�̃t�F�C�h�A�E�g
bool SceneLoading::FadeOutUi(const float& elapsedTime)
{
    float maxTime = 1.0f;
    if (easingTimer_ <= maxTime)
    {
        float playerColor = Easing::InSine(easingTimer_, maxTime, 0.0f, 0.3f);

        playerConstants_->data.color_ = { playerColor, playerColor, playerColor, 1.0f };

        easingTimer_ += elapsedTime;
        return false; // �t�F�[�h�A�E�g��
    }
    else
    {
        playerConstants_->data.color_ = { 0.0f, 0.0f, 0.0f, 1.0f };
        return true; // �t�F�[�h�A�E�g�I�����
    }
}

// ���[�f�B���O�X���b�h
void SceneLoading::LoadingThread(SceneLoading* scene)
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