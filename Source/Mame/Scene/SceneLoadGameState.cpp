#include "SceneLoadGameState.h"

#include "../Other/Easing.h"

#include "../Input/Input.h"

#include "../Scene/SceneManager.h"
#include "../Scene/SceneLoading.h"
#include "../Scene/SceneGame.h"
#include "../Scene/SceneDummy.h"
#include "../Scene/SceneLoadingBack.h"
#include "../Scene/SceneTitle.h"

// SlideUiState
namespace SceneLoadGameState
{
    // 初期化
    void SlideUiState::Initialize()
    {
        // 現在のステートを設定
        owner->SetCurrentState(SceneLoadGame::STATE::SlideUi);

        // 変数初期化
        easingTimer_ = 0.0f;
    }

    // 更新
    void SlideUiState::Update(const float& elapsedTime)
    {
        float maxTime = 0.4f;
        float startGameDataBase = -1100.0f;
        float endGameDataBase = 125.0f;
        if (easingTimer_ <= maxTime)
        {
            float gameDataBasePosX = Easing::InSine(easingTimer_, maxTime, endGameDataBase, startGameDataBase);

            for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
            {
                owner->gameDataBaseSprite_[i]->GetSpriteTransform()->SetPosX(gameDataBasePosX);
                owner->gameDataWordSprite_[i]->GetSpriteTransform()->SetPosX(gameDataBasePosX);
            }

            owner->gameDataChoseSprite_->GetSpriteTransform()->SetPosX(gameDataBasePosX);

            float bookSpritePosX = Easing::InSine(easingTimer_, maxTime, endGameDataBase - endGameDataBase, startGameDataBase - endGameDataBase);
            owner->bookSprite_->GetSpriteTransform()->SetPosX(bookSpritePosX);
            owner->choseLoadDataWordSprite_->GetSpriteTransform()->SetPosX(bookSpritePosX);

            float pointPosX = Easing::InSine(easingTimer_, maxTime, endGameDataBase - 20.0f, startGameDataBase - 20.0f);
            owner->pointWakuSprite_->GetSpriteTransform()->SetPosX(pointPosX);
            owner->pointRhombusSprite_->GetSpriteTransform()->SetPosX(pointPosX);


            easingTimer_ += elapsedTime;
        }
        else
        {
            for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
            {
                owner->gameDataBaseSprite_[i]->GetSpriteTransform()->SetPosX(endGameDataBase);
                owner->gameDataWordSprite_[i]->GetSpriteTransform()->SetPosX(endGameDataBase);
            }

            owner->gameDataChoseSprite_->GetSpriteTransform()->SetPosX(endGameDataBase);

            owner->bookSprite_->GetSpriteTransform()->SetPosX(endGameDataBase - endGameDataBase);
            owner->choseLoadDataWordSprite_->GetSpriteTransform()->SetPosX(endGameDataBase - endGameDataBase);

            owner->pointWakuSprite_->GetSpriteTransform()->SetPosX(endGameDataBase - 20.0f);
            owner->pointRhombusSprite_->GetSpriteTransform()->SetPosX(endGameDataBase - 20.0f);

            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneLoadGame::STATE::Select));
            return;
        }
    }

    // 終了化
    void SlideUiState::Finalize()
    {
    }
}

// SelectState
namespace SceneLoadGameState
{
    // 初期化
    void SelectState::Initialize()
    {
        // 現在のステートを設定
        owner->SetCurrentState(SceneLoadGame::STATE::Select);

        // spriteリセット
        owner->gameDataChoseSprite_->GetSpriteDissolve()->SetDissolveValue(0.0f);
        owner->gameDataChoseSprite_->GetSpriteTransform()->SetColorA(1.0f);

        // 変数リセット
        gameDataNum_ = GameData::One;
        isFadeSprite_ = false;
        fadeSpriteTimer_ = 0.0f;
    }

