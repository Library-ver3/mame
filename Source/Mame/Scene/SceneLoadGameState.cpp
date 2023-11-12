#include "SceneLoadGameState.h"

#include "../Other/Easing.h"

#include "../Input/Input.h"

#include "../Scene/SceneManager.h"
#include "../Scene/SceneLoading.h"
#include "../Scene/SceneGame.h"
#include "../Scene/SceneLoadingBack.h"
#include "../Scene/SceneTitle.h"

// SlideUiState
namespace SceneLoadGameState
{
    // ������
    void SlideUiState::Initialize()
    {
        // ���݂̃X�e�[�g��ݒ�
        owner->SetCurrentState(SceneLoadGame::STATE::SlideUi);

        // �ϐ�������
        easingTimer_ = 0.0f;
    }

    // �X�V
    void SlideUiState::Update(const float& elapsedTime)
    {
        float maxTime = 0.5f;
        float startGameDataBase = -150.0f;
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

            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneLoadGame::STATE::Select));
            return;
        }
    }

    // �I����
    void SlideUiState::Finalize()
    {
    }
}

// SelectState
namespace SceneLoadGameState
{
    // ������
    void SelectState::Initialize()
    {
        // sprite���Z�b�g
        owner->gameDataChoseSprite_->GetSpriteDissolve()->SetDissolveValue(0.0f);
        owner->gameDataChoseSprite_->GetSpriteTransform()->SetColorA(1.0f);

        // �ϐ����Z�b�g
        gameDataNum_ = GameData::One;
        isFadeSprite_ = false;
        fadeSpriteTimer_ = 0.0f;
    }

