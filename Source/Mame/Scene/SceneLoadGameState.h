#pragma once
#include "../Game/AI/State.h"
#include "SceneLoadGame.h"

namespace SceneLoadGameState
{
    class SlideUiState : public State<SceneLoadGame>
    {
    public:
        SlideUiState(SceneLoadGame* sceneLoadGame) : State(sceneLoadGame, "SlideUiState") {}
        ~SlideUiState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        float easingTimer_ = 0.0f;
    };

    class SelectState : public State<SceneLoadGame>
    {
    public:
        enum class GameData
        {
            One,
            Two,
            Three,
        };
    public:
        SelectState(SceneLoadGame* sceneLoadGame) : State(sceneLoadGame, "SelectState") {}
        ~SelectState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

        void UpdateSelectGameData(const float& elapsedTime);
        void UpdateBreakTime(const float& elapsedTime);

    private:
        GameData gameDataNum_ = GameData::One;
        GameData maxGameDataNum_ = GameData::Two;

        bool isChangeState_ = false;
        float breakTimer_ = 0.0f;

        bool isFadeSprite_ = false;
        float fadeSpriteTimer_ = 0.0f;
    };

    class LoadGameChoseState : public State<SceneLoadGame>
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
        LoadGameChoseState(SceneLoadGame* sceneLoadGame) : State(sceneLoadGame, "LoadGameChoseState") {}
        ~LoadGameChoseState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

        void FadeInUI(const float& elapsedTime);
        void ChoseYesOrNo(const float& elapsedTime);
        bool ChoseSpriteUpdate(const float& elapsedTime);
        void PushButton();

        void ChangeCurrentState(STATE state);

    private:
        bool isChose_           = false;  // trueなら選択可能
        
        STATE currentState_ = STATE::Yes;
        STATE oldCurrentState_ = STATE::Yes;
        float easingTimer_ = 0.0f;
        float animationTimer_ = 0.0f;
    };

    class SlideUiFadeOutState : public State<SceneLoadGame>
    {
    public:
        SlideUiFadeOutState(SceneLoadGame* sceneLoadGame) : State(sceneLoadGame, "SlideUiFadeOutState") {}
        ~SlideUiFadeOutState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        float slideTimer_ = 0.0f;
        float easingTimer_ = 0.0f;

        bool isFadeOut_ = false;
    };

    class LoadGameState : public State<SceneLoadGame>
    {
    public:
        LoadGameState(SceneLoadGame* sceneLoadGame) : State(sceneLoadGame, "LoadGameState") {}
        ~LoadGameState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        float loadingTimer_ = 0.0f;
        float easingTimer_ = 0.0f;
        bool isChangeState_ = false;

        bool isCompleteFadeIn_ = false; // ゲームデータを読み込んでいます。表示完了
    };

    class LoadTitleState : public State<SceneLoadGame>
    {
    public:
        LoadTitleState(SceneLoadGame* sceneLoadGame) : State(sceneLoadGame, "LoadTitleState") {}
        ~LoadTitleState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        float slideTimer_ = 0.0f;
        float easingTimer_ = 0.0f;

        bool isFadeOut_ = false;
    };

    class ChangeSceneState : public State<SceneLoadGame>
    {
    public:
        ChangeSceneState(SceneLoadGame* sceneLoadGame) : State(sceneLoadGame, "ChangeSceneState") {}
        ~ChangeSceneState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };
}