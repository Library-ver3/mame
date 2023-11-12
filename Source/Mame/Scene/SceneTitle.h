#pragma once
#include "BaseScene.h"

#include <thread>
#include <memory>

#include "../Resource/sprite.h"
#include "../Graphics/Bloom.h"

#include "../Game/AI/StateMachine.h"

class SceneTitle : public BaseScene
{
public:// 定数
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

public:// 基本的な関数
    SceneTitle(BaseScene* nextScene);
    ~SceneTitle()       override {}

    void CreateResource()                   override; // リソース生成
    void Initialize()                       override; // 初期化
    void Finalize()                         override; // 終了化
    void Update(const float& elapsedTime)   override; // 更新処理
    void Render()                           override; // 描画処理

    void DrawDebug()                        override; // ImGui用

public:// 取得・設定
#pragma region [Get,Set]Function
    // ----- ステートマシン -----
    StateMachine<State<SceneTitle>>* GetStateMachine() const { return stateMachine_.get(); }
    
    // --- 現在のステート ---
    int GetCurrentState() { return currentState_; }
    void SetCurrentState(SceneTitle::STATE state) { currentState_ = static_cast<UINT>(state); }    
    
    // --- 選択していた値を保存しておく ---
    int GetSelectState() { return selectState_; }
    void SetSelectState(int state) { selectState_ = state; }

    // ----- シーン切り替え用 -----
    bool GetIsReady() { return nextScene_->IsReady(); }
    void ChangeScene();
#pragma endregion// [Get,Set]Function
private:

    
public:// ステートマシンで触るためにpublicで定義
    std::unique_ptr<Sprite> titleLogoSprite_;       // Emma
    std::unique_ptr<Sprite> pressAnyButtonSprite_;  // PressAnyButton
    std::unique_ptr<Sprite> loadGameSprite_;        // LoadGame
    std::unique_ptr<Sprite> quitGameSprite_;        // QuitGame
    std::unique_ptr<Sprite> blackBeltSprite_;       // 文字の下に表示する
    std::unique_ptr<Sprite> quitGameWordSprite_;    // ゲームを終了しますか？
    std::unique_ptr<Sprite> choseSprite_;           // カーソル (アニメーション)

private:// メンバ変数
    // ---------- ステートマシン ----------------------------------------
    std::unique_ptr<StateMachine<State<SceneTitle>>> stateMachine_ = nullptr;
    int currentState_   = 0; // 現在のステート
    int selectState_    = 0; // 選択してた物を保持するための変数
    // -----------------------------------------------------------------

    // ---------- シェーダー --------------------------------------------
    // --- framebuffer ---
    std::unique_ptr<FrameBuffer>    frameBuffer_;
    std::unique_ptr<FullscreenQuad> bitBlockTransfer_;

    // --- bloom ---
    std::unique_ptr<Bloom>                    bloom_;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> bloomPS_;

    // --- sprite ---
    Microsoft::WRL::ComPtr<ID3D11PixelShader> spriteEmissivePS_;
    // -----------------------------------------------------------------

private:// スレッド関係
    static void LoadingThread(SceneTitle* scene);
    BaseScene* nextScene_   = nullptr;
    std::thread* thread_    = nullptr;
};