    // �X�V
    void SelectState::Update(const float& elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();

        // �^�C�g���ɖ߂�
        if (gamePad.GetButtonDown() & GamePad::BTN_B)
        {
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneLoadGame::STATE::LoadTitle));
            return;
        }

        // �Q�[���f�[�^��I��
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

        // ���̃X�e�[�g��
        if (gamePad.GetButtonDown() & GamePad::BTN_A)
        {
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneLoadGame::STATE::LoadGameChose));
            return;
        }
    }

    // �I����
    void SelectState::Finalize()
    {

    }

    // �Q�[���f�[�^�I��
    void SelectState::UpdateSelectGameData(const float& elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();

        float aLY = gamePad.GetAxisLY();
        float stick = 0.8f;

        switch (gameDataNum_)
        {
        case GameData::One:
            // ���ݑI��ł���Q�[���f�[�^��ۑ�
            owner->SetSelectState(static_cast<UINT>(gameDataNum_));


            if ((aLY < -stick) && isChangeState_
                && (maxGameDataNum_ > GameData::One))
            {   // �X�e�B�b�N�����ɌX����
                isChangeState_ = false;
                isFadeSprite_ = true;
                fadeSpriteTimer_ = 0.0f;
                gameDataNum_ = GameData::Two;
                break;
            }

            owner->gameDataChoseSprite_->GetSpriteTransform()->SetPos(owner->spritePos[0]);

            break;
        case GameData::Two:
            // ���ݑI��ł���Q�[���f�[�^��ۑ�
            owner->SetSelectState(static_cast<UINT>(gameDataNum_));

            if ((aLY < -stick) && isChangeState_
                && (maxGameDataNum_ > GameData::Two))
            {   // �X�e�B�b�N�����ɌX����
                isChangeState_ = false;
                isFadeSprite_ = true;
                fadeSpriteTimer_ = 0.0f;
                gameDataNum_ = GameData::Three;
                break;
            }

            if ((aLY > stick) && isChangeState_)
            {   // �X�e�B�b�N����ɌX����
                isChangeState_ = false;
                isFadeSprite_ = true;
                fadeSpriteTimer_ = 0.0f;
                gameDataNum_ = GameData::One;
                break;
            }

            owner->gameDataChoseSprite_->GetSpriteTransform()->SetPos(owner->spritePos[1]);

            break;
        case GameData::Three:
            // ���ݑI��ł���Q�[���f�[�^��ۑ�
            owner->SetSelectState(static_cast<UINT>(gameDataNum_));

            if ((aLY > stick) && isChangeState_)
            {   // �X�e�B�b�N����ɌX����
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

    // �A�����đI�����Ă��܂�����X�g���X�Ȃ̂ŁAbreakTime������
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
    // ������
    void LoadGameChoseState::Initialize()
    {
        // ���݂̃X�e�[�g��ݒ�
        owner->SetCurrentState(SceneLoadGame::STATE::LoadGameChose);

        // Sprite������
        owner->blackBeltSprite_->GetSpriteTransform()->SetColorA(0.0f);
        owner->loadGameWordSprite_->GetSpriteTransform()->SetColorA(0.0f);
        owner->choseSprite_->GetSpriteTransform()->SetSize(0.0f);
        owner->choseSprite_->GetSpriteTransform()->SetColorA(1.0f);

        // �ϐ�������
        isChose_ = false;
        
        currentState_ = 0;
        easingTimer_ = 0.0f;
        animationTimer_ = 0.0f;
    }

    // �X�V
    void LoadGameChoseState::Update(const float& elapsedTime)
    {
        if (!isChose_)
        {   // UI �t�F�C�h�C��
            FadeInUI(elapsedTime);
        }
        else
        {   // �I�� ( �Q�[���ǂݍ��� or �߂� )
            // �͂� or ������ ��I������
            ChoseYesOrNo(elapsedTime);

            // �I���J�[�\���X�V�B
            if (ChoseSpriteUpdate(elapsedTime))
            {
                // �A�j���[�V�����Đ��\�Ȃ�Đ��B
                owner->choseSprite_->PlayAnimation(elapsedTime, 15.0f, 3, false);
            }


                PushButton();
        }

    }

    // �I����
    void LoadGameChoseState::Finalize()
    {
    }

    // UI �t�F�C�h�C��
    void LoadGameChoseState::FadeInUI(const float& elapsedTime)
    {
        float maxTime = 0.4f;
        float maxAlpha = 0.91f;
        if (easingTimer_ <= maxTime)
        {   // �����ƍ����т��t�F�C�h�C������
            float alpha = Easing::InSine(easingTimer_, maxTime, maxAlpha, 0.0f);
            float wordAlpha = Easing::InSine(easingTimer_, maxTime, 1.0f, 0.0f);

            owner->blackBeltSprite_->GetSpriteTransform()->SetColorA(alpha);
            owner->loadGameWordSprite_->GetSpriteTransform()->SetColorA(wordAlpha);

            easingTimer_ += elapsedTime;
        }
        else
        {
            // �J���[��ݒ肷��
            owner->blackBeltSprite_->GetSpriteTransform()->SetColorA(maxAlpha);
            owner->loadGameWordSprite_->GetSpriteTransform()->SetColorA(1.0f);

            easingTimer_ = 0.0f;
            isChose_ = true;
            return;
        }
    }
    
    // �͂� �� ������ ��I������
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
            {   // �X�e�B�b�N���E�ɌX�����B
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
            {   // �X�e�B�b�N�����ɌX�����B
                easingTimer_ = 0.0f;
                owner->choseSprite_->ResetAnimation();
                owner->choseSprite_->GetSpriteTransform()->SetSize(0.0f);

                currentState_ = static_cast<UINT>(STATE::Yes);
                return;
            }
        }
    }

    // �I���J�[�\���̍X�V
    bool LoadGameChoseState::ChoseSpriteUpdate(const float& elapsedTime)
    {
        float maxTime = 0.2f;
        if (easingTimer_ <= maxTime)
        {   // �I��UI�����X�ɑ傫������
            float scale = Easing::InSine(easingTimer_, maxTime, 100.0f, 0.0f);

            // �ʒu�ݒ�
            if (currentState_ == static_cast<UINT>(STATE::Yes))
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

            // max�ɓ��B���Ă�����A�j���[�V�����Đ��\
            return true;
        }

        // �A�j���[�V�����Đ��s��
        return false;
    }

    // �{�^���������̂܂��Ă�B
    void LoadGameChoseState::PushButton()
    {
        GamePad& gamePad = Input::Instance().GetGamePad();
        if (currentState_ == static_cast<UINT>(STATE::Yes))
        {   // �Q�[����ǂݍ���
            if (gamePad.GetButtonDown() & GamePad::BTN_A)
            {
                // �ŏI�m�F�pSprite������
                owner->blackBeltSprite_->GetSpriteTransform()->SetColorA(0.0f);
                owner->loadGameWordSprite_->GetSpriteTransform()->SetColorA(0.0f);
                owner->choseSprite_->GetSpriteTransform()->SetColorA(0.0f);

                // ���̃X�e�[�g��
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneLoadGame::STATE::SlideUiFadeOut));
                return;

#if 0
                // �ǂ̃Q�[���f�[�^�ɔ�Ԃ�
                switch (owner->GetSelectState())
                {
                case static_cast<UINT>(SceneLoadGame::GameDataBase::Top):
                    // ��ڂ̃f�[�^

                    break;
                case static_cast<UINT>(SceneLoadGame::GameDataBase::Middle):
                    // ��ڂ̃f�[�^
                    break;
                case static_cast<UINT>(SceneLoadGame::GameDataBase::Bottom):
                    // �O�ڂ̃f�[�^
                    break;
                }
#endif
            }
        }
        else
        {   // �߂�
            if (gamePad.GetButtonDown() & GamePad::BTN_A)
            {
                // �ŏI�m�F�pSprite������
                owner->blackBeltSprite_->GetSpriteTransform()->SetColorA(0.0f);
                owner->loadGameWordSprite_->GetSpriteTransform()->SetColorA(0.0f);
                owner->choseSprite_->GetSpriteTransform()->SetColorA(0.0f);

                // ���̃X�e�[�g��
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneLoadGame::STATE::Select));
                return;
            }
        }
    }
}

