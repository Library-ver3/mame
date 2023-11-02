#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>

#include "../Other/Transform.h"
#include "../Resource/Model.h"

// ゲームオブジェクト
class GameObject
{
public:// 基本的な関数 ( virtual付けてるやつとか )
    GameObject(const char* fbxFileName, bool triangulate = true, float samplingRate = 0);
    GameObject(const char* fbxFileName, std::vector<std::string>& animationFileName, bool triangulate = true, float samplingRate = 0);
    virtual ~GameObject() {}

    virtual void Initialize();
    virtual void Finalize();
    virtual void Update(const float& elapsedTime);
    virtual void Render(const float& scale, ID3D11PixelShader* psShader = nullptr);
    virtual void DrawDebug();

    virtual void UpdateConstants() {}


public: // 取得・設定 関数
#pragma region [Get/Set]Function
    // ----- Transform -----
    Transform* GetTransform() { return &transform_; }

    // --- Position ---
    const DirectX::XMFLOAT3& GetPosition() { return GetTransform()->GetPosition(); }
    void SetPosition(const DirectX::XMFLOAT3& pos) { GetTransform()->SetPosition(pos); }

    // ----- Color -----
    const DirectX::XMFLOAT4& GetColor() const { return color_; }
    void SetColor(const DirectX::XMFLOAT4& color) { color_ = color; }

    // ----- JointPosition -----
    DirectX::XMFLOAT3 GetJointPosition(const std::string& meshName, const std::string& boneName, const float& scale);
    DirectX::XMFLOAT3 GetJointPosition(size_t meshIndex, size_t boneIndex, const float& scale);



    // emissive ※constansのやつなのでこいつを使う場所は UpdateConstansで使ってほしい
    //void SetEmissiveIntensity(float intensity) { model_->skinnedMeshs->data.emissiveIntensity = intensity; }
    //void SetEmissiveScrollDirection(DirectX::XMFLOAT2 scroll) { model_->skinnedMeshs->data.emissiveScrollDirection = scroll; }
    //void SetEmissiveColor(DirectX::XMFLOAT4 color) { model_->skinnedMeshs->data.emissiveColor = color; }


#pragma endregion// [Get/Set]Function


private:
    std::shared_ptr<Model> model_;        // モデル情報
    Transform transform_;                       // モデル姿勢
    Animation::KeyFrame keyFrame = {};          // アニメーション用
    DirectX::XMFLOAT4 color_ = { 1, 1, 1, 1 };  // モデル色

public: // アニメーション関連関数
#pragma region AnimationFunction
    // アニメーション再生
    void PlayAnimation(
        const int& index,                // アニメーション番号
        const bool& loop,                // ループするか
        const float& speed = 1.0f,       // 再生速度
        const float& blendSeconds = 1.0f // スムーズ切り替え時間（速度）
    );

    // アニメーションブレンド
    void PlayBlendAnimation(
        const int& index1,          // ブレンドする一つ目のアニメーション番号
        const int& index2,          // ブレンドする二つ目のアニメーション番号
        const bool& loop,           // ループするか
        const float& speed = 1.0f   // 再生速度
    );

    // アニメーション更新処理
    void UpdateAnimation(const float& elapsedTime);

    // アニメーションが再生中なら true
    const bool IsPlayAnimation() const;

#pragma endregion// AnimationFunction
private: // アニメーション関連関数 ( 内部処理 )
#pragma region AnimationFunction
    // ブレンド率の計算更新処理
    void UpdateBlendRate(float blendRate, const float& elapsedTime);

    // ブレンドアニメーション更新処理 ( 更新していたら true )
    bool UpdateBlendAnimation(const float& elapsedTime);

    // アニメーションデータ取得
    std::vector<Animation>* GetAnimation() { return &model_->animationClips; }

#pragma endregion // AnimationFunction
private:// アニメーション関連変数
#pragma region Animation
    float   currentAnimationSeconds_ = 0.0f;  // 現在のアニメーション再生時間
                                     
    int     currentKeyFrameIndex_    = 0;     //現在のアニメーション再生フレーム
                                     
    float   animationBlendTime_      = 0.0f;  // 現在のブレンドタイマー
    float   animationBlendSeconds_   = 0.0f;  // ブレンド時間
                                     
    int     currentAnimationIndex_   = -1;	  // 現在のアニメーション番号
                                     
    int     blendAnimationIndex1_    = -1;	  // 一つ目ブレンド用アニメーション番号
    int     blendAnimationIndex2_    = -1;	  // 二つ目ブレンド用アニメーション番号
    float   blendAnimationSeconds_   = 0.0f;  // 現在のアニメーション再生時間
                                     
    bool    animationLoopFlag_       = false; // アニメーションをループ再生するか
    bool    animationEndFlag_        = false; // アニメーションが終了したか
                                     
    float   blendThreshold_          = 0.0f;  // ブレンドアニメーションのベースとなるアニメーションの切り替えしきい値
                                     
    float   blendRate_               = 0.0f;  // ブレンド率
                                     
    float   weight_                  = 0.0f;  // 
    float   animationSpeed_          = 1.0f;  // アニメーション再生速度
#pragma endregion // Animation

protected:// ----- ImGui用 ----- //
    const char* const GetName() const { return name_.c_str(); }
    void SetName(const std::string& name) { name_ = name; }

private:// ----- ImGui用 ----- //
    std::string name_ = "";
};

