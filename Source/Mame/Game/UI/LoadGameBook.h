#pragma once

#include "../GameObject.h"

class LoadGameBook : public GameObject
{
public:// äÓñ{ìIÇ»ä÷êî
    LoadGameBook();
    ~LoadGameBook() override;

    void Initialize() override;
    void Finalize() override;
    void Update(const float& elapsedTime) override;
    void Render();

    void DrawDebug() override;

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> psShader_;
};