    // 更新
    void SelectState::Update(const float& elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();

        // タイトルに戻る
        if (gamePad.GetButtonDown() & GamePad::BTN_B)
        {
            owner->loadingScene_ = (new SceneLoading(new SceneDummy));
            owner->thread_ = new std::thread(owner->LoadingThread, owner);
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneLoadGame::STATE::LoadTitle));
            return;
        }

        // ゲームデータを選択
        UpdateSelectGameData(elapsedTime);

        // breakTime
        UpdateBreakTime(elapsedTime);

        if (isFadeSprite_)
        {
            float maxTime = 0.2f;
            if (fadeSpriteTimer_ <= maxTime)
            {
                float fade = Easing::InSine(fadeSpriteTimer_, maxTime, 0.0f, 0.26f);
                float alpha = Easing::OutSine(fadeSpriteTimer_, maxTime, 1.0f, 0.0f);

                owner->gameDataChoseSprite_->GetSpriteDissolve()->SetDissolveValue(fade);
                owner->gameDataChoseSprite_->GetSpriteTransform()->SetColorA(alpha);

                fadeSpriteTimer_ += elapsedTime;
            }
            else
            {
                owner->gameDataChoseSprite_->GetSpriteDissolve()->SetDissolveValue(0.0f);
                owner->gameDataChoseSprite_->GetSpriteTransform()->SetColorA(1.0f);
                isFadeSprite_ = false;
            }
        }

        // 次のステートへ
        if (gamePad.GetButtonDown() & GamePad::BTN_A)
        {
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneLoadGame::STATE::LoadGameChose));
            return;
        }
    }

    // 終了化
    void SelectState::Finalize()
    {

    }

    // ゲームデータ選択
    void SelectState::UpdateSelectGameData(const float& elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();

        float aLY = gamePad.GetAxisLY();
        float stick = 0.8f;

        switch (gameDataNum_)
        {
        case GameData::One:
            // 現在選んでいるゲームデータを保存
            owner->SetSelectState(static_cast<UINT>(gameDataNum_));


            if ((aLY < -stick) && isChangeState_
                && (maxGameDataNum_ > GameData::One))
            {   // スティックを下に傾けた
                isChangeState_ = false;
                isFadeSprite_ = true;
                fadeSpriteTimer_ = 0.0f;
                gameDataNum_ = GameData::Two;
                break;
            }

            owner->gameDataChoseSprite_->GetSpriteTransform()->SetPos(owner->spritePos[0]);

            break;
        case GameData::Two:
            // 現在選んでいるゲームデータを保存
            owner->SetSelectState(static_cast<UINT>(gameDataNum_));

            if ((aLY < -stick) && isChangeState_
                && (maxGameDataNum_ > GameData::Two))
            {   // スティックを下に傾けた
                isChangeState_ = false;
                isFadeSprite_ = true;
                fadeSpriteTimer_ = 0.0f;
                gameDataNum_ = GameData::Three;
                break;
            }

            if ((aLY > stick) && isChangeState_)
            {   // スティックを上に傾けた
                isChangeState_ = false;
                isFadeSprite_ = true;
                fadeSpriteTimer_ = 0.0f;
                gameDataNum_ = GameData::One;
                break;
            }

            owner->gameDataChoseSprite_->GetSpriteTransform()->SetPos(owner->spritePos[1]);

            break;
        case GameData::Three:
            // 現在選んでいるゲームデータを保存
            owner->SetSelectState(static_cast<UINT>(gameDataNum_));

            if ((aLY > stick) && isChangeState_)
            {   // スティックを上に傾けた
                isChangeState_ = false;
                isFadeSprite_ = true;
                fadeSpriteTimer_ = 0.0f;
                gameDataNum_ = GameData::Two;
                break;
            }

            owner->gameDataChoseSprite_->GetSpriteTransform()->SetPos(owner->spritePos[2]);
            break;
        }
    }

    // 連続して選択してしまったらストレスなので、breakTimeを挟む
    void SelectState::UpdateBreakTime(const float& elapsedTime)
    {
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
}

// LoadGameChoseState
namespace SceneLoadGameState
{
    // 初期化
    void LoadGameChoseState::Initialize()
    {
        // 現在のステートを設定
        owner->SetCurrentState(SceneLoadGame::STATE::LoadGameChose);

        // Sprite初期化
        owner->blackBeltSprite_->GetSpriteTransform()->SetColorA(0.0f);
        owner->loadGameWordSprite_->GetSpriteTransform()->SetColorA(0.0f);
        owner->choseSprite_->GetSpriteTransform()->SetSize(0.0f);
        owner->choseSprite_->GetSpriteTransform()->SetColorA(1.0f);

        // 変数初期化
        isChose_ = false;
        
        currentState_ = 0;
        easingTimer_ = 0.0f;
        animationTimer_ = 0.0f;
    }

