#pragma once
#include "BaseScene.h"

class SceneDummy : public BaseScene
{
public:
    SceneDummy() {};
    ~SceneDummy()       override {}

    void CreateResource()                   override {}; // ���\�[�X����
    void Initialize()                       override {}; // ������
    void Finalize()                         override {}; // �I����
    void Update(const float& elapsedTime)   override {}; // �X�V����
    void Render()                           override {}; // �`�揈��

    void DrawDebug()                        override {}; // ImGui�p
};

