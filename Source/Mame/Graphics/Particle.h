#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>

#include <vector>
#include "../Resource/SkinnedMesh.h"

#define NUMTHREADS_X 16

struct Particle
{
    DirectX::XMFLOAT4 color{ 1, 1, 1, 1 };
    DirectX::XMFLOAT3 position{ 0, 0, 0 };
    DirectX::XMFLOAT3 velocity{ 0, 0, 0 };
    float age{};
    int state{};
};

struct Particles
{
    const size_t maxParticleCount;
    struct ParticleConstants
    {
        DirectX::XMFLOAT3 emitterPosition{};
        float particleSize{ 0.02f };
        DirectX::XMFLOAT4 color{ 0.1f,0.1f,1.0f,1.0f };
        float time{};
        float deltaTime{};
        
        DirectX::XMFLOAT2 scrollDirection{ 1.0f, 0.0f };    // UVScroll����
        float options = 0.0f;                               // UVScroll�p�^�C�}�[
                
        DirectX::XMFLOAT2 dissolveParameters{}; // x : �f�B�]���u�K���ʁA y : ����臒l
        float something = 0; // �_�~�[

        DirectX::XMFLOAT4 edgeColor{};   // ���̐F

    };
    ParticleConstants particleData;

    Microsoft::WRL::ComPtr<ID3D11Buffer> particleBuffer;
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> particleBufferUav;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> particleBufferSrv;

    Microsoft::WRL::ComPtr<ID3D11VertexShader> particleVS;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> particlePS;
    Microsoft::WRL::ComPtr<ID3D11GeometryShader> particleGS;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    Microsoft::WRL::ComPtr<ID3D11ComputeShader> particleCS;
    Microsoft::WRL::ComPtr<ID3D11ComputeShader> particleInitializerCS;
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

    Particles(ID3D11Device* device, size_t particleCount);
    Particles& operator=(const Particles&) = delete;
    Particles(Particles&&) noexcept = delete;
    Particles& operator=(Particles&&) noexcept = delete;
    virtual ~Particles() = default;

    void Integrate(ID3D11DeviceContext* deviceContext, float deltaTime);
    void Initialize(ID3D11DeviceContext* deviceContext, float deltaTime);
    void Render(ID3D11DeviceContext* deviceContext);
    void DrawDebug();
};