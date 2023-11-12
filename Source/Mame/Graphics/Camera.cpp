#include "Camera.h"

#include "../Graphics/Graphics.h"

#include "../Input/Input.h"

#include "../Other/MathHelper.h"
#include "../Other/Easing.h"
#include "../Other/MathHelper.h"

// 固定カメラ初期化
void Camera::FixedInitialize()
{
    transform.SetPosition(DirectX::XMFLOAT3(0.0f, 1.0f, 10.0f));
    transform.SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
    transform.SetRotation(DirectX::XMFLOAT4(0.0f, DirectX::XMConvertToRadians(180), 0.0f, 0.0f));
}

// 固定カメラ用 透視投影
void Camera::FixedSetPerSpectiveFov()
{
    D3D11_VIEWPORT viewport{};
    UINT unm_viewports{ 1 };
    Graphics::Instance().GetDeviceContext()->RSGetViewports(&unm_viewports, &viewport);

    float aspect_ratio{ viewport.Width / viewport.Height };
    P = { DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(30), aspect_ratio, 0.1f, 1000.0f) };

    DirectX::XMVECTOR eye = { DirectX::XMVectorSet(camera.eye.x,camera.eye.y,camera.eye.z,1.0f) };
    DirectX::XMVECTOR focus = { DirectX::XMVectorSet(camera.focus.x,camera.focus.y,camera.focus.z,1.0f) };
    DirectX::XMVECTOR up = { DirectX::XMVectorSet(camera.up.x,camera.up.y,camera.up.z,0.0f) };
    V = { DirectX::XMMatrixLookAtLH(eye, focus, up) };
}

void Camera::FixedOrthographic()
{
    D3D11_VIEWPORT viewport{};
    UINT unm_viewports{ 1 };
    Graphics::Instance().GetDeviceContext()->RSGetViewports(&unm_viewports, &viewport);




    DirectX::XMMatrixOrthographicLH(viewport.Width, viewport.Height, 0.1f, 10.0f);
    //DirectX::XMMatrixOrthographicLH(viewport.Width, viewport.Height, 0.1f, 1000.0f);

    DirectX::XMVECTOR eye = { DirectX::XMVectorSet(camera.eye.x,camera.eye.y,camera.eye.z,1.0f) };
    DirectX::XMVECTOR focus = { DirectX::XMVectorSet(camera.focus.x,camera.focus.y,camera.focus.z,1.0f) };
    DirectX::XMVECTOR up = { DirectX::XMVectorSet(camera.up.x,camera.up.y,camera.up.z,0.0f) };
    V = { DirectX::XMMatrixLookAtLH(eye, focus, up) };
}

// SceneGame用
void Camera::GameCameraInitialize()
{
    transform.SetPosition(DirectX::XMFLOAT3(0.0f, 25.0f, 100.0f));
    transform.SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
    transform.SetRotation(DirectX::XMFLOAT4(0.0f, DirectX::XMConvertToRadians(180), 0.0f, 0.0f));
}

// SceneGame用
void Camera::GameCameraSetPerSpectiveFov()
{
    D3D11_VIEWPORT viewport{};
    UINT unm_viewports{ 1 };
    Graphics::Instance().GetDeviceContext()->RSGetViewports(&unm_viewports, &viewport);

    float aspect_ratio{ viewport.Width / viewport.Height };
    P = { DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(30),aspect_ratio,0.1f,1000.0f) };

    DirectX::XMVECTOR eye;
    DirectX::XMVECTOR focus;

    DirectX::XMFLOAT3 pos = transform.GetPosition();
    DirectX::XMFLOAT3 forward = transform.CalcForward();
    eye = { DirectX::XMVectorSet(pos.x, pos.y, pos.z, 1.0f) };
    focus = { DirectX::XMVectorSet(pos.x + forward.x, pos.y + forward.y, pos.z + forward.z, 1.0f) };
    DirectX::XMVECTOR up{ DirectX::XMVectorSet(camera.up.x,camera.up.y,camera.up.z,0.0f) };
    V = { DirectX::XMMatrixLookAtLH(eye, focus, up) };
}

void Camera::DebugSetPerSpectiveFov()
{
    D3D11_VIEWPORT viewport{};
    UINT unm_viewports{ 1 };
    Graphics::Instance().GetDeviceContext()->RSGetViewports(&unm_viewports, &viewport);

    float aspect_ratio{ viewport.Width / viewport.Height };
    P = { DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(30),aspect_ratio,0.1f,1000.0f) };

    DirectX::XMVECTOR eye;
    DirectX::XMVECTOR focus;

    DirectX::XMFLOAT3 pos = transform.GetPosition();
    DirectX::XMFLOAT3 forward = transform.CalcForward();
    eye = { DirectX::XMVectorSet(pos.x, pos.y, pos.z, 1.0f) };
    focus = { DirectX::XMVectorSet(pos.x + forward.x, pos.y + forward.y, pos.z + forward.z, 1.0f) };
    DirectX::XMVECTOR up{ DirectX::XMVectorSet(camera.up.x,camera.up.y,camera.up.z,0.0f) };
    V = { DirectX::XMMatrixLookAtLH(eye, focus, up) };
}

