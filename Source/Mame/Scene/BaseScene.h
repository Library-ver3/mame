#pragma once
#include <string>
#include <vector>

class BaseScene
{
public:
    BaseScene() {}
    virtual ~BaseScene() {}

    virtual void CreateResource()                   = 0; // リソース生成
    virtual void Initialize()                       = 0; // 初期化
    virtual void Finalize()                         = 0; // 終了化
    virtual void Update(const float& elapsedTime)   = 0; // 更新処理
    virtual void Render()                           = 0; // 描画処理
    virtual void DrawDebug()                        = 0; // ImGui用

    virtual void RenderInitialize();

public:
    // 準備完了しているかどうか
    bool IsReady()const { return ready; }

    // 準備完了
    void SetReady() { ready = true; }

private:
    bool ready = false;
};

