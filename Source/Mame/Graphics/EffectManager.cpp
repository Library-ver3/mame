#include "../Graphics/Graphics.h"
#include "EffectManager.h"

// ������
void EffectManager::Initialize()
{
    Graphics& graphics = Graphics::Instance();

    // Effekseer�����_������
    effekseerRenderer = EffekseerRendererDX11::Renderer::Create(graphics.GetDevice(),
        graphics.GetDeviceContext(), 2048);

    // Effekseer�}�l�[�W���[����
    effekseerManager = Effekseer::Manager::Create(2048);

    // Effekseer�����_���̊e��ݒ�(���ʂȃJ�X�^�}�C�Y�����Ȃ��ꍇ�͒�^�I�Ɉȉ��̐ݒ��OK)
    effekseerManager->SetSpriteRenderer(effekseerRenderer->CreateSpriteRenderer());
    effekseerManager->SetRibbonRenderer(effekseerRenderer->CreateRibbonRenderer());
    effekseerManager->SetRingRenderer(effekseerRenderer->CreateRingRenderer());
    effekseerManager->SetTrackRenderer(effekseerRenderer->CreateTrackRenderer());
    effekseerManager->SetModelRenderer(effekseerRenderer->CreateModelRenderer());
    // Effekseer���ł̃��[�_�[�̐ݒ�(���ʂȃJ�X�^�}�C�Y�����Ȃ��ꍇ�͈ȉ��̐ݒ��OK)
    effekseerManager->SetTextureLoader(effekseerRenderer->CreateTextureLoader());
    effekseerManager->SetModelLoader(effekseerRenderer->CreateModelLoader());
    effekseerManager->SetMaterialLoader(effekseerRenderer->CreateMaterialLoader());

    // Effekseer�����舟���W�n�Ōv�Z����
    effekseerManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
}

// �I����
void EffectManager::Finalize()
{
    // EffekseerManager�Ȃǂ̓X�}�[�g�|�C���^�ɂ���Ĕj�������̂ŉ������Ȃ�
}

// �X�V����
void EffectManager::Update(float elapsedTime)
{
    // �G�t�F�N�g�X�V����(�����ɂ̓t���[���̒ʉߎ��Ԃ�n��)
    effekseerManager->Update(elapsedTime * 60.0f);
}

// �`�揈��
void EffectManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    // �r���[&�v���W�F�N�V�����s���Effekseer�����_���ɐݒ�
    effekseerRenderer->SetCameraMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&view));
    effekseerRenderer->SetProjectionMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&projection));

    // Effekseer�`��J�n
    effekseerRenderer->BeginRendering();

    // Effekseer�`����s
    // �}�l�[�W���[�P�ʂŕ`�悷��̂ŕ`�揇�𐧌䂷��ꍇ�̓}�l�[�W���[�𕡐����삵�A
    // Draw()�֐������s���鏇���Ő���ł�����
    effekseerManager->Draw();

    // Effekseer�`��I��
    effekseerRenderer->EndRendering();
}