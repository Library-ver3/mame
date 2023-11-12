#pragma once
#include "../AI/State.h"

class Player;
namespace PlayerState
{
#pragma region IdleState
    class IdleState : public State<Player>
    {
    public:
        IdleState(Player* player) : State(player, "Idle") {}
        ~IdleState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };
#pragma endregion// IdleState

#pragma region WalkState
    class WalkState : public State<Player>
    {
    public:
        WalkState(Player* player) : State(player, "Walk") {}
        ~WalkState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };
#pragma endregion// WalkState


#pragma region JumpState
    class JumpState : State<Player>
    {
    public:
        JumpState(Player* player) : State(player, "Jump") {}
        ~JumpState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };
#pragma endregion// JumpState
}