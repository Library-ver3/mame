#include "Player.h"
#include "PlayerState.h"

#include "../../Graphics/Graphics.h"

// コンストラクタ
Player::Player()
    :Character("./Resources/Model/Character/Player/player.fbx")
{
    Graphics& graphics = Graphics::Instance();

    // pixelShader
    CreatePsFromCso(graphics.GetDevice(),
        "./Resources/Shader/playerPS.cso",
        psShader_.GetAddressOf());
}

// デストラクタ
Player::~Player()
{
}

// 初期化
void Player::Initialize()
{
    // ----- アニメーション設定 -----
    Character::PlayAnimation(0, true);

    // ----- ステートマシン -----
#pragma region ステートマシン
    stateMachine_ = std::make_unique<StateMachine<State<Player>>>();

    // --- ステート登録 ---
    GetStateMachine()->RegisterState(new PlayerState::IdleState(this));
    GetStateMachine()->RegisterState(new PlayerState::WalkState(this));
    GetStateMachine()->RegisterState(new PlayerState::JumpState(this));

    // --- 最初のステートをセット ---
    GetStateMachine()->SetState(static_cast<UINT>(STATE::Idle));
#pragma endregion// ステートマシン



}

// 終了化
void Player::Finalize()
{
}

// 更新
void Player::Update(const float& elapsedTime)
{
    // ----- ステートマシン -----
    GetStateMachine()->Update(elapsedTime);


}

// 描画
void Player::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Character::Render(scale, psShader_.Get());
}

// ImGui用
void Player::DrawDebug()
{
    if (ImGui::BeginMenu("player"))
    {
        Character::DrawDebug();

        ImGui::EndMenu();
    }
}

// 入力値をカメラから見たベクトルに変換する
void Player::ConvertToCameraMoveVec(float ax, float ay)
{
    //  入力があったら
    if (ax || ay)
    {
        DirectX::XMFLOAT3 forward = Camera::Instance().GetForward();
        DirectX::XMFLOAT3 right   = Camera::Instance().GetRight();

        forward.x *= ay;
        forward.z *= ay;
        right.x   *= ax;
        right.z   *= ax;

        

    }
}

void Player::MoveUpdate(const float& elapsedTime)
{
    
}

// ステート切り替え
void Player::ChangeState(STATE state)
{
    GetStateMachine()->ChangeState(static_cast<UINT>(state));
}