    // 更新
    void LoadGameChoseState::Update(const float& elapsedTime)
    {
        if (!isChose_)
        {   // UI フェイドイン
            FadeInUI(elapsedTime);
        }
        else
        {   // 選択 ( ゲーム読み込む or 戻る )
            // はい or いいえ を選択する
            ChoseYesOrNo(elapsedTime);

            // 選択カーソル更新。
            if (ChoseSpriteUpdate(elapsedTime))
            {
                // アニメーション再生可能なら再生。
                owner->choseSprite_->PlayAnimation(elapsedTime, 15.0f, 3, false);
            }

            // ボタン押されるのまってる。
            PushButton();
        }

    }

    // 終了化
    void LoadGameChoseState::Finalize()
    {
    }

    // UI フェイドイン
    void LoadGameChoseState::FadeInUI(const float& elapsedTime)
    {
        float maxTime = 0.4f;
        float maxAlpha = 0.91f;
        if (easingTimer_ <= maxTime)
        {   // 文字と黒い帯をフェイドインする
            float alpha = Easing::InSine(easingTimer_, maxTime, maxAlpha, 0.0f);
            float wordAlpha = Easing::InSine(easingTimer_, maxTime, 1.0f, 0.0f);

            owner->blackBeltSprite_->GetSpriteTransform()->SetColorA(alpha);
            owner->loadGameWordSprite_->GetSpriteTransform()->SetColorA(wordAlpha);

            easingTimer_ += elapsedTime;
        }
        else
        {
            // カラーを設定する
            owner->blackBeltSprite_->GetSpriteTransform()->SetColorA(maxAlpha);
            owner->loadGameWordSprite_->GetSpriteTransform()->SetColorA(1.0f);

            easingTimer_ = 0.0f;
            isChose_ = true;
            return;
        }
    }
    
    // はい か いいえ を選択する
    void LoadGameChoseState::ChoseYesOrNo(const float& elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();
        float aLx = gamePad.GetAxisLX();

        if (currentState_ == static_cast<UINT>(STATE::Yes))
        {
            if (gamePad.GetButtonDown() & GamePad::BTN_B)
            {
                easingTimer_ = 0.0f;
                owner->choseSprite_->ResetAnimation();
                owner->choseSprite_->GetSpriteTransform()->SetSize(0.0f);

                currentState_ = static_cast<UINT>(STATE::No);
                return;
            }

            if (aLx > 0.8f)
            {   // スティックを右に傾けた。
                easingTimer_ = 0.0f;
                owner->choseSprite_->ResetAnimation();
                owner->choseSprite_->GetSpriteTransform()->SetSize(0.0f);

                currentState_ = static_cast<UINT>(STATE::No);
                return;
            }
        }
        else
        {
            if (aLx < -0.8f)
            {   // スティックを左に傾けた。
                easingTimer_ = 0.0f;
                owner->choseSprite_->ResetAnimation();
                owner->choseSprite_->GetSpriteTransform()->SetSize(0.0f);

                currentState_ = static_cast<UINT>(STATE::Yes);
                return;
            }
        }
    }

    // 選択カーソルの更新
    bool LoadGameChoseState::ChoseSpriteUpdate(const float& elapsedTime)
    {
        float maxTime = 0.2f;
        if (easingTimer_ <= maxTime)
        {   // 選択UIを徐々に大きくする
            float scale = Easing::InSine(easingTimer_, maxTime, 100.0f, 0.0f);

            // 位置設定
            if (currentState_ == static_cast<UINT>(STATE::Yes))
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

            // maxに到達していたらアニメーション再生可能
            return true;
        }

        // アニメーション再生不可
        return false;
    }

