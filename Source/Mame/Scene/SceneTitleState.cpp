#include "SceneTitleState.h"

#include "../Input/Input.h"
#include "../Other/Easing.h"

#include "../Core/framework.h"

// FadeInState
namespace SceneTitleState
{
    // 初期化
    void FadeInState::Initialize()
    {
        // 変数初期化
        easingTimer_ = 0.0f;
    }

    // 更新
    void FadeInState::Update(const float& elapsedTime)
    {
        float maxTime = 1.0f;
        if (easingTimer_ <= maxTime)
        {
            float alpha = Easing::InSine(easingTimer_, maxTime, 1.0f, 0.0f);

            owner->titleLogoSprite_->GetSpriteTransform()->SetColorA(alpha);
            owner->pressAnyButtonSprite_->GetSpriteTransform()->SetColorA(alpha);

            easingTimer_ += elapsedTime;
        }
        else
        {
            owner->titleLogoSprite_->GetSpriteTransform()->SetColorA(1.0f);
            owner->pressAnyButtonSprite_->GetSpriteTransform()->SetColorA(1.0f);

            // 次のステートへ
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneTitle::STATE::PressAnyButton));
            return;
        }
    }

    // 終了化
    void FadeInState::Finalize()
    {
    }
}

// PressAnyButtonState
namespace SceneTitleState
{
    // 初期化
    void PressAnyButtonState::Initialize()
    {
        // 現在のステートを設定
        owner->SetCurrentState(SceneTitle::STATE::PressAnyButton);

        // sprite初期化
        owner->pressAnyButtonSprite_->GetSpriteTransform()->SetColorA(1.0f);

        // 変数初期化
        easingTimer_ = 0.0f;
        isBrightnessUp_ = false;
    }

    // 更新
    void PressAnyButtonState::Update(const float& elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();
        Mouse& mouse = Input::Instance().GetMouse();

#pragma region ボタン入力
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

        auto anyMouse =
            Mouse::BTN_LEFT
            | Mouse::BTN_MIDDLE
            | Mouse::BTN_RIGHT;

        // ボタンが押されたら
        if (((gamePad.GetButtonDown() & anyButton)
            || anyKey
            || (mouse.GetButtonDown() & anyMouse)))
        {   // 明るさを上げて、次のステートへ
            owner->pressAnyButtonSprite_->GetEmissive()->SetEmissiveIntensity(1.0f);
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneTitle::STATE::PressAnyButtonFadeOut));
            return;
        }
