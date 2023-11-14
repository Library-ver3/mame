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

        float startAddPosX = -1225.0f;
        float endAddPosX = 0.0f;

        if (easingTimer_ <= maxTime)
        {
            // 125 ~ 1225 - 125
            {
                // ゲームデータの外枠と選択用画像
                float finalGameDataBasePosX = 125.0f;
                float gameDataBasePosX = Easing::InSine(easingTimer_, maxTime, endAddPosX - finalGameDataBasePosX, startAddPosX - finalGameDataBasePosX);

                for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
                {
                    owner->gameDataBaseSprite_[i]->GetSpriteTransform()->SetPosX(gameDataBasePosX);
                    owner->gameDataWordSprite_[i]->GetSpriteTransform()->SetPosX(gameDataBasePosX);
                }

                owner->gameDataChoseSprite_->GetSpriteTransform()->SetPosX(gameDataBasePosX);
            }

            // -1225 ~ 0
            {
                float bookSpritePosX = Easing::InSine(easingTimer_, maxTime, endAddPosX, startAddPosX);
                
                // 背景の本の画像
                owner->bookSprite_->GetSpriteTransform()->SetPosX(bookSpritePosX);
                
                // 文字
                owner->choseLoadDataWordSprite_->GetSpriteTransform()->SetPosX(bookSpritePosX);
            }

            // 105 ~ 1255 - 145
            {
                float finalPointPosX = 105.0f;
                float pointPosX = Easing::InSine(easingTimer_, maxTime, endAddPosX - finalPointPosX, startAddPosX - finalPointPosX);
                owner->pointWakuSprite_->GetSpriteTransform()->SetPosX(pointPosX);
                owner->pointRhombusSprite_->GetSpriteTransform()->SetPosX(pointPosX);
            }

            // [01,02,03 文字]
            {
                float finalNumPosX[3] = { 260.0f, 290.0f, 245.0f };
                for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
                {
                    float numPosX = Easing::InSine(easingTimer_, maxTime, endAddPosX + finalNumPosX[i], startAddPosX + finalNumPosX[i]);
                    owner->gameDataNumSprite_[i]->GetSpriteTransform()->SetPosX(numPosX);
                }                
            }

            easingTimer_ += elapsedTime;
        }
        else
        {
            // 125
            {
                float finalGameDataBasePosX = 125.0f;
                for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
                {
                    owner->gameDataBaseSprite_[i]->GetSpriteTransform()->SetPosX(finalGameDataBasePosX);
                    owner->gameDataWordSprite_[i]->GetSpriteTransform()->SetPosX(finalGameDataBasePosX);
                }
                owner->gameDataChoseSprite_->GetSpriteTransform()->SetPosX(finalGameDataBasePosX);
            }

            // 0
            {
                float finalPosX = 0.0f;
                owner->bookSprite_->GetSpriteTransform()->SetPosX(finalPosX);
                owner->choseLoadDataWordSprite_->GetSpriteTransform()->SetPosX(finalPosX);
            }

            // 105
            {
                float finalPointPosX = 105.0f;
                owner->pointWakuSprite_->GetSpriteTransform()->SetPosX(finalPointPosX);
                owner->pointRhombusSprite_->GetSpriteTransform()->SetPosX(finalPointPosX);
            }

            // [01,02,03 文字]
            {
                float finalNumPosX[3] = { 260.0f, 290.0f, 245.0f };
                for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
                {
                    owner->gameDataNumSprite_[i]->GetSpriteTransform()->SetPosX(endAddPosX + finalNumPosX[i]);
                }
            }

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
        Mouse& mouse = Input::Instance().GetMouse();

        // タイトルに戻る
        if ((gamePad.GetButtonDown() & GamePad::BTN_B)
            || mouse.GetButtonDown() & Mouse::BTN_RIGHT)
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
        
        currentState_ = STATE::Yes;
        oldCurrentState_ = STATE::Yes;
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

            // ボタン押されるのまってる。( はい、いいえ )
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

        Mouse& mouse = Input::Instance().GetMouse();
        DirectX::XMFLOAT2 mousePos = { static_cast<float>(mouse.GetPositionX()), static_cast<float>(mouse.GetPositionY()) };

        oldCurrentState_ = currentState_;

#pragma region マウスによる選択
        // ---------- マウスのカーソルの位置によってステートを変更 ----------

        // --- はい を選択 ---
        if (currentState_ == STATE::Yes)
        {   
            // 右クリックしたら No に飛ばす
            if (mouse.GetButtonDown() & Mouse::BTN_RIGHT)
            {
                ChangeCurrentState(STATE::No);
            }
        }
        if ((mousePos.x > mousePos_[0].left_) && (mousePos.x < mousePos_[0].right_) && (mousePos.y > mousePos_[0].top_) && (mousePos.y < mousePos_[0].bottom_))
        {
            ChangeCurrentState(STATE::Yes);
        }

        // --- いいえ を選択 ---
        if ((mousePos.x > mousePos_[1].left_) && (mousePos.x < mousePos_[1].right_) && (mousePos.y > mousePos_[1].top_) && (mousePos.y < mousePos_[1].bottom_))
        {
            ChangeCurrentState(STATE::No);
        }

#pragma endregion// マウスによる選択

#pragma region ゲームパッドによる選択 (keyboard)
        // ---------- 入力によるステート変更 ----------

        if (currentState_ == STATE::Yes)
        {   // [はい] にカーソルがある
            // --- Bボタンを押したら [いいえ] に飛ばす
            if (gamePad.GetButtonDown() & GamePad::BTN_B)
            {
                ChangeCurrentState(STATE::No);
            }

            // --- スティックの傾きによるステート変更 ---
            if (aLx > 0.8f)
            {
                ChangeCurrentState(STATE::No);
            }
        }

        if(currentState_ == STATE::No)
        {   // [いいえ] にカーソルがある
            // --- スティックの傾きによるステート変更 ---
            if (aLx < -0.8f)
            {
                ChangeCurrentState(STATE::Yes);
            }
        }
#pragma endregion// ゲームパッドによる選択 (keyboard)
    }

    // 選択カーソルの更新
    bool LoadGameChoseState::ChoseSpriteUpdate(const float& elapsedTime)
    {
        float maxTime = 0.2f;
        if (easingTimer_ <= maxTime)
        {   // 選択UIを徐々に大きくする
            float scale = Easing::InSine(easingTimer_, maxTime, 100.0f, 0.0f);

            // 位置設定
            if (currentState_ == STATE::Yes)
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

        Mouse& mouse = Input::Instance().GetMouse();
        DirectX::XMFLOAT2 mousePos = { static_cast<float>(mouse.GetPositionX()), static_cast<float>(mouse.GetPositionY()) };

#pragma region マウスによる選択
        // ---------- マウスクリックによる選択 ----------

        // --- はい を選択 ---
        if (((mousePos.x > mousePos_[0].left_) && (mousePos.x < mousePos_[0].right_) && (mousePos.y > mousePos_[0].top_) && (mousePos.y < mousePos_[0].bottom_))
            && (mouse.GetButtonDown() & Mouse::BTN_LEFT))
        {   // ゲームを読み込む                
            // 最終確認用Spriteを消す
            owner->blackBeltSprite_->GetSpriteTransform()->SetColorA(0.0f);
            owner->loadGameWordSprite_->GetSpriteTransform()->SetColorA(0.0f);
            owner->choseSprite_->GetSpriteTransform()->SetColorA(0.0f);

            // 次のステートへ
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneLoadGame::STATE::LoadGame));
            return;
        }

        // --- いいえ を選択 ---
        if (((mousePos.x > mousePos_[1].left_) && (mousePos.x < mousePos_[1].right_) && (mousePos.y > mousePos_[1].top_) && (mousePos.y < mousePos_[1].bottom_))
            && (mouse.GetButtonDown() & Mouse::BTN_LEFT))
        {   // selectに戻る
            // 最終確認用Spriteを消す
            owner->blackBeltSprite_->GetSpriteTransform()->SetColorA(0.0f);
            owner->loadGameWordSprite_->GetSpriteTransform()->SetColorA(0.0f);
            owner->choseSprite_->GetSpriteTransform()->SetColorA(0.0f);

            // 次のステートへ
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneLoadGame::STATE::Select));
            return;
        }

