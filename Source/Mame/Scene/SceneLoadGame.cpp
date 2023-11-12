#include "SceneLoadGame.h"
#include "SceneManager.h"
#include "SceneTitle.h"

#include "../Core/framework.h"
#include "../Graphics/Graphics.h"

#include "SceneLoadGameState.h"
#include "../Other/Easing.h"

#include "../Resource/NoiseTexture.h"

#include "SceneLoading.h"
#include "SceneGame.h"

SceneLoadGame::SceneLoadGame()
    : loadingScene_(new SceneLoading(new SceneGame))
{
}

// ���\�[�X����
void SceneLoadGame::CreateResource()
{
    Graphics& graphics = Graphics::Instance();

    // Sprite
    {
        bookSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/UI/book.png");

        whiteSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/UI/white.png");

        backSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/LoadGame/back.png");

        for (int i = 0; i < static_cast<UINT>(GameDataBase::Max); ++i)
        {
            gameDataBaseSprite_[i] = std::make_unique<Sprite>(graphics.GetDevice(),
                L"./Resources/Image/LoadGame/base.png");
        }

        gameDataChoseSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/LoadGame/chose.png");

        gameDataWordSprite_[0] = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/LoadGame/gameData1.png");
        gameDataWordSprite_[1] = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/LoadGame/gameData2.png");
        gameDataWordSprite_[2] = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/LoadGame/gameData2.png");

        blackBeltSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/UI/blackBelt.png");
        loadGameWordSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/LoadGame/loadGameWord.png");
        choseSprite_ = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/UI/chose.png");
    }

    // �X�e�[�g�}�V��
    {
        stateMachine_.reset(new StateMachine<State<SceneLoadGame>>);

        GetStateMachine()->RegisterState(new SceneLoadGameState::SlideUiState(this));
        GetStateMachine()->RegisterState(new SceneLoadGameState::SelectState(this));
        GetStateMachine()->RegisterState(new SceneLoadGameState::LoadGameChoseState(this));
        GetStateMachine()->RegisterState(new SceneLoadGameState::SlideUiFadeOutState(this));
        GetStateMachine()->RegisterState(new SceneLoadGameState::LoadGameState(this));
        GetStateMachine()->RegisterState(new SceneLoadGameState::LoadTitleState(this));

        GetStateMachine()->SetState(static_cast<UINT>(STATE::SlideUi));
    }

    CreatePsFromCso(graphics.GetDevice(), "./Resources/Shader/SpriteDissolvePS.cso",
        spriteDissolvePS_.GetAddressOf());

    // SceneConstant
    sceneConstants_ = std::make_unique<ConstantBuffer<Shader::SceneConstants>>(graphics.GetDevice());
}

// ������
void SceneLoadGame::Initialize()
{
    Camera::Instance().FixedInitialize();

    // Sprite������
    {
        bookSprite_->GetSpriteTransform()->SetPos(0, -720);

        whiteSprite_->GetSpriteTransform()->SetColor(0, 0, 0, 1);

        backSprite_->GetSpriteTransform()->SetSize(1280, 720);

        for (int i = 0; i < static_cast<UINT>(GameDataBase::Max); ++i)
        {
            gameDataBaseSprite_[i]->GetSpriteTransform()->SetPos(spritePos[i]);
            gameDataWordSprite_[i]->GetSpriteTransform()->SetPos(spritePos[i]);
        }

        gameDataBaseSprite_[2]->GetSpriteTransform()->SetColor(0.4f, 0.4f, 0.4f);
        gameDataWordSprite_[2]->GetSpriteTransform()->SetColorA(0.0f);

        gameDataChoseSprite_->GetSpriteTransform()->SetPos(spritePos[0]);

#pragma region choseSprite_
        choseSprite_->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(450, 375));
        choseSprite_->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(100, 100));
        choseSprite_->GetSpriteTransform()->SetTexSize(DirectX::XMFLOAT2(100, 100));
#pragma endregion// choseSprite_
    }

    // �ϐ�������
    fadeInTimer_ = 0.0f;
    isFadeIn_ = true;
    isFadeOut_ = false;

    // �X���b�h
    thread_ = new std::thread(LoadingThread, this);
}

// �I����
void SceneLoadGame::Finalize()
{
    // �X���b�h�I����
    thread_->join();
    if (thread_ != nullptr)
    {
        delete thread_;
        thread_ = nullptr;
    }
}

