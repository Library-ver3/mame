#pragma once
#include "../Game/AI/State.h"
#include "SceneTitle.h"

namespace SceneTitleState
{
    class PressAnyButtonState : public State<SceneTitle>
    {
    public:
        PressAnyButtonState(SceneTitle* sceneTitle) : State(sceneTitle, "PressAnyButtonState") {}
        ~PressAnyButtonState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

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
    public:
        SelectState(SceneTitle* sceneTitle) : State(sceneTitle, "SelectState") {}
        ~SelectState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

        void UpdateState(const float& elapsedTime);
        float UpdateBrightness(const float& elapsedTime, const char* who);

    private:// 定数
        enum class STATE
        {
            LoadGame,
            QuitGame,
        };

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
    public:
        QuitGameChoseState(SceneTitle* sceneTitle) : State(sceneTitle, "QuitGameChoseState") {}
        ~QuitGameChoseState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        enum class STATE
        {
            Yes,
            No,
        };

    private:
        bool isChose_ = false;
        bool isPlayAnimation_ = false;
        bool isButtonPush = false;
        int currentState_ = 0;
        float easingTimer_ = 0.0f;
        float animationTimer_ = 0.0f;
    };
}