#pragma endregion// ボタン入力

        // 点滅更新
        UpdateSprite(elapsedTime);
    }

    // 終了化
    void PressAnyButtonState::Finalize()
    {
    }

    // 点滅更新
    void PressAnyButtonState::UpdateSprite(const float& elapsedTime)
    {
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

        // LoadGameが選択されているようにする
        owner->SetSelectState(0);

        // sprite初期化
        owner->loadGameSprite_->GetSpriteTransform()->SetColorA(0.0f);
        owner->quitGameSprite_->GetSpriteTransform()->SetColorA(0.0f);

        // LoadGameが最初に選択されてるようにする
        owner->loadGameSprite_->GetEmissive()->SetEmissiveIntensity(0.6f);
        owner->quitGameSprite_->GetEmissive()->SetEmissiveIntensity(-0.15f);

        // 変数初期化
        easingTimer_ = 0.0f;
    }

    // 更新
    void SelectFadeInState::Update(const float& elapsedTime)
    {
        float maxTime = 1.0f;
        if (easingTimer_ <= maxTime)
        {   // 徐々に文字が出て来る
            float alpha = Easing::InSine(easingTimer_, maxTime, 1.0f, 0.0f);

            owner->loadGameSprite_->GetSpriteTransform()->SetColorA(alpha);
            owner->quitGameSprite_->GetSpriteTransform()->SetColorA(alpha);

            easingTimer_ += elapsedTime;
        }
        else
        {   // alpha値が1を超えたら
            // alpha値を１にしておく
            owner->loadGameSprite_->GetSpriteTransform()->SetColorA(1.0f);
            owner->quitGameSprite_->GetSpriteTransform()->SetColorA(1.0f);

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
        // 現在のステートを設定
        owner->SetCurrentState(SceneTitle::STATE::Select);

        // sprite初期化
        // LoadGameが最初に選択されてるようにする
        owner->loadGameSprite_->GetEmissive()->SetEmissiveIntensity(0.4f);
        owner->quitGameSprite_->GetEmissive()->SetEmissiveIntensity(-0.15f);

        // 変数初期化
        easingTimer_ = 0.0f;
        currentState_ = owner->GetSelectState();
        isBrightnessUp_ = false;
        isChangeState_ = false;
        breakTimer_ = 0.0f;
    }

    // 更新
    void SelectState::Update(const float& elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();
        Mouse& mouse = Input::Instance().GetMouse();

        // Bボタン or 右クリック
        if ((gamePad.GetButtonDown() & GamePad::BTN_B)
            || (mouse.GetButtonDown() & Mouse::BTN_RIGHT))
        {   // PressAnyButtonに戻る
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneTitle::STATE::PressAnyButton));
            return;
        }

        // ステート更新
        UpdateState(elapsedTime);
    }

    // 終了化
    void SelectState::Finalize()
    {
    }

    // ステート更新
    void SelectState::UpdateState(const float& elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();

        float aLY = gamePad.GetAxisLY();
        float minBrightness = -0.15f;

        float stick = 0.8f;

        Mouse& mouse = Input::Instance().GetMouse();
        DirectX::XMFLOAT2 mousePos = { static_cast<float>(mouse.GetPositionX()), static_cast<float>(mouse.GetPositionY()) };

#pragma region マウスによる選択
        // ---------- マウスのカーソルに位置によってステートを変更 ----------

        // --- LoadGameを選択 ---
        if ((mousePos.x > mousePos_[0].left_) && (mousePos.x < mousePos_[0].right_) && (mousePos.y > mousePos_[0].top_) && (mousePos.y < mousePos_[0].bottom_))
        {
            ChangeCurrentState(STATE::LoadGame);
        }
        // --- QuitGameを選択 ---
        if ((mousePos.x > mousePos_[1].left_) && (mousePos.x < mousePos_[1].right_) && (mousePos.y > mousePos_[1].top_) && (mousePos.y < mousePos_[1].bottom_))
        {
            ChangeCurrentState(STATE::QuitGame);
        }

        // ---------- マウスクリックによる選択 ----------

        // --- LoadGameを選択 ---
        if (((mousePos.x > mousePos_[0].left_) && (mousePos.x < mousePos_[0].right_) && (mousePos.y > mousePos_[0].top_) && (mousePos.y < mousePos_[0].bottom_))
            && (mouse.GetButtonDown() & Mouse::BTN_LEFT))
        {
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneTitle::STATE::LoadGameFadeOut));
            return;
        }
        // --- QuitGameを選択 ---
        if (((mousePos.x > mousePos_[1].left_) && (mousePos.x < mousePos_[1].right_) && (mousePos.y > mousePos_[1].top_) && (mousePos.y < mousePos_[1].bottom_))
            && (mouse.GetButtonDown() & Mouse::BTN_LEFT))
        {
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneTitle::STATE::QuitGameChose));
            return;
        }

#pragma endregion// マウスによる選択

#pragma region ゲームパッドによる選択 (keyboard)
        // ---------- 入力によるステート変更 ----------
        switch (currentState_)
        {
        case static_cast<UINT>(STATE::LoadGame):
            // --- LoadGame ---
            if ((aLY > stick) && isChangeState_)
            {   // スティックを上に傾けた
                ChangeCurrentState(STATE::LoadGame);
                break;
            }
            if ((aLY < -stick) && isChangeState_)
            {   // スティックを下に傾けた
                ChangeCurrentState(STATE::QuitGame);
                break;
            }

            break;
        case static_cast<UINT>(STATE::QuitGame):
            // --- QuitGame ---
            if ((aLY > stick) && isChangeState_)
            {   // スティックを上に傾けた
                ChangeCurrentState(STATE::LoadGame);
                break;
            }
            if ((aLY < -stick) && isChangeState_)
            {   // スティックを下に傾けた
                ChangeCurrentState(STATE::QuitGame);
                break;
            }

            break;
        }

        // ---------- ボタン入力による選択 ----------
        switch (currentState_)
        {
        case static_cast<UINT>(STATE::LoadGame):
            // --- LoadGame ---
            if (gamePad.GetButtonDown() & GamePad::BTN_A)
            {   // ゲーム選択
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneTitle::STATE::LoadGameFadeOut));
                return;
            }

            break;
        case static_cast<UINT>(STATE::QuitGame):
            // --- QuitGame ---
            if (gamePad.GetButtonDown() & GamePad::BTN_A)
            {   // 選択した
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneTitle::STATE::QuitGameChose));
                return;
            }

            break;
        }
