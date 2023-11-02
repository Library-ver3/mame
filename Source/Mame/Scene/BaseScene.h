#pragma once
#include <string>
#include <vector>

class BaseScene
{
public:
    BaseScene() {}
    virtual ~BaseScene() {}

    virtual void CreateResource()                   = 0; // ���\�[�X����
    virtual void Initialize()                       = 0; // ������
    virtual void Finalize()                         = 0; // �I����
    virtual void Update(const float& elapsedTime)   = 0; // �X�V����
    virtual void Render()                           = 0; // �`�揈��
    virtual void DrawDebug()                        = 0; // ImGui�p

    virtual void RenderInitialize();

public:
    // �����������Ă��邩�ǂ���
    bool IsReady()const { return ready; }

    // ��������
    void SetReady() { ready = true; }

private:
    bool ready = false;
};

