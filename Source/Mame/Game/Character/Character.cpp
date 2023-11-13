#include "Character.h"

// コンストラクタ ( デフォルト )
Character::Character(const char* fbxFileName, bool triangulate, float samplingRate)
    :GameObject(fbxFileName, triangulate, samplingRate)
{
}

// コンストラクタ ( 別アニメーションファイルあり )
Character::Character(const char* fbxFileName, std::vector<std::string>& animationFileName, bool triangulate, float samplingRate)
    :GameObject(fbxFileName, animationFileName, triangulate, samplingRate)
{
}

// デストラクタ
Character::~Character()
{
}

// 初期化
void Character::Initialize()
{
}

// 終了化
void Character::Finalize()
{
}

// 更新
void Character::Update(const float& elapsedTime)
{
}

// 描画
void Character::Render(const float& scale, ID3D11PixelShader* psShader)
{
    // 描画
    GameObject::Render(scale, psShader);
}

// ImGui用
void Character::DrawDebug()
{
}

// ConstantBuffer
void Character::UpdateConstants()
{
}

void Character::Turn(const float& elapsedTime, float vx, float vz, float rotSpeed)
{
    // 移動値がない場合処理しない
    if (vx < 0.01f && vx > -0.01f && vz < 0.01f && vz > -0.01f) return;

    rotSpeed = DirectX::XMConvertToRadians(rotSpeed * elapsedTime);

    float length = sqrtf(vx * vx + vz * vz);
    vx /= length;
    vz /= length;

    DirectX::XMFLOAT3 front = GetTransform()->CalcForward();

    float dot = (vx * front.x) + (vz * front.z);
    float rot = 1.0f - dot;
    if (rot < 0.005f)return;
    if (rot < 0.3f) rot = 0.3f;

    rot += 0.5f;
    float rotationSpeed = rotSpeed * rot;

    // 演算がオーバーフローしたときは処理しない
    if (rotationSpeed > 100.0f || rotationSpeed < -100.0f) return;

    // 左右判定
    float cross = (vx * front.z) - (vz * front.x);

    DirectX::XMFLOAT4 rotation = GetTransform()->GetRotation();
    rotation.y += cross < 0.0f ? -rotationSpeed : rotationSpeed;

    rotationValue_ = cross < 0.0f ? -rotationSpeed : rotationSpeed;

    GetTransform()->SetRotationY(rotation.y);
}
