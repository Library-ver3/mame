#include "GameObject.h"
#include "../Graphics/Graphics.h"

#include "../Resource/ResourceManager.h"

#include "../Other/MathHelper.h"

// コンストラクタ ( デフォルト )
GameObject::GameObject(const char* fbxFileName, bool triangulate, float samplingRate)
{
    model_ = ResourceManager::Instance().LoadModelResource(
        Graphics::Instance().GetDevice(),
        fbxFileName,
        triangulate,
        samplingRate
    );
}

// コンストラクタ ( 別アニメーションファイルあり )
GameObject::GameObject(const char* fbxFileName, std::vector<std::string>& animationFileName, bool triangulate, float samplingRate)
{
    model_ = ResourceManager::Instance().LoadModelResource(
        Graphics::Instance().GetDevice(),
        fbxFileName,
        animationFileName,
        triangulate,
        samplingRate
    );
}

// 初期化
void GameObject::Initialize()
{
}

void GameObject::Finalize()
{
}

// 更新処理
void GameObject::Update(const float& elapsedTime)
{
}

// 描画処理
void GameObject::Render(
    const float& scale, ID3D11PixelShader* psShader)
{
    Graphics& graphics = Graphics::Instance();

    // world行列更新
    DirectX::XMFLOAT4X4 world = {};
    DirectX::XMStoreFloat4x4(&world, GetTransform()->CalcWorldMatrix(scale));

    // 描画
    if (&keyFrame)
    {   // アニメーション有
        model_->Render(graphics.GetDeviceContext(), world, GetColor(), &keyFrame, psShader);
    }
    else
    {   // アニメーション無
        model_->Render(graphics.GetDeviceContext(), world, GetColor(), nullptr, psShader);
    }    
}

// ImGui用
void GameObject::DrawDebug()
{
    GetTransform()->DrawDebug();

    static int index;

    if (ImGui::TreeNode("Animation"))
    {
        ImGui::SliderFloat("weight", &weight_, 0.0f, 1.0f);
        ImGui::SliderFloat("threshold", &blendThreshold_, 0.0f, 1.0f);
        ImGui::InputInt("Current Index", &currentAnimationIndex_);

        ImGui::SliderInt("set Index", &index, 0, model_->animationClips.size() - 1);
        if (ImGui::Button("Play Animation"))
        {
            PlayAnimation(index, true);
        }
        static int index = (currentAnimationIndex_ > -1) ? currentAnimationIndex_ : blendAnimationIndex1_;
        ImGui::SliderInt("Keyframe Index", &currentKeyFrameIndex_, -1, GetAnimation()->at(index).sequence.size() - 1);

        ImGui::SliderFloat("Animation Speed", &animationSpeed_, 0.0f, 3.0f);

        ImGui::SliderFloat("BlendRate", &blendRate_, 0.0f, 1.0f);

        ImGui::TreePop();
    }
}


// アニメーション再生
void GameObject::PlayAnimation(const int& index, const bool& loop, const float& speed, const float& blendSeconds)
{
    // 設定用 ( 引数 ) のアニメーション番号が現在のアニメーション番号と同じ場合はreturn
    if (index == currentAnimationIndex_) return;

    currentAnimationIndex_      = index;    // 再生するアニメーション番号を設定
    currentAnimationSeconds_    = 0.0f;     // アニメーション再生時間をリセット
    
    animationLoopFlag_          = loop;     // ループさせるか
    animationEndFlag_           = false;    // 再生終了フラグをリセット

    animationSpeed_             = speed;    // アニメーション再生速度

    blendAnimationIndex1_       = -1;           // ブレンド無
    blendAnimationIndex2_       = -1;           // ブレンド無
    animationBlendTime_         = 0.0f;         // ブレンド無 
    animationBlendSeconds_      = blendSeconds; // ブレンド無
}

// アニメーションブレンド
void GameObject::PlayBlendAnimation(const int& index1, const int& index2, const bool& loop, const float& speed)
{
    // 設定用 ( 引数 ) のアニメーション番号が現在のアニメーション番号と同じ場合はreturn
    if (blendAnimationIndex1_ == index1 && blendAnimationIndex2_ == index2) return;

    currentAnimationIndex_  = -1; // 通常のアニメーションを使わないので　-1

    blendAnimationIndex1_   = index1; // 再生するアニメーションを設定
    blendAnimationIndex2_   = index2; // 再生するアニメーションを設定
    blendAnimationSeconds_  = 0.0f;   // アニメーション再生時間をリセット

    animationLoopFlag_      = loop;   // ループさせるか
    animationEndFlag_       = false;  // 再生終了フラグをリセット

    animationSpeed_         = speed;  // アニメーション再生速度

    animationBlendTime_     = 0.0f;   // ブレンド用タイマー
}

