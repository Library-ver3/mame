#pragma once
#include "BaseScene.h"

#include <memory>

#include "../Resource/sprite.h"
#include "../Graphics/Bloom.h"

#include "../Game/AI/StateMachine.h"

class SceneTitle : public BaseScene
{
public:// 定数
    enum class STATE
    {
        PressAnyButton,
        PressAnyButtonFadeOut,
        SelectFadeIn,
        Select,
        QuitGameChose,
        LoadGameChose,
    };

public:// 基本的な関数
    SceneTitle() {}
    ~SceneTitle()       override {}

    void CreateResource()                   override; // リソース生成
    void Initialize()                       override; // 初期化
    void Finalize()                         override; // 終了化
    void Update(const float& elapsedTime)   override; // 更新処理
    void Render()                           override; // 描画処理

    void DrawDebug()    override;

public:// 取得・設定
    // ステートマシン
    StateMachine<State<SceneTitle>>* GetStateMachine() const { return stateMachine_.get(); }

    void SetCurrentState(SceneTitle::STATE state) { currentState_ = static_cast<UINT>(state); }    
    int GetCurrentState() { return currentState_; }
    void SetSelectState(int state) { selectState_ = state; }
    int GetSelectState() { return selectState_; }

private:
    // ステートマシン
    std::unique_ptr<StateMachine<State<SceneTitle>>> stateMachine_ = nullptr;
    int currentState_ = 0;  // 現在のステート
    int selectState_ = 0;   // 選択してた物を保持するための変数
    
public:// ステートマシンで触るためにpublicで定義
    std::unique_ptr<Sprite> titleLogoSprite_;
    std::unique_ptr<Sprite> pressAnyButtonSprite_;
    std::unique_ptr<Sprite> loadGameSprite_;
    std::unique_ptr<Sprite> quitGameSprite_;
    std::unique_ptr<Sprite> blackBeltSprite_;
    std::unique_ptr<Sprite> quitGameWordSprite_;
    std::unique_ptr<Sprite> loadGameWordSprite_;
    std::unique_ptr<Sprite> choseSprite_;


private:// シェーダー関連
#pragma region Shader
    std::unique_ptr<FrameBuffer> frameBuffer_;
    std::unique_ptr<FullscreenQuad> bitBlockTransfer_;

    std::unique_ptr<Bloom> bloom_;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> bloomPS_;

    Microsoft::WRL::ComPtr<ID3D11PixelShader> spriteEmissivePS_;
#pragma endregion// Shader
};

