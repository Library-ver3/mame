#pragma once
#include "BaseScene.h"

#include <d3d11.h>
#include <wrl.h>
#include <memory>

class SceneGame : public BaseScene
{
public: // 基底クラスにある関数
    SceneGame() {}
    ~SceneGame() override {}

    void CreateResource()                   override; // リソース生成
    void Initialize()                       override; // 初期化
    void Finalize()                         override; // 終了化
    void Update(const float& elapsedTime)   override; // 更新処理
    void Render()                           override; // 描画処理
    void DrawDebug()                        override; // ImGui用

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer_ = nullptr;

#ifdef _DEBUG
    bool isDebugCamera = false;
#endif // _DEBUG
};

