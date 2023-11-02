#pragma once
#include "../GameObject.h"

// ステージ
class Stage : public GameObject
{
public:
    Stage(const char* fbxFileName, bool triangulate = true, float samplingRate = 0);
    Stage(const char* fbxFileName, std::vector<std::string>& animationFileName, bool triangulate = true, float samplingRate = 0);
    virtual ~Stage() {}

    void Initialize()                       override {} // 初期化
    void Finalize()                         override {} // 終了化
    void Update(const float& elapsedTime)   override {} // 更新
    void Render(const float& scale,
        ID3D11PixelShader* psShader)        override;   // 描画
    void DrawDebug()                        override;   // ImGui用

protected:// ----- ImGui用 ----- //
    const int GetNameNum() const { return nameNum_++; }

private:// ----- ImGui用 ----- //
    static int nameNum_;    // 名前かぶらないための数字
};

