#pragma once
#include "BaseScene.h"

#include <memory>

#include "../Resource/sprite.h"
#include "../Graphics/Bloom.h"

class SceneTitle : public BaseScene
{
public:
    SceneTitle() {}
    ~SceneTitle()       override {}

    void CreateResource()                   override; // リソース生成
    void Initialize()                       override; // 初期化
    void Finalize()                         override; // 終了化
    void Update(const float& elapsedTime)   override; // 更新処理
    void Render()                           override; // 描画処理

    void DrawDebug()    override;

private:
    std::unique_ptr<Sprite> titleLogoSprite_;
    std::unique_ptr<Sprite> pressAnyButtonSprite_;
    std::unique_ptr<Sprite> LoadGameSprite_;
    std::unique_ptr<Sprite> QuitGameSprite_;

private:// シェーダー関連
#pragma region Shader
    std::unique_ptr<FrameBuffer> frameBuffer_;
    std::unique_ptr<FullscreenQuad> bitBlockTransfer_;

    std::unique_ptr<Bloom> bloom_;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> bloomPS_;

    Microsoft::WRL::ComPtr<ID3D11PixelShader> spriteEmissivePS_;
#pragma endregion// Shader
};