    // ボタン押されるのまってる。
    void LoadGameChoseState::PushButton()
    {
        GamePad& gamePad = Input::Instance().GetGamePad();
        if (currentState_ == static_cast<UINT>(STATE::Yes))
        {   // ゲームを読み込む
            if (gamePad.GetButtonDown() & GamePad::BTN_A)
            {
                // 最終確認用Spriteを消す
                owner->blackBeltSprite_->GetSpriteTransform()->SetColorA(0.0f);
                owner->loadGameWordSprite_->GetSpriteTransform()->SetColorA(0.0f);
                owner->choseSprite_->GetSpriteTransform()->SetColorA(0.0f);

                // 次のステートへ
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneLoadGame::STATE::LoadGame));
                return;

#if 0
                // どのゲームデータに飛ぶか
                switch (owner->GetSelectState())
                {
                case static_cast<UINT>(SceneLoadGame::GameDataBase::Top):
                    // 一つ目のデータ

                    break;
                case static_cast<UINT>(SceneLoadGame::GameDataBase::Middle):
                    // 二つ目のデータ
                    break;
                case static_cast<UINT>(SceneLoadGame::GameDataBase::Bottom):
                    // 三つ目のデータ
                    break;
                }
#endif
            }
        }
        else
        {   // 戻る
            if (gamePad.GetButtonDown() & GamePad::BTN_A)
            {
                // 最終確認用Spriteを消す
                owner->blackBeltSprite_->GetSpriteTransform()->SetColorA(0.0f);
                owner->loadGameWordSprite_->GetSpriteTransform()->SetColorA(0.0f);
                owner->choseSprite_->GetSpriteTransform()->SetColorA(0.0f);

                // 次のステートへ
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneLoadGame::STATE::Select));
                return;
            }
        }
    }
}

// SlideUiFadeOutState
namespace SceneLoadGameState
{
    // 初期化
    void SlideUiFadeOutState::Initialize()
    {
        // 現在のステートを設定
        owner->SetCurrentState(SceneLoadGame::STATE::SlideUiFadeOut);

        // 変数初期化
        slideTimer_ = 0.0f;
        easingTimer_ = 0.0f;
        isFadeOut_ = false;
    }

    // 更新
    void SlideUiFadeOutState::Update(const float& elapsedTime)
    {
#pragma region 画像を移動させる
        float maxSlideTime = 0.2f;
        float beforePosX = 125.0f;
        float afterPosX = -1100.0f;
        
        if (!isFadeOut_)
        {
            if (slideTimer_ <= maxSlideTime)
            {
                float spritePosX = Easing::InSine(slideTimer_, maxSlideTime, afterPosX, beforePosX);

                for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
                {
                    owner->gameDataBaseSprite_[i]->GetSpriteTransform()->SetPosX(spritePosX);
                    owner->gameDataWordSprite_[i]->GetSpriteTransform()->SetPosX(spritePosX);
                }

                owner->gameDataChoseSprite_->GetSpriteTransform()->SetPosX(spritePosX);

                float bookSpritePosX = Easing::InSine(slideTimer_, maxSlideTime, afterPosX - beforePosX, beforePosX - beforePosX);
                owner->bookSprite_->GetSpriteTransform()->SetPosX(bookSpritePosX);
                owner->choseLoadDataWordSprite_->GetSpriteTransform()->SetPosX(bookSpritePosX);

                float pointPosX = Easing::InSine(slideTimer_, maxSlideTime, afterPosX - 20.0f, beforePosX - 20.0f);
                owner->pointWakuSprite_->GetSpriteTransform()->SetPosX(pointPosX);
                owner->pointRhombusSprite_->GetSpriteTransform()->SetPosX(pointPosX);

                slideTimer_ += elapsedTime;
            }
            else
            {
                // Spriteを指定の位置に設定
                for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
                {
                    owner->gameDataBaseSprite_[i]->GetSpriteTransform()->SetPosX(afterPosX);
                    owner->gameDataWordSprite_[i]->GetSpriteTransform()->SetPosX(afterPosX);
                }
                owner->gameDataChoseSprite_->GetSpriteTransform()->SetPosX(afterPosX);

                owner->bookSprite_->GetSpriteTransform()->SetPosX(afterPosX - beforePosX);
                owner->choseLoadDataWordSprite_->GetSpriteTransform()->SetPosX(afterPosX - beforePosX);

                owner->pointWakuSprite_->GetSpriteTransform()->SetPosX(afterPosX - 20.0f);
                owner->pointRhombusSprite_->GetSpriteTransform()->SetPosX(afterPosX - 20.0f);

                isFadeOut_ = true;
            }
        }
#pragma endregion// 画像を移動させる

        // フェードアウト
        if (isFadeOut_)
        {
            float maxTime = 0.2f;
            if (easingTimer_ <= maxTime)
            {
                float alpha = Easing::OutSine(easingTimer_, maxTime, 1.0f, 0.0f);

                owner->whiteSprite_->GetSpriteTransform()->SetColorA(alpha);

                easingTimer_ += elapsedTime;
            }
            else
            {
                // 次のステートへ
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneLoadGame::STATE::ChangeScene));
                return;
            }
        }

    }

    // 終了化
    void SlideUiFadeOutState::Finalize()
    {
    }
}

