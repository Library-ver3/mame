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

    private:
        float easingTimer_ = 0.0f;
    };
}