// SlideUiFadeOutState
namespace SceneLoadGameState
{
    // ������
    void SlideUiFadeOutState::Initialize()
    {

        // �ϐ�������
        easingTimer_ = 0.0f;
    }

    // �X�V
    void SlideUiFadeOutState::Update(const float& elapsedTime)
    {
        float maxTime = 0.5f;
        float beforePosX = 125.0f;
        float afterPosX = -1000.0f;
        
        if (easingTimer_ <= maxTime)
        {
            float spritePosX = Easing::InSine(easingTimer_, maxTime, afterPosX, beforePosX);

            for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
            {
                owner->gameDataBaseSprite_[i]->GetSpriteTransform()->SetPosX(spritePosX);
                owner->gameDataWordSprite_[i]->GetSpriteTransform()->SetPosX(spritePosX);
            }

            owner->gameDataChoseSprite_->GetSpriteTransform()->SetPosX(spritePosX);

            float bookSpritePosX = Easing::InSine(easingTimer_, maxTime, afterPosX - beforePosX, beforePosX - beforePosX);
            owner->bookSprite_->GetSpriteTransform()->SetPosX(bookSpritePosX);

            // �t�F�[�h�A�E�g
            float alpha = Easing::InSine(easingTimer_, maxTime, 1.0f, 0.0f);
            owner->whiteSprite_->GetSpriteTransform()->SetColorA(alpha);

            easingTimer_ += elapsedTime;
        }
        else
        {
            // Sprite���w��̈ʒu�ɐݒ�
            for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
            {
                owner->gameDataBaseSprite_[i]->GetSpriteTransform()->SetPosX(afterPosX);
                owner->gameDataWordSprite_[i]->GetSpriteTransform()->SetPosX(afterPosX);
            }
            owner->gameDataChoseSprite_->GetSpriteTransform()->SetPosX(afterPosX);
            
            owner->bookSprite_->GetSpriteTransform()->SetPosX(afterPosX - beforePosX);

            // ���̃X�e�[�g��
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneLoadGame::STATE::LoadGame));
        }

    }

    // �I����
    void SlideUiFadeOutState::Finalize()
    {
    }
}

// LoadGameState
namespace SceneLoadGameState
{
    // ������
    void LoadGameState::Initialize()
    {        
    }

    // �X�V
    void LoadGameState::Update(const float& elapsedTime)
    {
        switch (owner->GetSelectState())
        {
        case 0:
            owner->ChangeScene();
            //SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
            break;
        case 1:
            break;
        case 2:
            break;
        }
    }

    // �I����
    void LoadGameState::Finalize()
    {
    }
}

// LoadTitleState
namespace SceneLoadGameState
{
    // ������
    void LoadTitleState::Initialize()
    {
        // �ϐ�������
        easingTimer_ = 0.0f;
    }

    // �X�V
    void LoadTitleState::Update(const float& elapsedTime)
    {
        float maxTime = 0.5f;
        float beforePosX = 125.0f;
        float afterPosX = -1000.0f;

        if (easingTimer_ <= maxTime)
        {
            float spritePosX = Easing::InSine(easingTimer_, maxTime, afterPosX, beforePosX);

            for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
            {
                owner->gameDataBaseSprite_[i]->GetSpriteTransform()->SetPosX(spritePosX);
                owner->gameDataWordSprite_[i]->GetSpriteTransform()->SetPosX(spritePosX);
            }

            owner->gameDataChoseSprite_->GetSpriteTransform()->SetPosX(spritePosX);

            float bookSpritePosX = Easing::InSine(easingTimer_, maxTime, afterPosX - beforePosX, beforePosX - beforePosX);
            owner->bookSprite_->GetSpriteTransform()->SetPosX(bookSpritePosX);

            // �t�F�[�h�A�E�g
            float alpha = Easing::InSine(easingTimer_, maxTime, 1.0f, 0.0f);
            owner->whiteSprite_->GetSpriteTransform()->SetColorA(alpha);

            easingTimer_ += elapsedTime;
        }
        else
        {
            // Sprite���w��̈ʒu�ɐݒ�
            for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
            {
                owner->gameDataBaseSprite_[i]->GetSpriteTransform()->SetPosX(afterPosX);
                owner->gameDataWordSprite_[i]->GetSpriteTransform()->SetPosX(afterPosX);
            }
            owner->gameDataChoseSprite_->GetSpriteTransform()->SetPosX(afterPosX);

            owner->bookSprite_->GetSpriteTransform()->SetPosX(afterPosX - beforePosX);

            // �V�[���؂�ւ�
            SceneManager::Instance().ChangeScene(new SceneLoadingBack(new SceneTitle(new SceneLoadGame)));
            return;
        }
    }

    // �I����
    void LoadTitleState::Finalize()
    {
    }
}