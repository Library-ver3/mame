#pragma once

#include "BaseScene.h"

#include <thread>
#include <memory>

#include "../Resource/sprite.h"

class SceneLoadingBack : public BaseScene
{
public:
    SceneLoadingBack(BaseScene* nextScene);
    ~SceneLoadingBack() override {}

    void CreateResource()                   override; // リソース生成
    void Initialize()                       override; // 初期化
    void Finalize()                         override; // 終了化
    void Update(const float& elapsedTime)   override; // 更新処理
    void Render()                           override; // 描画処理

    void DrawDebug()    override;

private:
    static void LoadingThread(SceneLoadingBack* scene);

private:
    BaseScene* nextScene_    = nullptr;
    std::thread* thread_     = nullptr;
};