// ブレンド率の計算更新処理
void GameObject::UpdateBlendRate(float blendRate, const float& elapsedTime)
{
    if (animationBlendTime_ < animationBlendSeconds_)
    {
        animationBlendTime_ += elapsedTime;
        if (animationBlendTime_ >= animationBlendSeconds_)
        {
            animationBlendTime_ = animationBlendSeconds_;
        }
        blendRate = animationBlendTime_ / animationBlendSeconds_;
        blendRate *= blendRate;
    }
}

// アニメーション更新処理
void GameObject::UpdateAnimation(const float& elapsedTime)
{
    // アニメーション再生してなかったら処理しない
    if (!IsPlayAnimation()) return;

    // 最終フレーム処理 ( 再生終了フラグがたっていれば再生終了 )
    if (animationEndFlag_)
    {
        animationEndFlag_ = false;   // 終了フラグをリセット
        currentAnimationIndex_ = -1; // アニメーション番号リセット
        return;
    }

    // ブレンドアニメーション再生 ( ブレンドアニメーションの場合ここで終了 )
    if (UpdateBlendAnimation(elapsedTime)) return;

    // アニメーション再生時間経過
    currentAnimationSeconds_ += elapsedTime;

    // 指定のアニメーションデータを取得
    Animation& animation = GetAnimation()->at(currentAnimationIndex_);

    // 現在のフレームを取得
    // 警告が邪魔なので一時的にfloat変数に格納
    const float frameIndex_float = (currentAnimationSeconds_ * animation.samplingRate) * animationSpeed_;
    const size_t frameIndex = static_cast<size_t>(frameIndex_float);

    // 外部から現在のフレームを取ってこれるようにする
    currentKeyFrameIndex_ = frameIndex;

    // 最後のフレームを取得
    const size_t frameEnd = (animation.sequence.size() - 1);

    // アニメーションが再生しきってた場合
    if (frameIndex > frameEnd)
    {
        if (animationLoopFlag_)
        {   // ループ有 : 最初のフレームに戻る
            currentAnimationSeconds_ = 0.0f;
            return;
        }
        else
        {   // ループ無 : 終了
            animationEndFlag_ = true;
            return;
        }
    }
    // アニメーション更新
    // キーフレームが更新されていてアニメーションが再生しきっていないときはアニメーションをスムーズに切り替える
    else if ((keyFrame.nodes.size() > 0) && (frameIndex < frameEnd))
    {
        // ブレンド率の計算
        blendRate_ = 1.0f;
        UpdateBlendRate(blendRate_, elapsedTime);

        // キーフレーム取得
        const std::vector<Animation::KeyFrame>& keyFrames = animation.sequence;

        // 現在の前後のキーフレームを取得
        const Animation::KeyFrame* keyFrameArray[2] =
        {
            &keyFrame,
            &keyFrames.at(frameIndex + 1),
        };

        // アニメーションを滑らかに切り替える
        model_->BlendAnimations(keyFrameArray, blendRate_, keyFrame);

        // アニメーション更新
        model_->UpdateAnimation(keyFrame);
    }
    // キーフレームが一度も更新されていなくてアニメーションが再生しきっていなければ現在のフレームを保存
    else
    {
        keyFrame = animation.sequence.at(frameIndex);
    }
}

// アニメーションが再生中なら true
const bool GameObject::IsPlayAnimation() const
{
    // アニメーション再生されてない
    if (currentAnimationIndex_ < 0 && blendAnimationIndex1_ < 0) return false;

    // アニメーション番号が存在しない
    const int animationIndexEnd = static_cast<int>(model_->animationClips.size());
    if (currentAnimationIndex_ >= animationIndexEnd && blendAnimationIndex1_ >= animationIndexEnd) return false;

    return true;
}