#pragma endregion// ゲームパッドによる選択 (keyboard)

        // ---------- 点滅更新 (分岐) ----------
        switch (currentState_)
        {
        case static_cast<UINT>(STATE::LoadGame):
            // --- LoadGame ---
            owner->quitGameSprite_->GetEmissive()->SetEmissiveIntensity(minBrightness);
            owner->loadGameSprite_->GetEmissive()->SetEmissiveIntensity(
                UpdateBrightness(elapsedTime, "LoadGame"));

            break;
        case static_cast<UINT>(STATE::QuitGame):
            // --- QuitGame ---
            owner->loadGameSprite_->GetEmissive()->SetEmissiveIntensity(minBrightness);
            owner->quitGameSprite_->GetEmissive()->SetEmissiveIntensity(
                UpdateBrightness(elapsedTime, "QuitGame"));

            break;
        }

        // ---------- 連続して選択できると操作性が悪いのでbreakTimeを設ける ----------
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

    // ゆったりと点滅させる
    float SelectState::UpdateBrightness(const float& elapsedTime, const char* who)
    {
        float maxTime = 0.7f;
        float maxBrightness = 0.4f;
        float minBrightness = -0.15f;

        if (isBrightnessUp_)
        {   // 明るさを上げる
            if (easingTimer_ <= maxTime)
            {
                float brightness = Easing::InSine(easingTimer_, maxTime, maxBrightness, minBrightness);

                easingTimer_ += elapsedTime;
                
                return brightness;
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

                easingTimer_ += elapsedTime;

                return brightness;
            }
            else
            {   // フラグを変更して、タイマーリセット
                isBrightnessUp_ = !isBrightnessUp_;
                easingTimer_ = 0.0f;
            }
        }

        // すり抜けてきたとき用
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

    // ステート切り替え用
    void SelectState::ChangeCurrentState(STATE state)
    {
        isChangeState_ = false;
        currentState_ = static_cast<UINT>(state);
        owner->SetSelectState(currentState_);
    }
}

// QuitGameChoseState
namespace SceneTitleState
{
    // 初期化
    void QuitGameChoseState::Initialize()
    {
        // 現在のステートを設定
        owner->SetCurrentState(SceneTitle::STATE::QuitGameChose);

        // sprite初期化
        owner->blackBeltSprite_->GetSpriteTransform()->SetColorA(0.0f);
        owner->quitGameWordSprite_->GetSpriteTransform()->SetColorA(0.0f);
        owner->choseSprite_->GetSpriteTransform()->SetSize(0.0f);
        

        // 変数初期化
        isChose_ = false;
        isPlayAnimation_ = false;
        currentState_ = 0;
        easingTimer_ = 0.0f;
        animationTimer_ = 0.0f;
    }

