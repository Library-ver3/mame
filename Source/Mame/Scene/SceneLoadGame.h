#pragma once
#include "BaseScene.h"

#include "../Graphics/shader.h"

#include "../Resource/sprite.h"
#include "../Game/UI/LoadGameBook.h"


class SceneLoadGame : public BaseScene
{
public:// ��{�I�Ȋ֐�
    SceneLoadGame() {}
    ~SceneLoadGame()       override {}

    void CreateResource()                   override; // ���\�[�X����
    void Initialize()                       override; // ������
    void Finalize()                         override; // �I����
    void Update(const float& elapsedTime)   override; // �X�V����
    void Render()                           override; // �`�揈��

    void DrawDebug()    override;

private:
    std::unique_ptr<LoadGameBook> loadGameBook_;
    std::unique_ptr<Sprite> backSprite_;
    std::unique_ptr<Sprite> gameDataBaseSprite_[3];

private:
    std::unique_ptr<ConstantBuffer<Shader::SceneConstants>> sceneConstants_;
};