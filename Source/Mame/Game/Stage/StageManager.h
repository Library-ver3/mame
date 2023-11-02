#pragma once
#include "Stage.h"

// �X�e�[�W�}�l�[�W���[
class StageManager
{
private:
    StageManager() {};
    ~StageManager() {}

public:
    // �C���X�^���X�擾
    static StageManager& Instance()
    {
        static StageManager instance;
        return instance;
    }

    void Initialize();                      // ������
    void Finalize();                        // �I����
    void Update(const float& elapsedTime);  // �X�V
    void Render();                          // �`��
    void DrawDebug();                       // ImGui�p

public:
    void Register(Stage* stage);    // �o�^
    void Clear();                   // �S�폜

private:
    std::vector<Stage*> stages_ = {};
};

