#include "GameObject.h"
#include "../Graphics/Graphics.h"

#include "../Resource/ResourceManager.h"

#include "../Other/MathHelper.h"

// �R���X�g���N�^ ( �f�t�H���g )
GameObject::GameObject(const char* fbxFileName, bool triangulate, float samplingRate)
{
    model_ = ResourceManager::Instance().LoadModelResource(
        Graphics::Instance().GetDevice(),
        fbxFileName,
        triangulate,
        samplingRate
    );
}

// �R���X�g���N�^ ( �ʃA�j���[�V�����t�@�C������ )
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

// ������
void GameObject::Initialize()
{
}

void GameObject::Finalize()
{
}

// �X�V����
void GameObject::Update(const float& elapsedTime)
{
}

// �`�揈��
void GameObject::Render(
    const float& scale, ID3D11PixelShader* psShader)
{
    Graphics& graphics = Graphics::Instance();

    // world�s��X�V
    DirectX::XMFLOAT4X4 world = {};
    DirectX::XMStoreFloat4x4(&world, GetTransform()->CalcWorldMatrix(scale));

    // �`��
    if (&keyFrame)
    {   // �A�j���[�V�����L
        model_->Render(graphics.GetDeviceContext(), world, GetColor(), &keyFrame, psShader);
    }
    else
    {   // �A�j���[�V������
        model_->Render(graphics.GetDeviceContext(), world, GetColor(), nullptr, psShader);
    }    
}

// ImGui�p
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


// �A�j���[�V�����Đ�
void GameObject::PlayAnimation(const int& index, const bool& loop, const float& speed, const float& blendSeconds)
{
    // �ݒ�p ( ���� ) �̃A�j���[�V�����ԍ������݂̃A�j���[�V�����ԍ��Ɠ����ꍇ��return
    if (index == currentAnimationIndex_) return;

    currentAnimationIndex_      = index;    // �Đ�����A�j���[�V�����ԍ���ݒ�
    currentAnimationSeconds_    = 0.0f;     // �A�j���[�V�����Đ����Ԃ����Z�b�g
    
    animationLoopFlag_          = loop;     // ���[�v�����邩
    animationEndFlag_           = false;    // �Đ��I���t���O�����Z�b�g

    animationSpeed_             = speed;    // �A�j���[�V�����Đ����x

    blendAnimationIndex1_       = -1;           // �u�����h��
    blendAnimationIndex2_       = -1;           // �u�����h��
    animationBlendTime_         = 0.0f;         // �u�����h�� 
    animationBlendSeconds_      = blendSeconds; // �u�����h��
}

// �A�j���[�V�����u�����h
void GameObject::PlayBlendAnimation(const int& index1, const int& index2, const bool& loop, const float& speed)
{
    // �ݒ�p ( ���� ) �̃A�j���[�V�����ԍ������݂̃A�j���[�V�����ԍ��Ɠ����ꍇ��return
    if (blendAnimationIndex1_ == index1 && blendAnimationIndex2_ == index2) return;

    currentAnimationIndex_  = -1; // �ʏ�̃A�j���[�V�������g��Ȃ��̂Ł@-1

    blendAnimationIndex1_   = index1; // �Đ�����A�j���[�V������ݒ�
    blendAnimationIndex2_   = index2; // �Đ�����A�j���[�V������ݒ�
    blendAnimationSeconds_  = 0.0f;   // �A�j���[�V�����Đ����Ԃ����Z�b�g

    animationLoopFlag_      = loop;   // ���[�v�����邩
    animationEndFlag_       = false;  // �Đ��I���t���O�����Z�b�g

    animationSpeed_         = speed;  // �A�j���[�V�����Đ����x

    animationBlendTime_     = 0.0f;   // �u�����h�p�^�C�}�[
}

// �u�����h���̌v�Z�X�V����
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

