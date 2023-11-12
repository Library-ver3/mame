#pragma once
#include "../Game/GameObject.h"

class LoadingPlayer : public GameObject
{
public:
    LoadingPlayer();
    ~LoadingPlayer() override {}

    void Initialize()                           override;
    void Finalize()                             override;
    void Update(const float& elapsedTime)       override;
    void Render(const float& scale, 
        ID3D11PixelShader* psShader = nullptr)  override;
    void DrawDebug()                            override;

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> psShader_ = nullptr;

    std::unique_ptr<GameObject> sword_ = nullptr;
};

