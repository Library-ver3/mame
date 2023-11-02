#pragma once
#include "Stage.h"

// ステージマネージャー
class StageManager
{
private:
    StageManager() {};
    ~StageManager() {}

public:
    // インスタンス取得
    static StageManager& Instance()
    {
        static StageManager instance;
        return instance;
    }

    void Initialize();                      // 初期化
    void Finalize();                        // 終了化
    void Update(const float& elapsedTime);  // 更新
    void Render();                          // 描画
    void DrawDebug();                       // ImGui用

public:
    void Register(Stage* stage);    // 登録
    void Clear();                   // 全削除

private:
    std::vector<Stage*> stages_ = {};
};

