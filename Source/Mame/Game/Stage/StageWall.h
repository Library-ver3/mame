#pragma once
#include "Stage.h"

// ステージに影の映り込みなしクラス
class StageWall : public Stage
{
public:
    StageWall(const char* fbxFileName);
    ~StageWall() {}

    void Initialize()                                               override;   // 初期化
    void Finalize()                                                 override;   // 終了化
    void Update(const float& elapsedTime)                           override;   // 更新
    void Render(const float& scale, ID3D11PixelShader* psShader)    override;   // 描画
    void DrawDebug()                                                override;   // ImGui用

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> psShader_ = nullptr;
};