// �X�V
void SceneLoadGame::Update(const float& elapsedTime)
{
    // �X�e�[�g�}�V���X�V
    GetStateMachine()->Update(elapsedTime);

    if (isFadeIn_ && !isFadeOut_)
    {   // �t�F�[�h�C��
        float maxTime = 0.4f;
        if (fadeInTimer_ <= maxTime)
        {
            float alpha = Easing::InSine(fadeInTimer_, maxTime, 0.0f, 1.0f);
            whiteSprite_->GetSpriteTransform()->SetColorA(alpha);
            fadeInTimer_ += elapsedTime;
        }
        else
        {
            whiteSprite_->GetSpriteTransform()->SetColorA(0.0f);
            isFadeIn_ = false;
        }
    }
    else if (!isFadeIn_ && isFadeOut_)
    {   // �t�F�[�h�A�E�g
        // �t�F�[�h�A�E�g�̓X�e�[�g�}�V���̒��ł�����������ꂢ�ɂȂ����̂ŁA
        // �����ł͏��������Ȃ�
    }

    //SceneManager::Instance().ChangeScene(loadingScene_);
}

// �`��
void SceneLoadGame::Render()
{
    // ----- �`��̏����ݒ� -----
    // ���K���ŏ��ɌĂԁI�I
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

    // Sprite�`�� ( �w�i )
    {
        shader->SetBlendState(Shader::BLEND_STATE::ALPHA);
        shader->SetDepthStencileState(Shader::DEPTH_STATE::ZT_OFF_ZW_OFF);
        shader->SetRasterizerState(Shader::RASTER_STATE::CULL_NONE);

        backSprite_->Render();

        bookSprite_->Render();
    }

    // 3D���f���`��
    {
        shader->SetBlendState(Shader::BLEND_STATE::NONE);
        shader->SetDepthStencileState(Shader::DEPTH_STATE::ZT_ON_ZW_ON);
        shader->SetRasterizerState(Shader::RASTER_STATE::SOLID);

        
    }

    // Sprite�`��
    {
        shader->SetBlendState(Shader::BLEND_STATE::ALPHA);
        shader->SetDepthStencileState(Shader::DEPTH_STATE::ZT_OFF_ZW_OFF);
        shader->SetRasterizerState(Shader::RASTER_STATE::CULL_NONE);

        // �}�X�N�摜�Z�b�g �X���b�g3
        NoiseTexture::Instance().SetConstantBuffers(3);

        // ���`�揇�ɒ���
        // base -> chose -> word
        {
            // base
            for (int i = 0; i < static_cast<UINT>(GameDataBase::Max); ++i)
            {
                gameDataBaseSprite_[i]->Render();
            }
            // chose
            gameDataChoseSprite_->Render(spriteDissolvePS_.Get(), "Dissolve");
            // word
            for (int i = 0; i < static_cast<UINT>(GameDataBase::Max); ++i)
            {
                gameDataWordSprite_[i]->Render();
            }
        }

        // �Q�[�������[�h���܂� �̕������o��
        if (currentState_ == static_cast<UINT>(STATE::LoadGameChose))
        {
            blackBeltSprite_->Render();
            choseSprite_->Render();
            loadGameWordSprite_->Render();
        }
    }

    // ----- �����艺�ɉ��������Ȃ� -----
    whiteSprite_->Render();
    // ----- �����艺�ɉ��������Ȃ� -----
}

// ImGui�p
void SceneLoadGame::DrawDebug()
{
    ImGui::Begin("SceneLoadGame");

    if (ImGui::Button("title"))
    {
        SceneManager::Instance().ChangeScene(new SceneTitle(new SceneLoadGame));
    }

    if (ImGui::Button("reset"))
    {
        GetStateMachine()->ChangeState(static_cast<UINT>(STATE::SlideUi));
    }

    bookSprite_->DrawDebug();

    gameDataChoseSprite_->DrawDebug();    

    backSprite_->DrawDebug();
    for (int i = 0; i < static_cast<UINT>(GameDataBase::Max); ++i)
    {
        gameDataBaseSprite_[i]->DrawDebug();
    }

    ImGui::End();
}

void SceneLoadGame::ChangeScene()
{
    if (loadingScene_->IsReady())
    {
        SceneManager::Instance().ChangeScene(loadingScene_);
    }
}

void SceneLoadGame::LoadingThread(SceneLoadGame* scene)
{
    // COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
    std::ignore = CoInitialize(nullptr); // std::ignore�ŕԂ�l�x������

    scene->loadingScene_->CreateResource();
    scene->loadingScene_->Initialize();
    
    // �X���b�h���I���O��COM�֘A�̏I����
    CoUninitialize();

    // ���̃V�[���̏��������ݒ�
    scene->loadingScene_->SetReady();
}
