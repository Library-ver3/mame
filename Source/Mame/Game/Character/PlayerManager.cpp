#include "PlayerManager.h"

// 初期化
void PlayerManager::Initialize()
{
    player->Initialize();
}

// 終了化
void PlayerManager::Finalize()
{
    player->Finalize();
}

// 更新
void PlayerManager::Update(const float& elapsedTime)
{
    player->Update(elapsedTime);
}

// 描画
void PlayerManager::Render()
{
    // 描画 ※ここでスケール決定する
    player->Render(0.1f, nullptr);
}

// ImGui用
void PlayerManager::DrawDebug()
{
    player->DrawDebug();
}