// �A�j���[�V�����X�V����
void GameObject::UpdateAnimation(const float& elapsedTime)
{
    // �A�j���[�V�����Đ����ĂȂ������珈�����Ȃ�
    if (!IsPlayAnimation()) return;

    // �ŏI�t���[������ ( �Đ��I���t���O�������Ă���΍Đ��I�� )
    if (animationEndFlag_)
    {
        animationEndFlag_ = false;   // �I���t���O�����Z�b�g
        currentAnimationIndex_ = -1; // �A�j���[�V�����ԍ����Z�b�g
        return;
    }

    // �u�����h�A�j���[�V�����Đ� ( �u�����h�A�j���[�V�����̏ꍇ�����ŏI�� )
    if (UpdateBlendAnimation(elapsedTime)) return;

    // �A�j���[�V�����Đ����Ԍo��
    currentAnimationSeconds_ += elapsedTime;

    // �w��̃A�j���[�V�����f�[�^���擾
    Animation& animation = GetAnimation()->at(currentAnimationIndex_);

    // ���݂̃t���[�����擾
    // �x�����ז��Ȃ̂ňꎞ�I��float�ϐ��Ɋi�[
    const float frameIndex_float = (currentAnimationSeconds_ * animation.samplingRate) * animationSpeed_;
    const size_t frameIndex = static_cast<size_t>(frameIndex_float);

    // �O�����猻�݂̃t���[��������Ă����悤�ɂ���
    currentKeyFrameIndex_ = frameIndex;

    // �Ō�̃t���[�����擾
    const size_t frameEnd = (animation.sequence.size() - 1);

    // �A�j���[�V�������Đ��������Ă��ꍇ
    if (frameIndex > frameEnd)
    {
        if (animationLoopFlag_)
        {   // ���[�v�L : �ŏ��̃t���[���ɖ߂�
            currentAnimationSeconds_ = 0.0f;
            return;
        }
        else
        {   // ���[�v�� : �I��
            animationEndFlag_ = true;
            return;
        }
    }
    // �A�j���[�V�����X�V
    // �L�[�t���[�����X�V����Ă��ăA�j���[�V�������Đ��������Ă��Ȃ��Ƃ��̓A�j���[�V�������X���[�Y�ɐ؂�ւ���
    else if ((keyFrame.nodes.size() > 0) && (frameIndex < frameEnd))
    {
        // �u�����h���̌v�Z
        blendRate_ = 1.0f;
        UpdateBlendRate(blendRate_, elapsedTime);

        // �L�[�t���[���擾
        const std::vector<Animation::KeyFrame>& keyFrames = animation.sequence;

        // ���݂̑O��̃L�[�t���[�����擾
        const Animation::KeyFrame* keyFrameArray[2] =
        {
            &keyFrame,
            &keyFrames.at(frameIndex + 1),
        };

        // �A�j���[�V���������炩�ɐ؂�ւ���
        model_->BlendAnimations(keyFrameArray, blendRate_, keyFrame);

        // �A�j���[�V�����X�V
        model_->UpdateAnimation(keyFrame);
    }
    // �L�[�t���[������x���X�V����Ă��Ȃ��ăA�j���[�V�������Đ��������Ă��Ȃ���Ό��݂̃t���[����ۑ�
    else
    {
        keyFrame = animation.sequence.at(frameIndex);
    }
}

// �A�j���[�V�������Đ����Ȃ� true
const bool GameObject::IsPlayAnimation() const
{
    // �A�j���[�V�����Đ�����ĂȂ�
    if (currentAnimationIndex_ < 0 && blendAnimationIndex1_ < 0) return false;

    // �A�j���[�V�����ԍ������݂��Ȃ�
    const int animationIndexEnd = static_cast<int>(model_->animationClips.size());
    if (currentAnimationIndex_ >= animationIndexEnd && blendAnimationIndex1_ >= animationIndexEnd) return false;

    return true;
}

