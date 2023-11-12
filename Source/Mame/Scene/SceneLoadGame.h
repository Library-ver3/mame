#pragma once
#include "BaseScene.h"

#include "../Graphics/shader.h"

#include "../Resource/sprite.h"

#include "../Game/AI/StateMachine.h"

#include <thread>

class SceneLoading;
class SceneLoadGame : public BaseScene
{
public:// 定数
    enum class GameDataBase
    {
        Top,    // 上
        Middle, // 真ん中
        Bottom, // 下
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

public:// 基本的な関数
    SceneLoadGame();
    ~SceneLoadGame()       override {}

    void CreateResource()                   override; // リソース生成
    void Initialize()                       override; // 初期化
    void Finalize()                         override; // 終了化
    void Update(const float& elapsedTime)   override; // 更新処理
    void Render()                           override; // 描画処理

    void DrawDebug()    override;

    void ChangeScene();

public:// 取得・設定
    // ステートマシン
    StateMachine<State<SceneLoadGame>>* GetStateMachine() const { return stateMachine_.get(); }

    void SetCurrentState(SceneLoadGame::STATE state) { currentState_ = static_cast<UINT>(state); }
    int GetCurrentState() { return currentState_; }
    void SetSelectState(int state) { selectState_ = state; }
    int GetSelectState() { return selectState_; }

public:// ステートマシンで触るために public
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
    // ステートマシン
    std::unique_ptr<StateMachine<State<SceneLoadGame>>> stateMachine_;
    int currentState_ = 0;  // 現在のステート
    int selectState_ = 0;   // 選択した物を保持するための変数

private:// スレッド関係
    static void LoadingThread(SceneLoadGame* scene);
    BaseScene* loadingScene_    = nullptr;
    std::thread* thread_        = nullptr;
};