void Camera::Initialize()
{
}

void Camera::Update(float elapsedTime)
{
}

void Camera::UpdateDebug(const float& elapsedTime, DirectX::XMFLOAT2 moveVector)
{
#if 1
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    DirectX::XMFLOAT3 position = transform.GetPosition();
    DirectX::XMFLOAT4 rotation = transform.GetRotation();

    DirectX::XMFLOAT3 forward = transform.CalcForward();
    DirectX::XMFLOAT3 right = transform.CalcRight();

    float ax = gamePad.GetAxisLX() * moveSpeed;
    float ay = gamePad.GetAxisLY() * moveSpeed;

    moveVector.x *= elapsedTime * rotationSpeed;
    moveVector.y *= elapsedTime * rotationSpeed;

    rotation.x += DirectX::XMConvertToRadians(moveVector.y);
    rotation.y += DirectX::XMConvertToRadians(moveVector.x);

    forward.x *= ay;
    forward.y *= ay;
    forward.z *= ay;
    right.x *= ax;
    right.y *= ax;
    right.z *= ax;

    position.x += forward.x + right.x;
    //position.y += forward.y + right.y;
    position.z += forward.z + right.z;

    if (rotation.y >= DirectX::XMConvertToRadians(360))rotation.y -= DirectX::XMConvertToRadians(360);
    if (rotation.y <= DirectX::XMConvertToRadians(0))rotation.y += DirectX::XMConvertToRadians(360);

    transform.SetPosition(position);
    transform.SetRotation(rotation);
#else

    GamePad& gamePad = Input::Instance().GetGamePad();

    float aLx = gamePad.GetAxisLX();
    float aLy = gamePad.GetAxisLY();

    DirectX::XMFLOAT3 position = GetTransform()->GetPosition();
    DirectX::XMFLOAT4 rotation = GetTransform()->GetRotation();

    float speed = elapsedTime * 10;
    if (rotation.y > DirectX::XMConvertToRadians(90) &&
        rotation.y < DirectX::XMConvertToRadians(270))
        speed *= (speed > 0) ? -1 : 1;
    else
        speed *= (speed < 0) ? -1 : 1;

    if (aLx <= -0.3f)position.x -= speed;   // 左
    if (aLx >=  0.3f)position.x += speed;   // 右
    if (aLy <= -0.3f)position.z -= speed;   // 後
    if (aLy >=  0.3f)position.z += speed;   // 前

    if (rotation.y >= DirectX::XMConvertToRadians(360))rotation.y -= DirectX::XMConvertToRadians(360);
    if (rotation.y <= DirectX::XMConvertToRadians(0))rotation.y += DirectX::XMConvertToRadians(360);

    GetTransform()->SetPosition(position);
    GetTransform()->SetRotation(rotation);
#endif
}


void Camera::SetPerspectiveFov(ID3D11DeviceContext* dc)
{    
}

void Camera::DrawDebug()
{
    if (ImGui::BeginMenu("Camera"))
    {
        ImGui::SliderAngle("Fov",&fov);
        if (fov < DirectX::XMConvertToRadians(6))fov = DirectX::XMConvertToRadians(6);

        //ImGui::Begin("Camera");



        transform.DrawDebug();

        ImGui::Checkbox("Enable Debug Camera", &enableDebugCamera);

        if (ImGui::TreeNode("DebugCamera"))
        {
            ImGui::DragFloat("moveSpeed", &moveSpeed);
            ImGui::DragFloat("rotationSpeed", &rotationSpeed);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("camera_information"))
        {
            ImGui::DragFloat3("eye", &camera.eye.x);
            ImGui::DragFloat3("focus", &camera.focus.x);
            ImGui::DragFloat3("up", &camera.up.x);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("player camera"))
        {
            ImGui::Checkbox("Lock On", &activeLockOn);

            ImGui::DragFloat3("Velocity", &velocity.x);
            ImGui::DragFloat3("Eye Pos", &eyePos.x);
            ImGui::DragFloat3("Focus Pos", &focusPos.x);
            ImGui::SliderFloat("Max Speed", &maxEyeSpeed,0.1f,20.0f);
            ImGui::SliderFloat("Acceleration", &acceleration,0.1f,10.0f);

            ImGui::SliderFloat("FocalLength", &focalLength, 0.1f, 20.0f);
            ImGui::SliderFloat("OffsetY", &offsetY, 0.1f, 10.0f);
            ImGui::SliderFloat("FocusOffsetY", &focusOffsetY, 0.1f, 10.0f);
            ImGui::TreePop();
        }

        static float vibVolume;
        static float vibTime;
        if (ImGui::TreeNode("Vibration"))
        {
            ImGui::SliderFloat("VibrationVolume", &vibVolume, 0.0f, 1.0f);
            ImGui::SliderFloat("VibrationTime", &vibTime, 0.0f, 5.0f);
            if (ImGui::Button("Vibrate"))
            {
                ScreenVibrate(vibVolume, vibTime);
            }
            ImGui::TreePop();
        }

        static float fov;
        static float fovTime;
        if (ImGui::TreeNode("Fov Changer"))
        {
            ImGui::SliderAngle("fov",&fov);
            ImGui::SliderFloat("fovTime", &fovTime, 0.01f, 3.0f);

            if (ImGui::Button("change fov"))
            {
                ChangeFov(fov, fovTime);
            }

            if (ImGui::Button("restore fov"))
            {
                RestoreFov(fovTime);
            }

            ImGui::TreePop();
        }

        // todo : camera
        // デバッグ用にカメラを動かしやすいようにしている
        ImGui::DragFloat("camera_position.x", &camera.eye.x);
        ImGui::DragFloat("camera_position.y", &camera.eye.y);


        if (ImGui::Button("Reset"))Camera::Reset();

        //ImGui::End();

        ImGui::EndMenu();
    }
    ImGui::Separator();

}

