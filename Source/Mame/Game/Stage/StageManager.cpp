#include "StageManager.h"

#include "../../Other/Common.h"

// 初期化
void StageManager::Initialize()
{
}

// 終了化
void StageManager::Finalize()
{
}

// 更新
void StageManager::Update(const float& elapsedTime)
{
}

// 描画
void StageManager::Render()
{
    for (Stage*& stage : stages_)
    {
        // 引数は仮で入れてる。
        stage->Render(1.0f, nullptr);
    }
}

// ImGui用
void StageManager::DrawDebug()
{
    if (ImGui::BeginMenu("Stage"))
    {
        for (Stage* stage : stages_)
        {
            stage->DrawDebug();
        }

        ImGui::EndMenu();
    }
}

// 登録
void StageManager::Register(Stage* stage)
{
    stages_.emplace_back(stage);
}

// 全削除
void StageManager::Clear()
{
    for (Stage*& stage : stages_)
    {
        SafeDeletePtr(stage);
    }
    stages_.clear();
    stages_.shrink_to_fit();
}
