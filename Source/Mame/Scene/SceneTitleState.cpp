#include "SceneTitleState.h"

#include "../Input/Input.h"
#include "../Other/Easing.h"

// PressAnyButtonState
namespace SceneTitleState
{
    // 初期化
    void PressAnyButtonState::Initialize()
    {
        // 現在のステートを設定
        owner->SetCurrentState(SceneTitle::STATE::PressAnyButton);

        // 変数初期化
        easingTimer_ = 0.0f;
        isBrightnessUp_ = false;
    }

    // 更新
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

        // ボタンが押されたら
        if (((gamePad.GetButtonDown() & anyButton) || anyKey))
        {   // 明るさを上げて、次のステートへ
            owner->pressAnyButtonSprite_->GetEmissive()->SetEmissiveIntensity(1.0f);
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneTitle::STATE::PressAnyButtonFadeOut));
            return;
        }

        float maxTime = 0.7f;
        float maxBrightness = 0.4f;
        float minBrightness = -0.15f;

        // ゆったりと点滅させる
        if (isBrightnessUp_)
        {   // 明るさを上げる
            if (easingTimer_ <= maxTime)
            {
                float brightness = Easing::InSine(easingTimer_, maxTime, maxBrightness, minBrightness);

                owner->pressAnyButtonSprite_->GetEmissive()->SetEmissiveIntensity(brightness);

                easingTimer_ += elapsedTime;
            }
            else
            {   // フラグを変更して、タイマーリセット
                isBrightnessUp_ = !isBrightnessUp_;
                easingTimer_ = 0.0f;
            }
        }
        else
        {   // 明るさを下げる
            if (easingTimer_ <= maxTime)
            {
                float brightness = Easing::InSine(easingTimer_, maxTime, minBrightness, maxBrightness);

                owner->pressAnyButtonSprite_->GetEmissive()->SetEmissiveIntensity(brightness);

                easingTimer_ += elapsedTime;
            }
            else
            {   // フラグを変更して、タイマーリセット
                isBrightnessUp_ = !isBrightnessUp_;
                easingTimer_ = 0.0f;
            }
        }
    }

    // 終了化
    void PressAnyButtonState::Finalize()
    {
    }
}

// PressAnyButtonFadeOutState
namespace SceneTitleState
{
    // 初期化
    void PressAnyButtonFadeOutState::Initialize()
    {
        // 現在のステートを設定
        owner->SetCurrentState(SceneTitle::STATE::PressAnyButtonFadeOut);

        // 変数初期化
        easingTimer_ = 0.0f;
    }

    // 更新
    void PressAnyButtonFadeOutState::Update(const float& elapsedTime)
    {   
        float maxTime = 1.0f;
        if (easingTimer_ <= maxTime)
        {   // 徐々に透明にしていく
            float alpha = Easing::InSine(easingTimer_, maxTime, 0.0f, 1.0f);

            owner->pressAnyButtonSprite_->GetSpriteTransform()->SetColorA(alpha);

            easingTimer_ += elapsedTime;
        }
        else
        {   // 透明になった
            // 完全にalphaを0にする
            owner->pressAnyButtonSprite_->GetSpriteTransform()->SetColorA(0.0f);

            // selectで出てくるspriteのalphaを0にする
            owner->LoadGameSprite_->GetSpriteTransform()->SetColorA(0.0f);
            owner->QuitGameSprite_->GetSpriteTransform()->SetColorA(0.0f);

            // selectでLoadGameが最初に選択されてるようにする
            owner->LoadGameSprite_->GetEmissive()->SetEmissiveIntensity(0.6f);
            owner->QuitGameSprite_->GetEmissive()->SetEmissiveIntensity(-0.15f);

            //次のステートへ
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneTitle::STATE::SelectFadeIn));

            return;
        }
    }

    // 終了化
    void PressAnyButtonFadeOutState::Finalize()
    {
    }
}

// SelectFadeInState
namespace SceneTitleState
{
    // 初期化
    void SelectFadeInState::Initialize()
    {
        // 現在のステートを設定
        owner->SetCurrentState(SceneTitle::STATE::SelectFadeIn);
    }

    // 更新
    void SelectFadeInState::Update(const float& elapsedTime)
    {
        float maxTime = 1.0f;
        if (easingTimer_ <= maxTime)
        {   // 徐々に文字が出て来る
            float alpha = Easing::InSine(easingTimer_, maxTime, 1.0f, 0.0f);

            owner->LoadGameSprite_->GetSpriteTransform()->SetColorA(alpha);
            owner->QuitGameSprite_->GetSpriteTransform()->SetColorA(alpha);

            easingTimer_ += elapsedTime;
        }
        else
        {   // alpha値が1を超えたら
            // alpha値を１にしておく
            owner->LoadGameSprite_->GetSpriteTransform()->SetColorA(1.0f);
            owner->QuitGameSprite_->GetSpriteTransform()->SetColorA(1.0f);

            // 次のステートへ
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneTitle::STATE::Select));

            return;
        }
    }

    // 終了化
    void SelectFadeInState::Finalize()
    {
    }
}

// SelectState
namespace SceneTitleState
{
    // 初期化
    void SelectState::Initialize()
    {
        owner->LoadGameSprite_->GetEmissive()->SetEmissiveIntensity(0.4f);
        owner->QuitGameSprite_->GetEmissive()->SetEmissiveIntensity(-0.15f);
    }

    // 更新
    void SelectState::Update(const float& elapsedTime)
    {
        float maxTime = 0.7f;
        float maxBrightness = 0.4f;
        float minBrightness = -0.15f;

    }

    // 終了化
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