// �A�j���[�V�����u�����h�X�V
bool GameObject::UpdateBlendAnimation(const float& elapsedTime)
{
    // �A�j���[�V�����ԍ����ݒ肳��ĂȂ� ( �Đ����Ȃ� )
    if (blendAnimationIndex1_ < 0) return false;

    // �A�j���[�V�������I����Ă���̂ŏ������Ȃ�
    if (animationEndFlag_)
    {
        animationEndFlag_       = false; // �I���t���O�����Z�b�g
        blendAnimationIndex1_   = -1;    // �A�j���[�V�����ԍ������Z�b�g
        blendAnimationIndex2_   = -1;    // �A�j���[�V�����ԍ������Z�b�g
        return false;
    }

    // weight�l�� 0 ~ 1 �̊ԂɎ��߂�
    if (weight_ < 0) weight_ = 0;
    else if (weight_ > 1) weight_ = 1;

    // �A�j���[�V�����Đ����ԍX�V
    blendAnimationSeconds_ += elapsedTime;

    Animation& bAnimation1 = GetAnimation()->at(blendAnimationIndex1_);
    Animation& bAnimation2 = GetAnimation()->at(blendAnimationIndex2_);

    const size_t frameCount1 = bAnimation1.sequence.size();
    const size_t frameCount2 = bAnimation2.sequence.size();

    // ��ƂȂ�A�j���[�V�����Ƀt���[���������킹��
    size_t maxFrameCount = frameCount1;
    if (blendThreshold_ < weight_)maxFrameCount = frameCount2;

    const float frameIndex = (animationBlendSeconds_ * bAnimation1.samplingRate) * animationSpeed_;

    // �A�j���[�V�������Đ��������Ă�ꍇ
    if (frameIndex > maxFrameCount)
    {
        if (animationLoopFlag_)
        {   // ���[�v�L : �ŏ��̃t���[���ɖ߂�
            blendAnimationSeconds_ = 0.0f;
            return true;
        }
        else
        {   // ���[�v�� : �I��
            animationEndFlag_ = true;
            return true;
        }
    }

    // �Đ��t���[���𐳋K�����čĐ����Ԃ̒��������킹��
    UINT64 frameIndex1 = static_cast<UINT64>(frameIndex / maxFrameCount * (frameCount1 - 1));
    UINT64 frameIndex2 = static_cast<UINT64>(frameIndex / maxFrameCount * (frameCount2 - 1));

    // �u�����h�����X�V
    blendRate_ = 1.0f;
    UpdateBlendRate(blendRate_, elapsedTime);

    // �L�[�t���[���擾
    const std::vector<Animation::KeyFrame>& kryFrames1 = bAnimation1.sequence;

    // ���݂̑O��̃L�[�t���[�����擾
    const Animation::KeyFrame* keyFrameArray1[2] =
    {
        &kryFrames1.at(frameIndex1),
        &kryFrames1.at(frameIndex1 + 1),
    };

    // �A�j���[�V���������炩�ɐ؂�ւ���
    Animation::KeyFrame keyFrame1;
    model_->BlendAnimations(keyFrameArray1, blendRate_, keyFrame1);

    // �L�[�t���[���擾
    const std::vector<Animation::KeyFrame>& keyFrames2 = bAnimation2.sequence;

    // ���݂̑O��̃L�[�t���[�����擾
    const Animation::KeyFrame* keyFrameArray2[2] =
    {
        &keyFrames2.at(frameIndex2),
        &keyFrames2.at(frameIndex2 + 1),
    };

    // �A�j���[�V���������炩�ɐ؂�ւ���
    Animation::KeyFrame keyFrame2;
    model_->BlendAnimations(keyFrameArray2, blendRate_, keyFrame2);

    // ��̃A�j���[�V���������������ɂ���
    const Animation::KeyFrame* resultKeyFrameArray[2] = { &keyFrame1, &keyFrame2 };
    model_->BlendAnimations(resultKeyFrameArray, weight_, keyFrame);

    // �A�j���[�V�����X�V
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
