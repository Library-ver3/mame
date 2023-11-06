#pragma once
#include "BaseScene.h"

#include <memory>

#include "../Resource/sprite.h"
#include "../Graphics/Bloom.h"

#include "../Game/AI/StateMachine.h"

class SceneTitle : public BaseScene
{
public:// �萔
    enum class STATE
    {
        PressAnyButton,
        PressAnyButtonFadeOut,
        SelectFadeIn,
        Select,
        QuitGameChose,
        LoadGameChose,
    };

public:// ��{�I�Ȋ֐�
    SceneTitle() {}
    ~SceneTitle()       override {}

    void CreateResource()                   override; // ���\�[�X����
    void Initialize()                       override; // ������
    void Finalize()                         override; // �I����
    void Update(const float& elapsedTime)   override; // �X�V����
    void Render()                           override; // �`�揈��

    void DrawDebug()    override;

public:// �擾�E�ݒ�
    // �X�e�[�g�}�V��
    StateMachine<State<SceneTitle>>* GetStateMachine() const { return stateMachine_.get(); }

    void SetCurrentState(SceneTitle::STATE state) { currentState_ = static_cast<UINT>(state); }    
    int GetCurrentState() { return currentState_; }
    void SetSelectState(int state) { selectState_ = state; }
    int GetSelectState() { return selectState_; }

private:
    // �X�e�[�g�}�V��
    std::unique_ptr<StateMachine<State<SceneTitle>>> stateMachine_ = nullptr;
    int currentState_ = 0;  // ���݂̃X�e�[�g
    int selectState_ = 0;   // �I�����Ă�����ێ����邽�߂̕ϐ�
    
public:// �X�e�[�g�}�V���ŐG�邽�߂�public�Œ�`
    std::unique_ptr<Sprite> titleLogoSprite_;
    std::unique_ptr<Sprite> pressAnyButtonSprite_;
    std::unique_ptr<Sprite> loadGameSprite_;
    std::unique_ptr<Sprite> quitGameSprite_;
    std::unique_ptr<Sprite> blackBeltSprite_;
    std::unique_ptr<Sprite> quitGameWordSprite_;
    std::unique_ptr<Sprite> loadGameWordSprite_;
    std::unique_ptr<Sprite> choseSprite_;


private:// �V�F�[�_�[�֘A
#pragma region Shader
    std::unique_ptr<FrameBuffer> frameBuffer_;
    std::unique_ptr<FullscreenQuad> bitBlockTransfer_;

    std::unique_ptr<Bloom> bloom_;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> bloomPS_;

    Microsoft::WRL::ComPtr<ID3D11PixelShader> spriteEmissivePS_;
#pragma endregion// Shader
};

