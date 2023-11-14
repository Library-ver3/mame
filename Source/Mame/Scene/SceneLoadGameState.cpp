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
        float maxTime = 0.4f;

        float startAddPosX = -1225.0f;
        float endAddPosX = 0.0f;

        if (easingTimer_ <= maxTime)
        {
            // 125 ~ 1225 - 125
            {
                // �Q�[���f�[�^�̊O�g�ƑI��p�摜
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
                
                // �w�i�̖{�̉摜
                owner->bookSprite_->GetSpriteTransform()->SetPosX(bookSpritePosX);
                
                // ����
                owner->choseLoadDataWordSprite_->GetSpriteTransform()->SetPosX(bookSpritePosX);
            }

            // 105 ~ 1255 - 145
            {
                float finalPointPosX = 105.0f;
                float pointPosX = Easing::InSine(easingTimer_, maxTime, endAddPosX - finalPointPosX, startAddPosX - finalPointPosX);
                owner->pointWakuSprite_->GetSpriteTransform()->SetPosX(pointPosX);
                owner->pointRhombusSprite_->GetSpriteTransform()->SetPosX(pointPosX);
            }

            // [01,02,03 ����]
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

            // [01,02,03 ����]
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
        // ���݂̃X�e�[�g��ݒ�
        owner->SetCurrentState(SceneLoadGame::STATE::Select);

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
        Mouse& mouse = Input::Instance().GetMouse();

        // �^�C�g���ɖ߂�
        if ((gamePad.GetButtonDown() & GamePad::BTN_B)
            || mouse.GetButtonDown() & Mouse::BTN_RIGHT)
        {
            owner->loadingScene_ = (new SceneLoading(new SceneDummy));
            owner->thread_ = new std::thread(owner->LoadingThread, owner);
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
        
        currentState_ = STATE::Yes;
        oldCurrentState_ = STATE::Yes;
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

            // �{�^���������̂܂��Ă�B( �͂��A������ )
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

        Mouse& mouse = Input::Instance().GetMouse();
        DirectX::XMFLOAT2 mousePos = { static_cast<float>(mouse.GetPositionX()), static_cast<float>(mouse.GetPositionY()) };

        oldCurrentState_ = currentState_;

#pragma region �}�E�X�ɂ��I��
        // ---------- �}�E�X�̃J�[�\���̈ʒu�ɂ���ăX�e�[�g��ύX ----------

        // --- �͂� ��I�� ---
        if (currentState_ == STATE::Yes)
        {   
            // �E�N���b�N������ No �ɔ�΂�
            if (mouse.GetButtonDown() & Mouse::BTN_RIGHT)
            {
                ChangeCurrentState(STATE::No);
            }
        }
        if ((mousePos.x > mousePos_[0].left_) && (mousePos.x < mousePos_[0].right_) && (mousePos.y > mousePos_[0].top_) && (mousePos.y < mousePos_[0].bottom_))
        {
            ChangeCurrentState(STATE::Yes);
        }

        // --- ������ ��I�� ---
        if ((mousePos.x > mousePos_[1].left_) && (mousePos.x < mousePos_[1].right_) && (mousePos.y > mousePos_[1].top_) && (mousePos.y < mousePos_[1].bottom_))
        {
            ChangeCurrentState(STATE::No);
        }

#pragma endregion// �}�E�X�ɂ��I��

#pragma region �Q�[���p�b�h�ɂ��I�� (keyboard)
        // ---------- ���͂ɂ��X�e�[�g�ύX ----------

        if (currentState_ == STATE::Yes)
        {   // [�͂�] �ɃJ�[�\��������
            // --- B�{�^������������ [������] �ɔ�΂�
            if (gamePad.GetButtonDown() & GamePad::BTN_B)
            {
                ChangeCurrentState(STATE::No);
            }

            // --- �X�e�B�b�N�̌X���ɂ��X�e�[�g�ύX ---
            if (aLx > 0.8f)
            {
                ChangeCurrentState(STATE::No);
            }
        }

        if(currentState_ == STATE::No)
        {   // [������] �ɃJ�[�\��������
            // --- �X�e�B�b�N�̌X���ɂ��X�e�[�g�ύX ---
            if (aLx < -0.8f)
            {
                ChangeCurrentState(STATE::Yes);
            }
        }
#pragma endregion// �Q�[���p�b�h�ɂ��I�� (keyboard)
    }

    // �I���J�[�\���̍X�V
    bool LoadGameChoseState::ChoseSpriteUpdate(const float& elapsedTime)
    {
        float maxTime = 0.2f;
        if (easingTimer_ <= maxTime)
        {   // �I��UI�����X�ɑ傫������
            float scale = Easing::InSine(easingTimer_, maxTime, 100.0f, 0.0f);

            // �ʒu�ݒ�
            if (currentState_ == STATE::Yes)
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

        Mouse& mouse = Input::Instance().GetMouse();
        DirectX::XMFLOAT2 mousePos = { static_cast<float>(mouse.GetPositionX()), static_cast<float>(mouse.GetPositionY()) };

#pragma region �}�E�X�ɂ��I��
        // ---------- �}�E�X�N���b�N�ɂ��I�� ----------

        // --- �͂� ��I�� ---
        if (((mousePos.x > mousePos_[0].left_) && (mousePos.x < mousePos_[0].right_) && (mousePos.y > mousePos_[0].top_) && (mousePos.y < mousePos_[0].bottom_))
            && (mouse.GetButtonDown() & Mouse::BTN_LEFT))
        {   // �Q�[����ǂݍ���                
            // �ŏI�m�F�pSprite������
            owner->blackBeltSprite_->GetSpriteTransform()->SetColorA(0.0f);
            owner->loadGameWordSprite_->GetSpriteTransform()->SetColorA(0.0f);
            owner->choseSprite_->GetSpriteTransform()->SetColorA(0.0f);

            // ���̃X�e�[�g��
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneLoadGame::STATE::LoadGame));
            return;
        }

        // --- ������ ��I�� ---
        if (((mousePos.x > mousePos_[1].left_) && (mousePos.x < mousePos_[1].right_) && (mousePos.y > mousePos_[1].top_) && (mousePos.y < mousePos_[1].bottom_))
            && (mouse.GetButtonDown() & Mouse::BTN_LEFT))
        {   // select�ɖ߂�
            // �ŏI�m�F�pSprite������
            owner->blackBeltSprite_->GetSpriteTransform()->SetColorA(0.0f);
            owner->loadGameWordSprite_->GetSpriteTransform()->SetColorA(0.0f);
            owner->choseSprite_->GetSpriteTransform()->SetColorA(0.0f);

            // ���̃X�e�[�g��
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneLoadGame::STATE::Select));
            return;
        }

