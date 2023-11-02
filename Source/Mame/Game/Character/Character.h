#pragma once
#include "../GameObject.h"

// キャラクター
class Character : public GameObject
{
public:// 基本的な関数 ( overrideしてる関数など )
    Character(const char* fbxFileName, bool triangulate = true, float samplingRate = 0);
    Character(const char* fbxFileName, std::vector<std::string>& animationFileName, bool triangulate = true, float samplingRate = 0);
    ~Character() override;

    void Initialize()                           override; // 初期化
    void Finalize()                             override; // 終了化
    void Update(const float& elapsedTime)       override; // 更新
    void Render(const float& scale,
        ID3D11PixelShader* psShader = nullptr)  override; // 描画
    void DrawDebug()                            override; // ImGui用

    void UpdateConstants()                      override; // 定数バッファー更新
};

