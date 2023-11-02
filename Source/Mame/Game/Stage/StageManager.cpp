#include "StageManager.h"

#include "../../Other/Common.h"

// ‰Šú‰»
void StageManager::Initialize()
{
}

// I—¹‰»
void StageManager::Finalize()
{
}

// XV
void StageManager::Update(const float& elapsedTime)
{
}

// •`‰æ
void StageManager::Render()
{
    for (Stage*& stage : stages_)
    {
        // ˆø”‚Í‰¼‚Å“ü‚ê‚Ä‚éB
        stage->Render(1.0f, nullptr);
    }
}

// ImGui—p
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

// “o˜^
void StageManager::Register(Stage* stage)
{
    stages_.emplace_back(stage);
}

// ‘Síœ
void StageManager::Clear()
{
    for (Stage*& stage : stages_)
    {
        SafeDeletePtr(stage);
    }
    stages_.clear();
    stages_.shrink_to_fit();
}