// LoadGameState
namespace SceneLoadGameState
{
    // 初期化
    void LoadGameState::Initialize()
    {
        // 現在のステートを設定
        owner->SetCurrentState(SceneLoadGame::STATE::LoadGame);

        // 選択したステートによってロードするデータを分ける
        switch (owner->GetSelectState())
        {
        case 0:
            owner->loadingScene_ = (new SceneLoading(new SceneGame));
            break;
        case 1:
            break;
        case 2:
            break;
        }

        // スプライト初期化
        owner->halfBlackBeltSprite_->GetSpriteTransform()->SetColorA(0.0f);
        owner->loadingWordSprite_->GetSpriteTransform()->SetColorA(0.0f);
        owner->completeWordSprite_->GetSpriteTransform()->SetColorA(0.0f);
        owner->loadArrowSprite_->GetSpriteTransform()->SetColorA(0.0f);

        // 変数初期化
        loadingTimer_ = 0.0f;
        easingTimer_ = 0.0f;
        isChangeState_ = false;

        isCompleteFadeIn_ = false;

        // スレッド開始
        owner->thread_ = new std::thread(owner->LoadingThread, owner);
    }

    // 更新
    void LoadGameState::Update(const float& elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();
        Mouse& mouse = Input::Instance().GetMouse();

        // 「ゲームデータを読み込んでいます。」を表示しきっていない。
        // または、　ロードが終わっていない
        if (!isCompleteFadeIn_
            || !(owner->loadingScene_->IsReady()))
        {
            float maxTime = 0.3f;
            if (loadingTimer_ <= maxTime)
            {
                float alpha = Easing::InSine(loadingTimer_, maxTime, 1.0f, 0.0f);

                owner->halfBlackBeltSprite_->GetSpriteTransform()->SetColorA(alpha);
                owner->loadingWordSprite_->GetSpriteTransform()->SetColorA(alpha);
                owner->loadArrowSprite_->GetSpriteTransform()->SetColorA(alpha);

                loadingTimer_ += elapsedTime;
            }
            else
            {
                owner->halfBlackBeltSprite_->GetSpriteTransform()->SetColorA(1.0f);
                owner->loadingWordSprite_->GetSpriteTransform()->SetColorA(1.0f);
                owner->loadArrowSprite_->GetSpriteTransform()->SetColorA(1.0f);

                isCompleteFadeIn_ = true;
            }

        }
        
        // ロードの文字回転
        owner->loadArrowSprite_->GetSpriteTransform()->AddAngle(360 * elapsedTime);


        // ロードが完了
        if (owner->loadingScene_->IsReady())
        {
            owner->loadingWordSprite_->GetSpriteTransform()->SetColorA(0.0f);
            owner->loadArrowSprite_->GetSpriteTransform()->SetColorA(0.0f);

            // フェードイン
            float maxTime = 0.5f;
            if (easingTimer_ <= maxTime)
            {
                float alpha = Easing::InSine(easingTimer_, maxTime, 1.0f, 0.0f);

                owner->halfBlackBeltSprite_->GetSpriteTransform()->SetColorA(alpha);
                owner->completeWordSprite_->GetSpriteTransform()->SetColorA(alpha);

                easingTimer_ += elapsedTime;
            }
            // フェードイン完了
            else
            {   
                // 入力待ち
                if ((gamePad.GetButtonDown() & GamePad::BTN_A) ||
                    (mouse.GetButtonDown() & Mouse::BTN_LEFT))
                {
                    owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneLoadGame::STATE::SlideUiFadeOut));
                    return;
                }

                owner->halfBlackBeltSprite_->GetSpriteTransform()->SetColorA(1.0f);
                owner->completeWordSprite_->GetSpriteTransform()->SetColorA(1.0f);
            }
        }


    }

    // 終了化
    void LoadGameState::Finalize()
    {
    }
}

