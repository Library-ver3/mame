#pragma once

#include "BaseScene.h"

#include <thread>
#include <memory>

#include "../Resource/sprite.h"

#include "LoadingPlayer.h"
#include "../Graphics/ConstantBuffer.h"

#include "../Graphics/shader.h"

class SceneLoading : public BaseScene
{
public:// 基本的な関数
    SceneLoading(BaseScene* nextScene);
    ~SceneLoading() override {}

    void CreateResource()                   override; // リソース生成
    void Initialize()                       override; // 初期化
    void Finalize()                         override; // 終了化
    void Update(const float& elapsedTime)   override; // 更新処理
    void Render()                           override; // 描画処理

    void DrawDebug()    override;

private:
    bool FadeOutUi(const float& elapsedTime);

private:
    // ---------- シーン切り替え用 ----------------------------
    float easingTimer_  = 0.0f;
    // -------------------------------------------------------

    // ---------- シェーダー -----------------------------------
    std::unique_ptr<LoadingPlayer> loadingPlayer_ = nullptr;

    struct PlayerConstants
    {
        DirectX::XMFLOAT4 color_ = { 0.3f, 0.3f, 0.3f, 1.0f };
    };

    std::unique_ptr<ConstantBuffer<PlayerConstants>> playerConstants_;

    std::unique_ptr<ConstantBuffer<Shader::SceneConstants>> sceneConstants_;
    // -------------------------------------------------------
    
private:// スレッド関係
    static void LoadingThread(SceneLoading* scene);
    BaseScene*      nextScene_   = nullptr;
    std::thread*    thread_      = nullptr;
};