void Camera::Reset()
{
    camera.eye = { 0.0f,0.0f,-10.0f };
}

void Camera::EyeMoveDelayUpdate(float elapsedTime,
    const DirectX::XMFLOAT3 eyeTargetPos)
{

    DirectX::XMFLOAT3 moveVec = eyeTargetPos - eyePos;
    float length = Length(moveVec);
    moveVec = Normalize(moveVec);

    //カメラ位置と目標位置までの距離が近いなら処理しない
    if (length < 0.1f && length > -0.1f)return;

    velocity = moveVec * length * acceleration;

    //最大速度制限
    length = Length(velocity);
    if (length > maxEyeSpeed)
    {
        velocity = moveVec * maxEyeSpeed;

        length = maxEyeSpeed;
    }

    eyePos = eyePos + velocity * elapsedTime;
}

void Camera::FocusMoveDelayUpdate(float elapsedTime,
    const DirectX::XMFLOAT3 focusTargetPos)
{
    DirectX::XMFLOAT3 moveVec = focusTargetPos - focusPos;
    float length = Length(moveVec);
    moveVec = Normalize(moveVec);

    //カメラ位置と目標位置までの距離が近いなら処理しない
    if (length < 0.1f && length > -0.1f)return;

    focusVelocity = moveVec * length * focusAcceleration;

    //最大速度制限
    length = Length(focusVelocity);
    if (length > maxFocusSpeed)
    {
        focusVelocity = moveVec * maxFocusSpeed;
        length = maxFocusSpeed;
    }

    focusPos = focusPos + focusVelocity * elapsedTime;

}


void Camera::ScreenVibrate(float volume, float effectTime)
{
    vibrationVolume = volume;
    vibrationTimer = effectTime;
    vibrationTime = effectTime;
}

void Camera::ScreenVibrationUpdate(float elapsedTime)
{
    screenVibrationOffset = {};
    if (vibrationTimer <= 0)return;

    //振動方向の指定(乱数)
    DirectX::XMFLOAT3 vibVec;
    auto right = GetTransform()->CalcRight();
    auto up = GetTransform()->CalcUp();

    right = right * (rand() % 100 - 50.0f);
    up = up * (rand() % 100 - 50.0f);

    vibVec = {
        right.x + up.x,
        right.y + up.y,
        0.0f
    };
    vibVec = Normalize(vibVec);

    //イージングを使い経過時間で振動量を調整する
    float vibrationVolume = Easing::InSine(vibrationTimer, vibrationTime, this->vibrationVolume, 0.0f);

    //振動値を入れる
    screenVibrationOffset = vibVec * vibrationVolume;

    vibrationTimer -= elapsedTime;
}

void Camera::UpdateFov(float elapsedTime)
{
    if (isChangingFov)
    {
        fovTimer += elapsedTime;

        if (fovTimer >= fovTime)
        {
            fovTimer = fovTime;
        }

        fov = Easing::OutCubic(fovTimer, fovTime, resultFov, holdFov);

        if (fovTimer >= fovTime)
        {
            isChangingFov = false;
        }
    }
}

void Camera::ChangeFov(float fov,float time)
{
    resultFov = fov;
    holdFov = this->fov;

    fovTime = time;
    fovTimer = 0;
    isChangingFov = true;
}

void Camera::RestoreFov(float time)
{
    resultFov = initFov;
    holdFov = fov;

    fovTime = time;
    fovTimer = 0;
    isChangingFov = true;
}