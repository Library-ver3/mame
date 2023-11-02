#include "SceneTitleState.h"

#include "../Input/Input.h"
#include "../Other/Easing.h"

// PressAnyButtonState
namespace SceneTitleState
{
    // ������
    void PressAnyButtonState::Initialize()
    {
        // ���݂̃X�e�[�g��ݒ�
        owner->SetCurrentState(SceneTitle::STATE::PressAnyButton);

        // �ϐ�������
        easingTimer_ = 0.0f;
        isBrightnessUp_ = false;
    }

    // �X�V
    void PressAnyButtonState::Update(const float& elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();

        const GamePadButton anyButton =
            GamePad::BTN_UP
            | GamePad::BTN_RIGHT
            | GamePad::BTN_DOWN
            | GamePad::BTN_LEFT
            | GamePad::BTN_A
            | GamePad::BTN_B
            | GamePad::BTN_X
            | GamePad::BTN_Y
            | GamePad::BTN_LEFT_SHOULDER
            | GamePad::BTN_RIGHT_SHOULDER
            | GamePad::BTN_LEFT_THUMB
            | GamePad::BTN_RIGHT_THUMB
            | GamePad::BTN_LEFT_TRIGGER
            | GamePad::BTN_RIGHT_TRIGGER;

        const unsigned int anyKey =
            GetAsyncKeyState('W')
            | GetAsyncKeyState('A')
            | GetAsyncKeyState('S')
            | GetAsyncKeyState('D');

        // �{�^���������ꂽ��
        if (((gamePad.GetButtonDown() & anyButton) || anyKey))
        {   // ���邳���グ�āA���̃X�e�[�g��
            owner->pressAnyButtonSprite_->GetEmissive()->SetEmissiveIntensity(1.0f);
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneTitle::STATE::PressAnyButtonFadeOut));
            return;
        }

        float maxTime = 0.7f;
        float maxBrightness = 0.4f;
        float minBrightness = -0.15f;

        // �������Ɠ_�ł�����
        if (isBrightnessUp_)
        {   // ���邳���グ��
            if (easingTimer_ <= maxTime)
            {
                float brightness = Easing::InSine(easingTimer_, maxTime, maxBrightness, minBrightness);

                owner->pressAnyButtonSprite_->GetEmissive()->SetEmissiveIntensity(brightness);

                easingTimer_ += elapsedTime;
            }
            else
            {   // �t���O��ύX���āA�^�C�}�[���Z�b�g
                isBrightnessUp_ = !isBrightnessUp_;
                easingTimer_ = 0.0f;
            }
        }
        else
        {   // ���邳��������
            if (easingTimer_ <= maxTime)
            {
                float brightness = Easing::InSine(easingTimer_, maxTime, minBrightness, maxBrightness);

                owner->pressAnyButtonSprite_->GetEmissive()->SetEmissiveIntensity(brightness);

                easingTimer_ += elapsedTime;
            }
            else
            {   // �t���O��ύX���āA�^�C�}�[���Z�b�g
                isBrightnessUp_ = !isBrightnessUp_;
                easingTimer_ = 0.0f;
            }
        }
    }

    // �I����
    void PressAnyButtonState::Finalize()
    {
    }
}

// PressAnyButtonFadeOutState
namespace SceneTitleState
{
    // ������
    void PressAnyButtonFadeOutState::Initialize()
    {
        // ���݂̃X�e�[�g��ݒ�
        owner->SetCurrentState(SceneTitle::STATE::PressAnyButtonFadeOut);

        // �ϐ�������
        easingTimer_ = 0.0f;
    }

    // �X�V
    void PressAnyButtonFadeOutState::Update(const float& elapsedTime)
    {   
        float maxTime = 1.0f;
        if (easingTimer_ <= maxTime)
        {   // ���X�ɓ����ɂ��Ă���
            float alpha = Easing::InSine(easingTimer_, maxTime, 0.0f, 1.0f);

            owner->pressAnyButtonSprite_->GetSpriteTransform()->SetColorA(alpha);

            easingTimer_ += elapsedTime;
        }
        else
        {   // �����ɂȂ���
            // ���S��alpha��0�ɂ���
            owner->pressAnyButtonSprite_->GetSpriteTransform()->SetColorA(0.0f);

            // select�ŏo�Ă���sprite��alpha��0�ɂ���
            owner->LoadGameSprite_->GetSpriteTransform()->SetColorA(0.0f);
            owner->QuitGameSprite_->GetSpriteTransform()->SetColorA(0.0f);

            // select��LoadGame���ŏ��ɑI������Ă�悤�ɂ���
            owner->LoadGameSprite_->GetEmissive()->SetEmissiveIntensity(0.6f);
            owner->QuitGameSprite_->GetEmissive()->SetEmissiveIntensity(-0.15f);

            //���̃X�e�[�g��
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneTitle::STATE::SelectFadeIn));

            return;
        }
    }

    // �I����
    void PressAnyButtonFadeOutState::Finalize()
    {
    }
}

// SelectFadeInState
namespace SceneTitleState
{
    // ������
    void SelectFadeInState::Initialize()
    {
        // ���݂̃X�e�[�g��ݒ�
        owner->SetCurrentState(SceneTitle::STATE::SelectFadeIn);
    }

    // �X�V
    void SelectFadeInState::Update(const float& elapsedTime)
    {
        float maxTime = 1.0f;
        if (easingTimer_ <= maxTime)
        {   // ���X�ɕ������o�ė���
            float alpha = Easing::InSine(easingTimer_, maxTime, 1.0f, 0.0f);

            owner->LoadGameSprite_->GetSpriteTransform()->SetColorA(alpha);
            owner->QuitGameSprite_->GetSpriteTransform()->SetColorA(alpha);

            easingTimer_ += elapsedTime;
        }
        else
        {   // alpha�l��1�𒴂�����
            // alpha�l���P�ɂ��Ă���
            owner->LoadGameSprite_->GetSpriteTransform()->SetColorA(1.0f);
            owner->QuitGameSprite_->GetSpriteTransform()->SetColorA(1.0f);

            // ���̃X�e�[�g��
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneTitle::STATE::Select));

            return;
        }
    }

    // �I����
    void SelectFadeInState::Finalize()
    {
    }
}

// SelectState
namespace SceneTitleState
{
    // ������
    void SelectState::Initialize()
    {
        owner->LoadGameSprite_->GetEmissive()->SetEmissiveIntensity(0.4f);
        owner->QuitGameSprite_->GetEmissive()->SetEmissiveIntensity(-0.15f);
    }

    // �X�V
    void SelectState::Update(const float& elapsedTime)
    {
        float maxTime = 0.7f;
        float maxBrightness = 0.4f;
        float minBrightness = -0.15f;

    }

    // �I����
    void SelectState::Finalize()
    {
    }
}

namespace SceneTitleState
{

}

namespace SceneTitleState
{

}