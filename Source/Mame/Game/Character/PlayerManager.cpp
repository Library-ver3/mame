#include "PlayerManager.h"

// ������
void PlayerManager::Initialize()
{
    player->Initialize();
}

// �I����
void PlayerManager::Finalize()
{
    player->Finalize();
}

// �X�V
void PlayerManager::Update(const float& elapsedTime)
{
    player->Update(elapsedTime);
}

// �`��
void PlayerManager::Render()
{
    // �`�� �������ŃX�P�[�����肷��
    player->Render(0.1f, nullptr);
}

// ImGui�p
void PlayerManager::DrawDebug()
{
    player->DrawDebug();
}
