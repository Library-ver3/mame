#include "SceneTitleState.h"

#include "../Input/Input.h"
#include "../Other/Easing.h"

#include "../Core/framework.h"

// PressAnyButtonState
namespace SceneTitleState
{
    // ������
    void PressAnyButtonState::Initialize()
    {
        // ���݂̃X�e�[�g��ݒ�
        owner->SetCurrentState(SceneTitle::STATE::PressAnyButton);

        // sprite������
        owner->pressAnyButtonSprite_->GetSpriteTransform()->SetColorA(1.0f);

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

        // LoadGame���I������Ă���悤�ɂ���
        owner->SetSelectState(0);

        // sprite������
        owner->loadGameSprite_->GetSpriteTransform()->SetColorA(0.0f);
        owner->quitGameSprite_->GetSpriteTransform()->SetColorA(0.0f);

        // LoadGame���ŏ��ɑI������Ă�悤�ɂ���
        owner->loadGameSprite_->GetEmissive()->SetEmissiveIntensity(0.6f);
        owner->quitGameSprite_->GetEmissive()->SetEmissiveIntensity(-0.15f);

        // �ϐ�������
        easingTimer_ = 0.0f;
    }

    // �X�V
    void SelectFadeInState::Update(const float& elapsedTime)
    {
        float maxTime = 1.0f;
        if (easingTimer_ <= maxTime)
        {   // ���X�ɕ������o�ė���
            float alpha = Easing::InSine(easingTimer_, maxTime, 1.0f, 0.0f);

            owner->loadGameSprite_->GetSpriteTransform()->SetColorA(alpha);
            owner->quitGameSprite_->GetSpriteTransform()->SetColorA(alpha);

            easingTimer_ += elapsedTime;
        }
        else
        {   // alpha�l��1�𒴂�����
            // alpha�l���P�ɂ��Ă���
            owner->loadGameSprite_->GetSpriteTransform()->SetColorA(1.0f);
            owner->quitGameSprite_->GetSpriteTransform()->SetColorA(1.0f);

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
        // ���݂̃X�e�[�g��ݒ�
        owner->SetCurrentState(SceneTitle::STATE::Select);

        // sprite������
        // LoadGame���ŏ��ɑI������Ă�悤�ɂ���
        owner->loadGameSprite_->GetEmissive()->SetEmissiveIntensity(0.4f);
        owner->quitGameSprite_->GetEmissive()->SetEmissiveIntensity(-0.15f);

        // �ϐ�������
        easingTimer_ = 0.0f;
        currentState_ = owner->GetSelectState();
        isBrightnessUp_ = false;
        isChangeState_ = false;
        breakTimer_ = 0.0f;
    }

    // �X�V
    void SelectState::Update(const float& elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();

        // B�{�^��
        if (gamePad.GetButtonDown() & GamePad::BTN_B)
        {   // PressAnyButton�ɖ߂�
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneTitle::STATE::PressAnyButton));
            return;
        }

        // �X�e�[�g�X�V
        UpdateState(elapsedTime);
    }

    // �I����
    void SelectState::Finalize()
    {
    }

    // �X�e�[�g�X�V
    void SelectState::UpdateState(const float& elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();

        float aLY = gamePad.GetAxisLY();
        float minBrightness = -0.15f;

        float stick = 0.8f;

        switch (currentState_)
        {
        case static_cast<UINT>(STATE::LoadGame):
            if ((aLY > stick) && isChangeState_)
            {   // �X�e�B�b�N����ɌX����
                isChangeState_ = false;
                currentState_ = static_cast<UINT>(STATE::QuitGame);
                owner->SetSelectState(currentState_);
                break;
            }
            if ((aLY < -stick) && isChangeState_)
            {   // �X�e�B�b�N�����ɌX����
                isChangeState_ = false;
                currentState_ = static_cast<UINT>(STATE::QuitGame);
                owner->SetSelectState(currentState_);
                break;
            }


            owner->loadGameSprite_->GetEmissive()->SetEmissiveIntensity(
                UpdateBrightness(elapsedTime, "LoadGame"));
            owner->quitGameSprite_->GetEmissive()->SetEmissiveIntensity(minBrightness);



            break;
        case static_cast<UINT>(STATE::QuitGame):
            if ((aLY > stick) && isChangeState_)
            {   // �X�e�B�b�N����ɌX����
                isChangeState_ = false;
                currentState_ = static_cast<UINT>(STATE::LoadGame);
                owner->SetSelectState(currentState_);
                break;
            }
            if ((aLY < -stick) && isChangeState_)
            {   // �X�e�B�b�N�����ɌX����
                isChangeState_ = false;
                currentState_ = static_cast<UINT>(STATE::LoadGame);
                owner->SetSelectState(currentState_);
                break;
            }

            // �I������
            if (gamePad.GetButtonDown() & GamePad::BTN_A)
            {
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneTitle::STATE::QuitGameChose));
                return;
            }

            owner->loadGameSprite_->GetEmissive()->SetEmissiveIntensity(minBrightness);
            owner->quitGameSprite_->GetEmissive()->SetEmissiveIntensity(
                UpdateBrightness(elapsedTime, "QuitGame"));

            break;
        }

        if (isChangeState_)
        {
            breakTimer_ = 0.0f;
        }
        else
        {
            float maxTime = 0.15f;
            if (breakTimer_ > maxTime)
            {
                breakTimer_ = 0.0f;
                isChangeState_ = true;
            }
            breakTimer_ += elapsedTime;
        }
    }

    // �������Ɠ_�ł�����
    float SelectState::UpdateBrightness(const float& elapsedTime, const char* who)
    {
        float maxTime = 0.7f;
        float maxBrightness = 0.4f;
        float minBrightness = -0.15f;

        if (isBrightnessUp_)
        {   // ���邳���グ��
            if (easingTimer_ <= maxTime)
            {
                float brightness = Easing::InSine(easingTimer_, maxTime, maxBrightness, minBrightness);

                easingTimer_ += elapsedTime;
                
                return brightness;
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

                easingTimer_ += elapsedTime;

                return brightness;
            }
            else
            {   // �t���O��ύX���āA�^�C�}�[���Z�b�g
                isBrightnessUp_ = !isBrightnessUp_;
                easingTimer_ = 0.0f;
            }
        }

        // ���蔲���Ă����Ƃ��p
        if (who == "LoadGame")
        {
            return owner->loadGameSprite_->GetEmissive()->GetEmissiveIntensity();
        }
        if (who == "QuitGame")
        {
            return owner->quitGameSprite_->GetEmissive()->GetEmissiveIntensity();
        }
        return isBrightnessUp_ ? maxBrightness : minBrightness;
    }
}