#pragma endregion// �}�E�X�ɂ��I��

#pragma region �Q�[���p�b�h�ɂ��I�� (keyboard)
        // ---------- �{�^�����͂ɂ��I�� ----------

        // --- �͂� ��I�� ---
        if (currentState_ == STATE::Yes)
        {   // �Q�[����ǂݍ���
            if (gamePad.GetButtonDown() & GamePad::BTN_A)
            {
                // �ŏI�m�F�pSprite������
                owner->blackBeltSprite_->GetSpriteTransform()->SetColorA(0.0f);
                owner->loadGameWordSprite_->GetSpriteTransform()->SetColorA(0.0f);
                owner->choseSprite_->GetSpriteTransform()->SetColorA(0.0f);

                // ���̃X�e�[�g��
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneLoadGame::STATE::LoadGame));
                return;
            }
        }

        // --- ������ ��I�� ---]
        if (currentState_ == STATE::No)
        {
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

#pragma endregion// �Q�[���p�b�h�ɂ��I�� (keyboard)
    }

    // �X�e�[�g�ύX
    void LoadGameChoseState::ChangeCurrentState(STATE state)
    {
        // ���I�����Ă�����̂ƁA�V�����I���������̂������Ȃ珈�����Ȃ�
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
    // ������
    void SlideUiFadeOutState::Initialize()
    {
        // ���݂̃X�e�[�g��ݒ�
        owner->SetCurrentState(SceneLoadGame::STATE::SlideUiFadeOut);

        // �ϐ�������
        slideTimer_ = 0.0f;
        easingTimer_ = 0.0f;
        isFadeOut_ = false;
    }

    // �X�V
    void SlideUiFadeOutState::Update(const float& elapsedTime)
    {
#pragma region �摜���ړ�������
        float maxSlideTime = 0.2f;

        float endAddPosX = -1225.0f;
        float startAddPosX = 0.0f;
        
        if (!isFadeOut_)
        {
            if (slideTimer_ <= maxSlideTime)
            {
                // 125 ~ 1225 - 125
                {
                    // �Q�[���f�[�^�̊O�g�ƑI��p�摜
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

                // [01,02,03 ����]
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
                // Sprite���w��̈ʒu�ɐݒ�
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

                // [01,02,03 ����]
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
#pragma endregion// �摜���ړ�������

        // �t�F�[�h�A�E�g
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
                // ���̃X�e�[�g��
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(SceneLoadGame::STATE::ChangeScene));
                return;
            }
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
        // ���݂̃X�e�[�g��ݒ�
        owner->SetCurrentState(SceneLoadGame::STATE::LoadGame);

        // �I�������X�e�[�g�ɂ���ă��[�h����f�[�^�𕪂���
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

        // �X�v���C�g������
        owner->halfBlackBeltSprite_->GetSpriteTransform()->SetColorA(0.0f);
        owner->loadingWordSprite_->GetSpriteTransform()->SetColorA(0.0f);
        owner->completeWordSprite_->GetSpriteTransform()->SetColorA(0.0f);
        owner->loadArrowSprite_->GetSpriteTransform()->SetColorA(0.0f);

        // �ϐ�������
        loadingTimer_ = 0.0f;
        easingTimer_ = 0.0f;
        isChangeState_ = false;

        isCompleteFadeIn_ = false;

        // �X���b�h�J�n
        owner->thread_ = new std::thread(owner->LoadingThread, owner);
    }

    // �X�V
    void LoadGameState::Update(const float& elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();
        Mouse& mouse = Input::Instance().GetMouse();

        // �u�Q�[���f�[�^��ǂݍ���ł��܂��B�v��\���������Ă��Ȃ��B
        // �܂��́A�@���[�h���I����Ă��Ȃ�
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
        
        // ���[�h�̕�����]
        owner->loadArrowSprite_->GetSpriteTransform()->AddAngle(360 * elapsedTime);


        // ���[�h������
        if (owner->loadingScene_->IsReady())
        {
            owner->loadingWordSprite_->GetSpriteTransform()->SetColorA(0.0f);
            owner->loadArrowSprite_->GetSpriteTransform()->SetColorA(0.0f);

            // �t�F�[�h�C��
            float maxTime = 0.5f;
            if (easingTimer_ <= maxTime)
            {
                float alpha = Easing::InSine(easingTimer_, maxTime, 1.0f, 0.0f);

                owner->halfBlackBeltSprite_->GetSpriteTransform()->SetColorA(alpha);
                owner->completeWordSprite_->GetSpriteTransform()->SetColorA(alpha);

                easingTimer_ += elapsedTime;
            }
            // �t�F�[�h�C������
            else
            {   
                // ���͑҂�
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
        // ���݂̃X�e�[�g��ݒ�
        owner->SetCurrentState(SceneLoadGame::STATE::LoadTitle);

        // �ϐ�������
        slideTimer_ = 0.0f;
        easingTimer_ = 0.0f;
        isFadeOut_ = false;
    }

    // �X�V
    void LoadTitleState::Update(const float& elapsedTime)
    {
        float maxSlideTime = 0.2f;

        float endAddPosX = -1225.0f;
        float startAddPosX = 0.0f;

        if (slideTimer_ <= maxSlideTime)
        {
            // 125 ~ 1225 - 125
            {
                // �Q�[���f�[�^�̊O�g�ƑI��p�摜
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

            // [01,02,03 ����]
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
            // Sprite���w��̈ʒu�ɐݒ�

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

            // [01,02,03 ����]
            {
                float finalNumPosX[3] = { 260.0f, 290.0f, 245.0f };
                for (int i = 0; i < static_cast<UINT>(SceneLoadGame::GameDataBase::Max); ++i)
                {
                    owner->gameDataNumSprite_[i]->GetSpriteTransform()->SetPosX(endAddPosX + finalNumPosX[i]);
                }
            }

            isFadeOut_ = true;
        }

        // �t�F�[�h�A�E�g
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

                // �V�[���؂�ւ�
                SceneManager::Instance().ChangeScene(new SceneLoadingBack(new SceneTitle(new SceneLoadGame)));
                return;
            }
        }
    }

    // �I����
    void LoadTitleState::Finalize()
    {
    }
}

// ChangeSceneState
namespace SceneLoadGameState
{
    // ������
    void ChangeSceneState::Initialize()
    {
        // ���݂̃X�e�[�g��ݒ�
        owner->SetCurrentState(SceneLoadGame::STATE::ChangeScene);
    }

    // �X�V
    void ChangeSceneState::Update(const float& elapsedTime)
    {
        owner->ChangeScene();
    }

    // �I����
    void ChangeSceneState::Finalize()
    {
    }
}