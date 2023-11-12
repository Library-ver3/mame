#pragma once
#include "BaseScene.h"

#include <thread>
#include <memory>

#include "../Resource/sprite.h"
#include "../Graphics/Bloom.h"

#include "../Game/AI/StateMachine.h"

class SceneTitle : public BaseScene
{
public:// �萔
    enum class STATE
    {
        FadeIn,
        PressAnyButton,
        PressAnyButtonFadeOut,
        SelectFadeIn,
        Select,
        QuitGameChose,
        LoadGameFadeOut,
    };

public:// ��{�I�Ȋ֐�
    SceneTitle(BaseScene* nextScene);
    ~SceneTitle()       override {}

    void CreateResource()                   override; // ���\�[�X����
    void Initialize()                       override; // ������
    void Finalize()                         override; // �I����
    void Update(const float& elapsedTime)   override; // �X�V����
    void Render()                           override; // �`�揈��

    void DrawDebug()                        override; // ImGui�p

public:// �擾�E�ݒ�
#pragma region [Get,Set]Function
    // ----- �X�e�[�g�}�V�� -----
    StateMachine<State<SceneTitle>>* GetStateMachine() const { return stateMachine_.get(); }
    
    // --- ���݂̃X�e�[�g ---
    int GetCurrentState() { return currentState_; }
    void SetCurrentState(SceneTitle::STATE state) { currentState_ = static_cast<UINT>(state); }    
    
    // --- �I�����Ă����l��ۑ����Ă��� ---
    int GetSelectState() { return selectState_; }
    void SetSelectState(int state) { selectState_ = state; }

    // ----- �V�[���؂�ւ��p -----
    bool GetIsReady() { return nextScene_->IsReady(); }
    void ChangeScene();
#pragma endregion// [Get,Set]Function
private:

    
public:// �X�e�[�g�}�V���ŐG�邽�߂�public�Œ�`
    std::unique_ptr<Sprite> titleLogoSprite_;       // Emma
    std::unique_ptr<Sprite> pressAnyButtonSprite_;  // PressAnyButton
    std::unique_ptr<Sprite> loadGameSprite_;        // LoadGame
    std::unique_ptr<Sprite> quitGameSprite_;        // QuitGame
    std::unique_ptr<Sprite> blackBeltSprite_;       // �����̉��ɕ\������
    std::unique_ptr<Sprite> quitGameWordSprite_;    // �Q�[�����I�����܂����H
    std::unique_ptr<Sprite> choseSprite_;           // �J�[�\�� (�A�j���[�V����)

private:// �����o�ϐ�
    // ---------- �X�e�[�g�}�V�� ----------------------------------------
    std::unique_ptr<StateMachine<State<SceneTitle>>> stateMachine_ = nullptr;
    int currentState_   = 0; // ���݂̃X�e�[�g
    int selectState_    = 0; // �I�����Ă�����ێ����邽�߂̕ϐ�
    // -----------------------------------------------------------------

    // ---------- �V�F�[�_�[ --------------------------------------------
    // --- framebuffer ---
    std::unique_ptr<FrameBuffer>    frameBuffer_;
    std::unique_ptr<FullscreenQuad> bitBlockTransfer_;

    // --- bloom ---
    std::unique_ptr<Bloom>                    bloom_;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> bloomPS_;

    // --- sprite ---
    Microsoft::WRL::ComPtr<ID3D11PixelShader> spriteEmissivePS_;
    // -----------------------------------------------------------------

private:// �X���b�h�֌W
    static void LoadingThread(SceneTitle* scene);
    BaseScene* nextScene_   = nullptr;
    std::thread* thread_    = nullptr;
};

