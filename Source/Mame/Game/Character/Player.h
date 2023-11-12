#pragma once
#include "Character.h"

#include "../AI/StateMachine.h"
#include "../../Input/Input.h"

class Player : public Character
{
public:// 定数
#pragma region Constants
    enum class Animation
    {
        Idle,           // 待機
        Dash,           // 走り
        Attack_1,       // 攻撃１
        Attack_2,       // 攻撃２
        Attack_3,       // 攻撃３
        Avoid,          // 回避
        Counter,        // カウンター
        CounterAttack,  // カウンター攻撃
        SoftStagger,    // 被ダメ
        HardStagger,    // 吹っ飛び、死亡
        StandUp,        // 起き上がり
        HardAttack,     // 強攻撃
        EnemyWalk,      // 歩き別ver
    };

    enum class STATE
    {
        Idle,
        Walk,
        Jump,
    };
#pragma endregion // Constants
public:// 基本的な関数 ( override された関数 )
    Player();
    ~Player() override;

    void Initialize()                           override;
    void Finalize()                             override;
    void Update(const float& elapsedTime)       override;
    void Render(const float& scale, 
        ID3D11PixelShader* psShader = nullptr)  override;
    void DrawDebug()                            override;

public:
    // ----- 入力値をカメラから見たベクトルに変換する -----
    void ConvertToCameraMoveVec(float ax, float ay);

public:// 取得・設定
    // ---------- Getter,Setter,etc... ----------
#pragma region [Get,Set]Function
    // ----- ステートマシン -----
    StateMachine<State<Player>>* GetStateMachine() { return stateMachine_.get(); }
    // --- ステート切り替え ---
    void ChangeState(STATE state);



#pragma endregion// [Get,Set]Function

    // ---------- 入力関数 ----------
#pragma region [Input]Function
    static bool InputLockOn()
    {
        return (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_RIGHT_THUMB);
    }
#pragma endregion// [Input]Function

private:// メンバ変数
    // ---------- 移動 ---------------------------------------------
    DirectX::XMFLOAT3 velocity = {}; // 速度
    
    // --- カメラの向いている方向を前とした移動方向ベクトル ---
    DirectX::XMFLOAT3 moveVec  = {};

    // -------------------------------------------------------------

    // ---------- ステートマシン ------------------------------------
    std::unique_ptr<StateMachine<State<Player>>> stateMachine_;
    // -------------------------------------------------------------

    // ---------- シェーダー ----------------------------------------
    Microsoft::WRL::ComPtr<ID3D11PixelShader> psShader_ = nullptr;
    // -------------------------------------------------------------
};

