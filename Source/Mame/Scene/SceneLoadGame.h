#pragma once
#include "BaseScene.h"

#include "../Graphics/shader.h"

#include "../Resource/sprite.h"
#include "../Game/UI/LoadGameBook.h"


class SceneLoadGame : public BaseScene
{
public:// 基本的な関数
    SceneLoadGame() {}
    ~SceneLoadGame()       override {}

    void CreateResource()                   override; // リソース生成
    void Initialize()                       override; // 初期化
    void Finalize()                         override; // 終了化
    void Update(const float& elapsedTime)   override; // 更新処理
    void Render()                           override; // 描画処理

    void DrawDebug()    override;

private:
    std::unique_ptr<LoadGameBook> loadGameBook_;
    std::unique_ptr<Sprite> backSprite_;
    std::unique_ptr<Sprite> gameDataBaseSprite_[3];

private:
    std::unique_ptr<ConstantBuffer<Shader::SceneConstants>> sceneConstants_;
};