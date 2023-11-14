#pragma once
#include "../Game/AI/State.h"
#include "SceneTitle.h"

namespace SceneTitleState
{
    class FadeInState : public State<SceneTitle>
    {
    public:
        FadeInState(SceneTitle* sceneTitle) : State(sceneTitle, "FadeInState") {}
        ~FadeInState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        float easingTimer_ = 0.0f;
    };

    class PressAnyButtonState : public State<SceneTitle>
    {
    public:
        PressAnyButtonState(SceneTitle* sceneTitle) : State(sceneTitle, "PressAnyButtonState") {}
        ~PressAnyButtonState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

        void UpdateSprite(const float& elapsedTime);

    private:
        float easingTimer_ = 0.0f;
        bool isBrightnessUp_ = false;
    };

    class PressAnyButtonFadeOutState : public State<SceneTitle>
    {
    public:
        PressAnyButtonFadeOutState(SceneTitle* sceneTitle) : State(sceneTitle, "PressAnyButtonFadeOut") {}
        ~PressAnyButtonFadeOutState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        float easingTimer_ = 0.0f;
    };

    class SelectFadeInState : public State<SceneTitle>
    {
    public:
        SelectFadeInState(SceneTitle* sceneTitle) : State(sceneTitle, "SelectFadeInState") {}
        ~SelectFadeInState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        float easingTimer_ = 0.0f;
    };

    class SelectState : public State<SceneTitle>
    {
    private:// 定数
        enum class STATE
        {
            LoadGame,
            QuitGame,
        };

        struct MousePos
        {
            float top_;
            float bottom_;
            float left_;
            float right_;
        }mousePos_[2] =
        {
            { 420.0f, 470.0f, 410.0f, 870.0f },
            { 470.0f, 520.0f, 410.0f, 870.0f },
        };

    public:
        SelectState(SceneTitle* sceneTitle) : State(sceneTitle, "SelectState") {}
        ~SelectState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

        void UpdateState(const float& elapsedTime);
        float UpdateBrightness(const float& elapsedTime, const char* who);

        void ChangeCurrentState(STATE state);

    private:
        float easingTimer_ = 0.0f;
        int currentState_ = static_cast<UINT>(STATE::LoadGame);
        bool isBrightnessUp_ = false;

        // ----- ステート切り替え用 -----
        bool isChangeState_ = false; 
        float breakTimer_ = 0.0f;
    };

    class QuitGameChoseState : public State<SceneTitle>
    {
    private:
        enum class STATE
        {
            Yes,
            No,
        };

        struct MousePos
        {
            float top_;
            float bottom_;
            float left_;
            float right_;
        }mousePos_[2] =
        {
            { 380.0f, 440.0f, 460.0f, 640.0f },
            { 380.0f, 440.0f, 640.0f, 820.0f },
        };

    public:
        QuitGameChoseState(SceneTitle* sceneTitle) : State(sceneTitle, "QuitGameChoseState") {}
        ~QuitGameChoseState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

        void UpdateFadeIn(const float& elapsedTime);
        void ChangeCurrentState(STATE state);

    private:
        bool isChose_ = false;
        bool isPlayAnimation_ = false;
        STATE currentState_ = STATE::Yes;
        STATE oldCurrentState_ = STATE::No;
        float easingTimer_ = 0.0f;
        float animationTimer_ = 0.0f;
    };

    class LoadGameFadeOut : public State<SceneTitle>
    {
    public:
        LoadGameFadeOut(SceneTitle* sceneTitle) : State(sceneTitle, "LoadGameFadeOut") {}
        ~LoadGameFadeOut() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        float easingTimer_ = 0.0f;
        float delayTimer_ = 0.0f;
        bool isChangeScene_ = false;
    };
}

