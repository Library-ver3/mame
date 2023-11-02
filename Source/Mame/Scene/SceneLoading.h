#pragma once

#include "BaseScene.h"

#include <thread>
#include <memory>

#include "../Resource/sprite.h"

class SceneLoading : public BaseScene
{
public:
    SceneLoading(BaseScene* nextScene);
    ~SceneLoading() override {}

    void CreateResource()                   override; // リソース生成
    void Initialize()                       override; // 初期化
    void Finalize()                         override; // 終了化
    void Update(const float& elapsedTime)   override; // 更新処理
    void Render()                           override; // 描画処理

    void DrawDebug()    override;

private:
    // ローディングスレッド
    static void LoadingThread(SceneLoading* scene);

private:
    BaseScene*      nextScene   = nullptr;
    std::thread*    thread      = nullptr;
};