    // 更新
    void QuitGameChoseState::Update(const float& elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();

        if (!isChose_)
        {   // blackBeltSprite_ fadeIn
            float maxTime = 0.4f;
            float maxAlpha = 0.91f;
            if (easingTimer_ <= maxTime)
            {   // 文字と黒い帯をフェイドインする
                float alpha = Easing::InSine(easingTimer_, maxTime, maxAlpha, 0.0f);
                float wordAlpha = Easing::InSine(easingTimer_, maxTime, 1.0f, 0.0f);

                owner->blackBeltSprite_->GetSpriteTransform()->SetColorA(alpha);
                owner->quitGameWordSprite_->GetSpriteTransform()->SetColorA(wordAlpha);

                easingTimer_ += elapsedTime;
            }
            else
            {   
                // カラーを設定する
                owner->blackBeltSprite_->GetSpriteTransform()->SetColorA(maxAlpha);
                owner->quitGameWordSprite_->GetSpriteTransform()->SetColorA(1.0f);

                easingTimer_ = 0.0f;
                isChose_ = true;
                return;
            }
        }
        else
        {   // 選択 ( ゲーム終了 or もどる )
            float aLx = gamePad.GetAxisLX();
            
#pragma region [はい,いいえ]を選択
            if (currentState_ == static_cast<UINT>(STATE::Yes))
            {   // [はい] にカーソルがある
                if (gamePad.GetButtonDown() & GamePad::BTN_B)
                {
                    easingTimer_ = 0.0f;
                    isPlayAnimation_ = false;
                    owner->choseSprite_->ResetAnimation();
                    owner->choseSprite_->GetSpriteTransform()->SetSize(0.0f);

                    currentState_ = static_cast<UINT>(STATE::No);
                    return;
                }

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
#pragma endregion// [はい,いいえ]を選択


            float maxTime = 0.2f;
            if (easingTimer_ <= maxTime)
            {   // 選択UIを徐々に大きくする
                float scale = Easing::InSine(easingTimer_, maxTime, 100.0f, 0.0f);

                // 位置設定
                if(currentState_==static_cast<UINT>(STATE::Yes))
                    owner->choseSprite_->GetSpriteTransform()->SetSpriteCenterPos(DirectX::XMFLOAT2(500, 425));
                else
                    owner->choseSprite_->GetSpriteTransform()->SetSpriteCenterPos(DirectX::XMFLOAT2(675, 425));

                // 大きさ設定
                owner->choseSprite_->GetSpriteTransform()->SetSize(scale);

                easingTimer_ += elapsedTime;
            }
            else
            {
                owner->choseSprite_->GetSpriteTransform()->SetSize(100.0f);

                isPlayAnimation_ = true;
            }

            if (isPlayAnimation_)
            {   // アニメーション再生
                owner->choseSprite_->PlayAnimation(elapsedTime, 15.0f, 3, false);
            }


            // ボタン押す処理
            if (currentState_ == static_cast<UINT>(STATE::Yes))
            {   // ゲーム終了
                if (gamePad.GetButtonDown() & GamePad::BTN_A)
                    framework::GameEnd();
                return;
            }
            else
            {   // selectに戻る
                if (gamePad.GetButtonDown() & GamePad::BTN_A)
                    owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneTitle::STATE::Select));
                return;
            }

        }
    }

    // 終了化
    void QuitGameChoseState::Finalize()
    {
    }
}

// LoadGameFadeOut
namespace SceneTitleState
{
    // 初期化
    void LoadGameFadeOut::Initialize()
    {
        // 現在のステートを設定
        owner->SetCurrentState(SceneTitle::STATE::LoadGameFadeOut);

        // 変数初期化
        easingTimer_ = 0.0f;
        delayTimer_ = 0.0f;
        isChangeScene_ = false;
    }

    // 更新
    void LoadGameFadeOut::Update(const float& elapsedTime)
    {
        float maxTime = 1.0f;
        float maxAlpha = 1.0f;
        float minAlpha = 0.0f;
        if (easingTimer_ <= maxTime)
        {
            float alpha = Easing::InSine(easingTimer_, maxTime, minAlpha, maxAlpha);

            owner->titleLogoSprite_->GetSpriteTransform()->SetColorA(alpha);
            owner->loadGameSprite_->GetSpriteTransform()->SetColorA(alpha);
            owner->quitGameSprite_->GetSpriteTransform()->SetColorA(alpha);

            easingTimer_ += elapsedTime;
        }
        else
        {
            owner->titleLogoSprite_->GetSpriteTransform()->SetColorA(minAlpha);
            owner->loadGameSprite_->GetSpriteTransform()->SetColorA(minAlpha);
            owner->quitGameSprite_->GetSpriteTransform()->SetColorA(minAlpha);

            // すぐに切り替わるのはダサいから少しディレイをはさむ
            float maxDelay = 0.3f;
            delayTimer_ += elapsedTime;
            if (delayTimer_ >= maxDelay)
            {
                // シーン切り替え
                if (owner->GetIsReady())
                {
                    owner->ChangeScene();
                    return;
                }
            }
        }
    }

    // 終了化
    void LoadGameFadeOut::Finalize()
    {
    }
}