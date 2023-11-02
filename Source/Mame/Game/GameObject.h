#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>

#include "../Other/Transform.h"
#include "../Resource/Model.h"

// �Q�[���I�u�W�F�N�g
class GameObject
{
public:// ��{�I�Ȋ֐� ( virtual�t���Ă��Ƃ� )
    GameObject(const char* fbxFileName, bool triangulate = true, float samplingRate = 0);
    GameObject(const char* fbxFileName, std::vector<std::string>& animationFileName, bool triangulate = true, float samplingRate = 0);
    virtual ~GameObject() {}

    virtual void Initialize();
    virtual void Finalize();
    virtual void Update(const float& elapsedTime);
    virtual void Render(const float& scale, ID3D11PixelShader* psShader = nullptr);
    virtual void DrawDebug();

    virtual void UpdateConstants() {}


public: // �擾�E�ݒ� �֐�
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



    // emissive ��constans�̂�Ȃ̂ł������g���ꏊ�� UpdateConstans�Ŏg���Ăق���
    //void SetEmissiveIntensity(float intensity) { model_->skinnedMeshs->data.emissiveIntensity = intensity; }
    //void SetEmissiveScrollDirection(DirectX::XMFLOAT2 scroll) { model_->skinnedMeshs->data.emissiveScrollDirection = scroll; }
    //void SetEmissiveColor(DirectX::XMFLOAT4 color) { model_->skinnedMeshs->data.emissiveColor = color; }


#pragma endregion// [Get/Set]Function


private:
    std::shared_ptr<Model> model_;        // ���f�����
    Transform transform_;                       // ���f���p��
    Animation::KeyFrame keyFrame = {};          // �A�j���[�V�����p
    DirectX::XMFLOAT4 color_ = { 1, 1, 1, 1 };  // ���f���F

public: // �A�j���[�V�����֘A�֐�
#pragma region AnimationFunction
    // �A�j���[�V�����Đ�
    void PlayAnimation(
        const int& index,                // �A�j���[�V�����ԍ�
        const bool& loop,                // ���[�v���邩
        const float& speed = 1.0f,       // �Đ����x
        const float& blendSeconds = 1.0f // �X���[�Y�؂�ւ����ԁi���x�j
    );

    // �A�j���[�V�����u�����h
    void PlayBlendAnimation(
        const int& index1,          // �u�����h�����ڂ̃A�j���[�V�����ԍ�
        const int& index2,          // �u�����h�����ڂ̃A�j���[�V�����ԍ�
        const bool& loop,           // ���[�v���邩
        const float& speed = 1.0f   // �Đ����x
    );

    // �A�j���[�V�����X�V����
    void UpdateAnimation(const float& elapsedTime);

    // �A�j���[�V�������Đ����Ȃ� true
    const bool IsPlayAnimation() const;

#pragma endregion// AnimationFunction
private: // �A�j���[�V�����֘A�֐� ( �������� )
#pragma region AnimationFunction
    // �u�����h���̌v�Z�X�V����
    void UpdateBlendRate(float blendRate, const float& elapsedTime);

    // �u�����h�A�j���[�V�����X�V���� ( �X�V���Ă����� true )
    bool UpdateBlendAnimation(const float& elapsedTime);

    // �A�j���[�V�����f�[�^�擾
    std::vector<Animation>* GetAnimation() { return &model_->animationClips; }

#pragma endregion // AnimationFunction
private:// �A�j���[�V�����֘A�ϐ�
#pragma region Animation
    float   currentAnimationSeconds_ = 0.0f;  // ���݂̃A�j���[�V�����Đ�����
                                     
    int     currentKeyFrameIndex_    = 0;     //���݂̃A�j���[�V�����Đ��t���[��
                                     
    float   animationBlendTime_      = 0.0f;  // ���݂̃u�����h�^�C�}�[
    float   animationBlendSeconds_   = 0.0f;  // �u�����h����
                                     
    int     currentAnimationIndex_   = -1;	  // ���݂̃A�j���[�V�����ԍ�
                                     
    int     blendAnimationIndex1_    = -1;	  // ��ڃu�����h�p�A�j���[�V�����ԍ�
    int     blendAnimationIndex2_    = -1;	  // ��ڃu�����h�p�A�j���[�V�����ԍ�
    float   blendAnimationSeconds_   = 0.0f;  // ���݂̃A�j���[�V�����Đ�����
                                     
    bool    animationLoopFlag_       = false; // �A�j���[�V���������[�v�Đ����邩
    bool    animationEndFlag_        = false; // �A�j���[�V�������I��������
                                     
    float   blendThreshold_          = 0.0f;  // �u�����h�A�j���[�V�����̃x�[�X�ƂȂ�A�j���[�V�����̐؂�ւ��������l
                                     
    float   blendRate_               = 0.0f;  // �u�����h��
                                     
    float   weight_                  = 0.0f;  // 
    float   animationSpeed_          = 1.0f;  // �A�j���[�V�����Đ����x
#pragma endregion // Animation

protected:// ----- ImGui�p ----- //
    const char* const GetName() const { return name_.c_str(); }
    void SetName(const std::string& name) { name_ = name; }

private:// ----- ImGui�p ----- //
    std::string name_ = "";
};

