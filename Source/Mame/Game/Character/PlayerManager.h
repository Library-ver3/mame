#pragma once
#include "Player.h"

class PlayerManager
{
private:
    PlayerManager() {}
    ~PlayerManager() {}

public:
    // �C���X�^���X�擾
    static PlayerManager& Instance()
    {
        static PlayerManager instance;
        return instance;
    }

    void Initialize();                      // ������
    void Finalize();                        // �I����
    void Update(const float& elapsedTime);  // �X�V
    void Render();                          // �`��
    void DrawDebug();                       // ImGui�p

public:// �擾�E�ݒ�
#pragma region[Get/Set]Function
    std::unique_ptr<Player>& GetPlayer() { return player; }
#pragma endregion// [Get/Set]Function

private:
    std::unique_ptr<Player> player = nullptr;
};

