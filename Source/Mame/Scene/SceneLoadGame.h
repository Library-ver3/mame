#pragma once
#include "BaseScene.h"

#include "../Graphics/shader.h"

#include "../Resource/sprite.h"

#include "../Game/AI/StateMachine.h"

#include <thread>

class SceneLoading;
class SceneLoadGame : public BaseScene
{
public:// �萔
    enum class GameDataBase
    {
        Top,    // ��
        Middle, // �^��
        Bottom, // ��
        Max,
    };

    enum class STATE
    {
        SlideUi,
        Select,
        LoadGameChose,
        SlideUiFadeOut,
        LoadGame,
        LoadTitle,
    };

    DirectX::XMFLOAT2 spritePos[3] =
    {
        { 125, 145 },
        { 125, 300 },
        { 125, 460 },
    };

public:// ��{�I�Ȋ֐�
    SceneLoadGame();
    ~SceneLoadGame()       override {}

    void CreateResource()                   override; // ���\�[�X����
    void Initialize()                       override; // ������
    void Finalize()                         override; // �I����
    void Update(const float& elapsedTime)   override; // �X�V����
    void Render()                           override; // �`�揈��

    void DrawDebug()    override;

    void ChangeScene();

public:// �擾�E�ݒ�
    // �X�e�[�g�}�V��
    StateMachine<State<SceneLoadGame>>* GetStateMachine() const { return stateMachine_.get(); }

    void SetCurrentState(SceneLoadGame::STATE state) { currentState_ = static_cast<UINT>(state); }
    int GetCurrentState() { return currentState_; }
    void SetSelectState(int state) { selectState_ = state; }
    int GetSelectState() { return selectState_; }

public:// �X�e�[�g�}�V���ŐG�邽�߂� public
    std::unique_ptr<Sprite> bookSprite_;
    std::unique_ptr<Sprite> whiteSprite_;
    std::unique_ptr<Sprite> backSprite_;
    std::unique_ptr<Sprite> gameDataBaseSprite_[static_cast<UINT>(GameDataBase::Max)];
    std::unique_ptr<Sprite> gameDataChoseSprite_;
    std::unique_ptr<Sprite> gameDataWordSprite_[static_cast<UINT>(GameDataBase::Max)];

    std::unique_ptr<Sprite> blackBeltSprite_;
    std::unique_ptr<Sprite> loadGameWordSprite_;
    std::unique_ptr<Sprite> choseSprite_;

private:
    std::unique_ptr<ConstantBuffer<Shader::SceneConstants>> sceneConstants_;
    
    float fadeInTimer_ = 0.0f;
    bool isFadeIn_ = false;
    bool isFadeOut_ = false;

    Microsoft::WRL::ComPtr<ID3D11PixelShader> spriteDissolvePS_;

private:
    // �X�e�[�g�}�V��
    std::unique_ptr<StateMachine<State<SceneLoadGame>>> stateMachine_;
    int currentState_ = 0;  // ���݂̃X�e�[�g
    int selectState_ = 0;   // �I����������ێ����邽�߂̕ϐ�

private:// �X���b�h�֌W
    static void LoadingThread(SceneLoadGame* scene);
    BaseScene* loadingScene_    = nullptr;
    std::thread* thread_        = nullptr;
};