// QuitGameChoseState
namespace SceneTitleState
{
    // ������
    void QuitGameChoseState::Initialize()
    {
        // ���݂̃X�e�[�g��ݒ�
        owner->SetCurrentState(SceneTitle::STATE::QuitGameChose);

        // sprite������
        owner->blackBeltSprite_->GetSpriteTransform()->SetColorA(0.0f);
        owner->quitGameWordSprite_->GetSpriteTransform()->SetColorA(0.0f);
        owner->choseSprite_->GetSpriteTransform()->SetSize(0.0f);
        

        // �ϐ�������
        isChose_ = false;
        isPlayAnimation_ = false;
        isButtonPush = false;
        currentState_ = 0;
        easingTimer_ = 0.0f;
        animationTimer_ = 0.0f;
    }

    // �X�V
    void QuitGameChoseState::Update(const float& elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();

        if (!isChose_)
        {   // blackBeltSprite_ fadeIn
            float maxTime = 0.4f;
            float maxAlpha = 0.91f;
            if (easingTimer_ <= maxTime)
            {   // �����ƍ����т��t�F�C�h�C������
                float alpha = Easing::InSine(easingTimer_, maxTime, maxAlpha, 0.0f);
                float wordAlpha = Easing::InSine(easingTimer_, maxTime, 1.0f, 0.0f);

                owner->blackBeltSprite_->GetSpriteTransform()->SetColorA(alpha);
                owner->quitGameWordSprite_->GetSpriteTransform()->SetColorA(wordAlpha);

                easingTimer_ += elapsedTime;
            }
            else
            {   
                // �J���[��ݒ肷��
                owner->blackBeltSprite_->GetSpriteTransform()->SetColorA(maxAlpha);
                owner->quitGameWordSprite_->GetSpriteTransform()->SetColorA(1.0f);

                easingTimer_ = 0.0f;
                isChose_ = true;
                return;
            }
        }
        else
        {   // �I�� ( �Q�[���I�� or ���ǂ� )
            float aLx = gamePad.GetAxisLX();
            
#pragma region [�͂�,������]��I��
            if (currentState_ == static_cast<UINT>(STATE::Yes))
            {
                if (aLx > 0.8f)
                {
                    easingTimer_ = 0.0f;
                    isPlayAnimation_ = false;
                    owner->choseSprite_->ResetAnimation();
                    owner->choseSprite_->GetSpriteTransform()->SetSize(0.0f);

                    currentState_ = static_cast<UINT>(STATE::No);
                    return;
                }
            }
            else
            {
                if (aLx < -0.8f)
                {
                    easingTimer_ = 0.0f;
                    isPlayAnimation_ = false;
                    owner->choseSprite_->ResetAnimation();
                    owner->choseSprite_->GetSpriteTransform()->SetSize(0.0f);

                    currentState_ = static_cast<UINT>(STATE::Yes);
                    return;
                }
            }
#pragma endregion// [�͂�,������]��I��


            float maxTime = 0.2f;
            if (easingTimer_ <= maxTime)
            {   // �I��UI�����X�ɑ傫������
                float scale = Easing::InSine(easingTimer_, maxTime, 100.0f, 0.0f);

                // �ʒu�ݒ�
                if(currentState_==static_cast<UINT>(STATE::Yes))
                    owner->choseSprite_->GetSpriteTransform()->SetSpriteCenterPos(DirectX::XMFLOAT2(500, 425));
                else
                    owner->choseSprite_->GetSpriteTransform()->SetSpriteCenterPos(DirectX::XMFLOAT2(675, 425));

                // �傫���ݒ�
                owner->choseSprite_->GetSpriteTransform()->SetSize(scale);

                easingTimer_ += elapsedTime;
            }
            else
            {
                owner->choseSprite_->GetSpriteTransform()->SetSize(100.0f);

                isPlayAnimation_ = true;
            }

            if (isPlayAnimation_)
            {   // �A�j���[�V�����Đ�
                if (owner->choseSprite_->PlayAnimation(elapsedTime, 15.0f, 3, false))
                {   // �{�^����������悤�ɂȂ�
                    isButtonPush = true;
                }
            }

            // �{�^����������
            if (isButtonPush)
            {
                if (currentState_ == static_cast<UINT>(STATE::Yes))
                {   // �Q�[���I��
                    if(gamePad.GetButtonDown()&GamePad::BTN_A)
                        framework::GameEnd();
                    return;
                }
                else
                {   // select�ɖ߂�
                    if (gamePad.GetButtonDown() & GamePad::BTN_A)
                    owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneTitle::STATE::Select));
                    return;
                }
            }
        }
    }

    // �I����
    void QuitGameChoseState::Finalize()
    {
    }
}

namespace SceneTitleState
{

}