// LoadTitleState
namespace SceneLoadGameState
{
    // 初期化
    void LoadTitleState::Initialize()
    {
        // 現在のステートを設定
        owner->SetCurrentState(SceneLoadGame::STATE::LoadTitle);

        // 変数初期化
        slideTimer_ = 0.0f;
        easingTimer_ = 0.0f;
        isFadeOut_ = false;
    }

    // 更新
    void LoadTitleState::Update(const float& elapsedTime)
    {
        float maxSlideTime = 0.2f;
        float beforePosX = 125.0f;
        float afterPosX = -1000.0f;
        
        if (!isFadeOut_)
        {
            if (slideTimer_ <= maxSlideTime)
            {
                float spritePosX = Easing::InSine(slideTimer_, maxSlideTime, afterPosX, beforePosX);

                for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
                {
                    owner->gameDataBaseSprite_[i]->GetSpriteTransform()->SetPosX(spritePosX);
                    owner->gameDataWordSprite_[i]->GetSpriteTransform()->SetPosX(spritePosX);
                }

                owner->gameDataChoseSprite_->GetSpriteTransform()->SetPosX(spritePosX);

                float bookSpritePosX = Easing::InSine(slideTimer_, maxSlideTime, afterPosX - beforePosX, beforePosX - beforePosX);
                owner->bookSprite_->GetSpriteTransform()->SetPosX(bookSpritePosX);
                owner->choseLoadDataWordSprite_->GetSpriteTransform()->SetPosX(bookSpritePosX);

                float pointPosX = Easing::InSine(slideTimer_, maxSlideTime, afterPosX - 20.0f, beforePosX - 20.0f);
                owner->pointWakuSprite_->GetSpriteTransform()->SetPosX(pointPosX);
                owner->pointRhombusSprite_->GetSpriteTransform()->SetPosX(pointPosX);

                slideTimer_ += elapsedTime;
            }
            else
            {
                // Spriteを指定の位置に設定
                for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
                {
                    owner->gameDataBaseSprite_[i]->GetSpriteTransform()->SetPosX(afterPosX);
                    owner->gameDataWordSprite_[i]->GetSpriteTransform()->SetPosX(afterPosX);
                }
                owner->gameDataChoseSprite_->GetSpriteTransform()->SetPosX(afterPosX);

                owner->bookSprite_->GetSpriteTransform()->SetPosX(afterPosX - beforePosX);
                owner->choseLoadDataWordSprite_->GetSpriteTransform()->SetPosX(afterPosX - beforePosX);

                owner->pointWakuSprite_->GetSpriteTransform()->SetPosX(afterPosX - 20.0f);
                owner->pointRhombusSprite_->GetSpriteTransform()->SetPosX(afterPosX - 20.0f);

                isFadeOut_ = true;
            }
        }

        // フェードアウト
        if (isFadeOut_)
        {
            float maxTime = 0.2f;
            if (easingTimer_ <= maxTime)
            {
                float alpha = Easing::InSine(easingTimer_, maxTime, 1.0f, 0.0f);
                owner->whiteSprite_->GetSpriteTransform()->SetColorA(alpha);

                easingTimer_ += elapsedTime;
            }
            else
            {
                owner->whiteSprite_->GetSpriteTransform()->SetColorA(1.0f);

                // シーン切り替え
                SceneManager::Instance().ChangeScene(new SceneLoadingBack(new SceneTitle(new SceneLoadGame)));
                return;
            }
        }
    }

    // 終了化
    void LoadTitleState::Finalize()
    {
    }
}

// ChangeSceneState
namespace SceneLoadGameState
{
    // 初期化
    void ChangeSceneState::Initialize()
    {
        // 現在のステートを設定
        owner->SetCurrentState(SceneLoadGame::STATE::ChangeScene);
    }

    // 更新
    void ChangeSceneState::Update(const float& elapsedTime)
    {
        owner->ChangeScene();
    }

    // 終了化
    void ChangeSceneState::Finalize()
    {
    }
}