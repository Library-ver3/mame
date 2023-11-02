#pragma once
#include "Player.h"

class PlayerManager
{
private:
    PlayerManager() {}
    ~PlayerManager() {}

public:
    // インスタンス取得
    static PlayerManager& Instance()
    {
        static PlayerManager instance;
        return instance;
    }

    void Initialize();                      // 初期化
    void Finalize();                        // 終了化
    void Update(const float& elapsedTime);  // 更新
    void Render();                          // 描画
    void DrawDebug();                       // ImGui用

public:// 取得・設定
#pragma region[Get/Set]Function
    std::unique_ptr<Player>& GetPlayer() { return player; }
#pragma endregion// [Get/Set]Function

private:
    std::unique_ptr<Player> player = nullptr;
};