#pragma endregion// マウスによる選択

#pragma region ゲームパッドによる選択 (keyboard)
        // ---------- ボタン入力による選択 ----------

        // --- はい を選択 ---
        if (currentState_ == STATE::Yes)
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
            }
        }

        // --- いいえ を選択 ---]
        if (currentState_ == STATE::No)
        {
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

#pragma endregion// ゲームパッドによる選択 (keyboard)
    }

    // ステート変更
    void LoadGameChoseState::ChangeCurrentState(STATE state)
    {
        // 今選択しているものと、新しく選択したものが同じなら処理しない
        currentState_ = state;
        if (currentState_ == oldCurrentState_) return;

        easingTimer_ = 0.0f;
        owner->choseSprite_->ResetAnimation();
        owner->choseSprite_->GetSpriteTransform()->SetSize(0.0f);
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

        float endAddPosX = -1225.0f;
        float startAddPosX = 0.0f;
        
        if (!isFadeOut_)
        {
            if (slideTimer_ <= maxSlideTime)
            {
                // 125 ~ 1225 - 125
                {
                    // ゲームデータの外枠と選択用画像
                    float finalGameDataBasePosX = 125.0f;
                    float spritePosX = Easing::InSine(slideTimer_, maxSlideTime, endAddPosX - finalGameDataBasePosX, startAddPosX - finalGameDataBasePosX);

                    for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
                    {
                        owner->gameDataBaseSprite_[i]->GetSpriteTransform()->SetPosX(spritePosX);
                        owner->gameDataWordSprite_[i]->GetSpriteTransform()->SetPosX(spritePosX);
                    }

                    owner->gameDataChoseSprite_->GetSpriteTransform()->SetPosX(spritePosX);
                }

                // -1225 ~ 0
                {
                    float bookSpritePosX = Easing::InSine(slideTimer_, maxSlideTime, endAddPosX, startAddPosX);
                    owner->bookSprite_->GetSpriteTransform()->SetPosX(bookSpritePosX);
                    owner->choseLoadDataWordSprite_->GetSpriteTransform()->SetPosX(bookSpritePosX);
                }

                // 105 ~ 1255 - 145
                {
                    float finalPointPosX = 105.0f;
                    float pointPosX = Easing::InSine(slideTimer_, maxSlideTime, endAddPosX - finalPointPosX, startAddPosX - finalPointPosX);
                    owner->pointWakuSprite_->GetSpriteTransform()->SetPosX(pointPosX);
                    owner->pointRhombusSprite_->GetSpriteTransform()->SetPosX(pointPosX);
                }

                // [01,02,03 文字]
                {
                    float finalNumPosX[3] = { 260.0f, 290.0f, 245.0f };
                    for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
                    {
                        float numPosX = Easing::InSine(slideTimer_, maxSlideTime, endAddPosX + finalNumPosX[i], startAddPosX + finalNumPosX[i]);
                        owner->gameDataNumSprite_[i]->GetSpriteTransform()->SetPosX(numPosX);
                    }
                }

                slideTimer_ += elapsedTime;
            }
            else
            {
                // Spriteを指定の位置に設定
                // 125
                {
                    float finalGameDataBasePosX = 125.0f;
                    for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
                    {
                        owner->gameDataBaseSprite_[i]->GetSpriteTransform()->SetPosX(endAddPosX - finalGameDataBasePosX);
                        owner->gameDataWordSprite_[i]->GetSpriteTransform()->SetPosX(endAddPosX - finalGameDataBasePosX);
                    }
                    owner->gameDataChoseSprite_->GetSpriteTransform()->SetPosX(endAddPosX - finalGameDataBasePosX);
                }

                // 0
                {
                    owner->bookSprite_->GetSpriteTransform()->SetPosX(endAddPosX);
                    owner->choseLoadDataWordSprite_->GetSpriteTransform()->SetPosX(endAddPosX);
                }

                // 105
                {
                    float finalPointPosX = 105.0f;
                    owner->pointWakuSprite_->GetSpriteTransform()->SetPosX(endAddPosX - finalPointPosX);
                    owner->pointRhombusSprite_->GetSpriteTransform()->SetPosX(endAddPosX - finalPointPosX);
                }

                // [01,02,03 文字]
                {
                    float finalNumPosX[3] = { 260.0f, 290.0f, 245.0f };
                    for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
                    {
                        owner->gameDataNumSprite_[i]->GetSpriteTransform()->SetPosX(endAddPosX + finalNumPosX[i]);
                    }
                }

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

        float endAddPosX = -1225.0f;
        float startAddPosX = 0.0f;

        if (slideTimer_ <= maxSlideTime)
        {
            // 125 ~ 1225 - 125
            {
                // ゲームデータの外枠と選択用画像
                float finalGameDataBasePosX = 125.0f;
                float spritePosX = Easing::InSine(slideTimer_, maxSlideTime, endAddPosX - finalGameDataBasePosX, startAddPosX - finalGameDataBasePosX);

                for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
                {
                    owner->gameDataBaseSprite_[i]->GetSpriteTransform()->SetPosX(spritePosX);
                    owner->gameDataWordSprite_[i]->GetSpriteTransform()->SetPosX(spritePosX);
                }

                owner->gameDataChoseSprite_->GetSpriteTransform()->SetPosX(spritePosX);
            }

            // -1225 ~ 0
            {
                float bookSpritePosX = Easing::InSine(slideTimer_, maxSlideTime, endAddPosX, startAddPosX);
                owner->bookSprite_->GetSpriteTransform()->SetPosX(bookSpritePosX);
                owner->choseLoadDataWordSprite_->GetSpriteTransform()->SetPosX(bookSpritePosX);
            }

            // 105 ~ 1255 - 145
            {
                float finalPointPosX = 105.0f;
                float pointPosX = Easing::InSine(slideTimer_, maxSlideTime, endAddPosX - finalPointPosX, startAddPosX - finalPointPosX);
                owner->pointWakuSprite_->GetSpriteTransform()->SetPosX(pointPosX);
                owner->pointRhombusSprite_->GetSpriteTransform()->SetPosX(pointPosX);
            }

            // [01,02,03 文字]
            {
                float finalNumPosX[3] = { 260.0f, 290.0f, 245.0f };
                for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
                {
                    float numPosX = Easing::InSine(slideTimer_, maxSlideTime, endAddPosX + finalNumPosX[i], startAddPosX + finalNumPosX[i]);
                    owner->gameDataNumSprite_[i]->GetSpriteTransform()->SetPosX(numPosX);
                }
            }

            slideTimer_ += elapsedTime;
        }
        else
        {
            // Spriteを指定の位置に設定

            // 125
            {
                float finalGameDataBasePosX = 125.0f;
                for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
                {
                    owner->gameDataBaseSprite_[i]->GetSpriteTransform()->SetPosX(endAddPosX - finalGameDataBasePosX);
                    owner->gameDataWordSprite_[i]->GetSpriteTransform()->SetPosX(endAddPosX - finalGameDataBasePosX);
                }
                owner->gameDataChoseSprite_->GetSpriteTransform()->SetPosX(endAddPosX - finalGameDataBasePosX);
            }

            // 0
            {
                owner->bookSprite_->GetSpriteTransform()->SetPosX(endAddPosX);
                owner->choseLoadDataWordSprite_->GetSpriteTransform()->SetPosX(endAddPosX);
            }

            // 105
            {
                float finalPointPosX = 105.0f;
                owner->pointWakuSprite_->GetSpriteTransform()->SetPosX(endAddPosX - finalPointPosX);
                owner->pointRhombusSprite_->GetSpriteTransform()->SetPosX(endAddPosX - finalPointPosX);
            }

            // [01,02,03 文字]
            {
                float finalNumPosX[3] = { 260.0f, 290.0f, 245.0f };
                for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
                {
                    owner->gameDataNumSprite_[i]->GetSpriteTransform()->SetPosX(endAddPosX + finalNumPosX[i]);
                }
            }

            isFadeOut_ = true;
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