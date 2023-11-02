#include "StageManager.h"

#include "../../Other/Common.h"

// ������
void StageManager::Initialize()
{
}

// �I����
void StageManager::Finalize()
{
}

// �X�V
void StageManager::Update(const float& elapsedTime)
{
}

// �`��
void StageManager::Render()
{
    for (Stage*& stage : stages_)
    {
        // �����͉��œ���Ă�B
        stage->Render(1.0f, nullptr);
    }
}

// ImGui�p
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

// �o�^
void StageManager::Register(Stage* stage)
{
    stages_.emplace_back(stage);
}

// �S�폜
void StageManager::Clear()
{
    for (Stage*& stage : stages_)
    {
        SafeDeletePtr(stage);
    }
    stages_.clear();
    stages_.shrink_to_fit();
}
