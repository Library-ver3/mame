#pragma once
#include "BaseScene.h"

class SceneDummy : public BaseScene
{
public:
    SceneDummy() {};
    ~SceneDummy()       override {}

    void CreateResource()                   override {}; // リソース生成
    void Initialize()                       override {}; // 初期化
    void Finalize()                         override {}; // 終了化
    void Update(const float& elapsedTime)   override {}; // 更新処理
    void Render()                           override {}; // 描画処理

    void DrawDebug()                        override {}; // ImGui用
};