// アニメーションブレンド更新
bool GameObject::UpdateBlendAnimation(const float& elapsedTime)
{
    // アニメーション番号が設定されてない ( 再生しない )
    if (blendAnimationIndex1_ < 0) return false;

    // アニメーションが終わっているので処理しない
    if (animationEndFlag_)
    {
        animationEndFlag_       = false; // 終了フラグをリセット
        blendAnimationIndex1_   = -1;    // アニメーション番号をリセット
        blendAnimationIndex2_   = -1;    // アニメーション番号をリセット
        return false;
    }

    // weight値を 0 ~ 1 の間に収める
    if (weight_ < 0) weight_ = 0;
    else if (weight_ > 1) weight_ = 1;

    // アニメーション再生時間更新
    blendAnimationSeconds_ += elapsedTime;

    Animation& bAnimation1 = GetAnimation()->at(blendAnimationIndex1_);
    Animation& bAnimation2 = GetAnimation()->at(blendAnimationIndex2_);

    const size_t frameCount1 = bAnimation1.sequence.size();
    const size_t frameCount2 = bAnimation2.sequence.size();

    // 基準となるアニメーションにフレーム数を合わせる
    size_t maxFrameCount = frameCount1;
    if (blendThreshold_ < weight_)maxFrameCount = frameCount2;

    const float frameIndex = (animationBlendSeconds_ * bAnimation1.samplingRate) * animationSpeed_;

    // アニメーションが再生しきってる場合
    if (frameIndex > maxFrameCount)
    {
        if (animationLoopFlag_)
        {   // ループ有 : 最初のフレームに戻る
            blendAnimationSeconds_ = 0.0f;
            return true;
        }
        else
        {   // ループ無 : 終了
            animationEndFlag_ = true;
            return true;
        }
    }

    // 再生フレームを正規化して再生時間の長さを合わせる
    UINT64 frameIndex1 = static_cast<UINT64>(frameIndex / maxFrameCount * (frameCount1 - 1));
    UINT64 frameIndex2 = static_cast<UINT64>(frameIndex / maxFrameCount * (frameCount2 - 1));

    // ブレンド率を更新
    blendRate_ = 1.0f;
    UpdateBlendRate(blendRate_, elapsedTime);

    // キーフレーム取得
    const std::vector<Animation::KeyFrame>& kryFrames1 = bAnimation1.sequence;

    // 現在の前後のキーフレームを取得
    const Animation::KeyFrame* keyFrameArray1[2] =
    {
        &kryFrames1.at(frameIndex1),
        &kryFrames1.at(frameIndex1 + 1),
    };

    // アニメーションを滑らかに切り替える
    Animation::KeyFrame keyFrame1;
    model_->BlendAnimations(keyFrameArray1, blendRate_, keyFrame1);

    // キーフレーム取得
    const std::vector<Animation::KeyFrame>& keyFrames2 = bAnimation2.sequence;

    // 現在の前後のキーフレームを取得
    const Animation::KeyFrame* keyFrameArray2[2] =
    {
        &keyFrames2.at(frameIndex2),
        &keyFrames2.at(frameIndex2 + 1),
    };

    // アニメーションを滑らかに切り替える
    Animation::KeyFrame keyFrame2;
    model_->BlendAnimations(keyFrameArray2, blendRate_, keyFrame2);

    // 二つのアニメーションをいい感じにする
    const Animation::KeyFrame* resultKeyFrameArray[2] = { &keyFrame1, &keyFrame2 };
    model_->BlendAnimations(resultKeyFrameArray, weight_, keyFrame);

    // アニメーション更新
    model_->UpdateAnimation(keyFrame);

    return true;
}

// JOINT_POSITION
DirectX::XMFLOAT3 GameObject::GetJointPosition(const std::string& meshName, const std::string& boneName, const float& scale)
{
    DirectX::XMFLOAT4X4 transform{};
    DirectX::XMStoreFloat4x4(&transform, GetTransform()->CalcWorldMatrix(scale));

    return model_->JointPosition(meshName, boneName, &keyFrame, transform);
}

// JOINT_POSITION
DirectX::XMFLOAT3 GameObject::GetJointPosition(size_t meshIndex, size_t boneIndex, const float& scale)
{
    DirectX::XMFLOAT4X4 transform{};
    DirectX::XMStoreFloat4x4(&transform, GetTransform()->CalcWorldMatrix(scale));

    return model_->JointPosition(meshIndex, boneIndex, &keyFrame, transform);
}
