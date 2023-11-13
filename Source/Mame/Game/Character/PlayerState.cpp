#include "PlayerState.h"
#include "Player.h"

#include "../../Input/GamePad.h"
#include "../../Other/Easing.h"


// IdleState
#pragma region IdleState
namespace PlayerState
{
    // ������
    void IdleState::Initialize()
    {
        
    }

    // �X�V
    void IdleState::Update(const float& elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();

        float ax = gamePad.GetAxisLX();
        float ay = gamePad.GetAxisLY();

        owner->MoveUpdate(elapsedTime);
    }

    // �I����
    void IdleState::Finalize()
    {
    }
}
#pragma endregion// IdleState

// WalkState
#pragma region WalkState
namespace PlayerState
{
    // ������
    void WalkState::Initialize()
    {
        owner->PlayWalkAnimation();
    }

    // �X�V
    void WalkState::Update(const float& elapsedTime)
    {
    }

    // �I����
    void WalkState::Finalize()
    {
    }
}
#pragma endregion// WalkState

// JumpState
#pragma region JumpState
namespace PlayerState
{
    // ������
    void JumpState::Initialize()
    {
    }

    // �X�V
    void JumpState::Update(const float& elapsedTime)
    {
    }

    // �I����
    void JumpState::Finalize()
    {
    }
}
#pragma endregion// JumpState

namespace PlayerState
{

}