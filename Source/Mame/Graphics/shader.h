#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <windows.h>
#include <memory>
#include <wrl.h>

#include "../Graphics/Camera.h"


// SHADOW
#include "../Graphics/ShadowMap.h"

#define POINT_LIGHT_ONE 1

#if !POINT_LIGHT_ONE
const int pointLightMax = 8;
#endif

class Shader
{
public: // enum�֘A
    enum class SAMPLER_STATE {
        POINT, LINEAR, ANISOTROPIC, LINEAR_BORDER_BLACK, LINEAR_BORDER_WHITE, COMPARISON_LINEAR_BORDER_WHITE/*SHADOW*/,
        LINEAR_BORDER_OPAQUE_BLACK, POINT_CLAMP, COUNT,
    };
    //enum class SAMPLER_STATE {
    //    POINT_WRAP, LINEAR_WRAP, ANISOTROPIC_WRAP, POINT_CLAMP, LINEAR_CLAMP, ANISOTROPIC_CLAMP,
    //    POINT_BORDER_OPAQUE_BLACK, LINEAR_BORDER_OPAQUE_BLACK, POINT_BORDER_OPAQUE_WHITE, LINEAR_BORDER_OPAQUE_WHITE,
    //    COMPARISON_DEPTH, COUNT
    //};
    
    enum class DEPTH_STATE { ZT_ON_ZW_ON, ZT_ON_ZW_OFF, ZT_OFF_ZW_ON, ZT_OFF_ZW_OFF };
    enum class BLEND_STATE { NONE, ALPHA, ADD, MULTIPLY };
    enum class RASTER_STATE { SOLID, WIREFRAME, CULL_NONE, WIREFRAME_CULL_NONE };

    enum class CONSTANT_BUFFER 
    {
        CONSTANT_BUFFER_PARAMETRIC,
        LIGHT_CONSTANT,
        FOG_CONSTANT,
        POST_EFFECT_CONSTANT,
    };

public:
    struct View
    {
        float viewWitdh = 100;
        float viewHeight = 100;
        float nearZ = 0;
        float farZ = 100;

        DirectX::XMFLOAT4 position{ 0, -4, -5, 0 };
        DirectX::XMFLOAT4 camera{ 0, 0, 1, 0 };
    }view;

    struct SceneConstants
    {
        DirectX::XMFLOAT4X4 viewProjection;
        DirectX::XMFLOAT4 lightDirection;
        DirectX::XMFLOAT4 cameraPosition;

        // SHADOW
        DirectX::XMFLOAT4X4 lightViewProjection;

        // FOG
        DirectX::XMFLOAT4X4 inverseProjection;
        DirectX::XMFLOAT4X4 inverseViewProjection;
        float time;

        float pads[3];
    };

    // FOG
    struct FogConstants
    {
        DirectX::XMFLOAT4 fogColor = { 1.000f, 1.000f, 1.000f, 0.894f }; // w: fog intensuty
        float fogDensity = 0.0005f;
        //float fogHeightFalloff = 0.9313f;
        float fogHeightFalloff = 10.0f;
        float startDistance = 5.00f;
        float fogCutoffDistance = 500.0f;
        float timeScale = 0.5f;
        float speedScale = 0.2f;
        float pads[2];
    }fogConstants;

    // DIRECTION_LIGHT
    struct DirectionLight
    {
        DirectX::XMFLOAT4 direction;    // ���C�g�̕���
        DirectX::XMFLOAT4 color;        // ���C�g�̈ʒu
    };

    // POINT_LIGHT
    struct PointLight
    {
        DirectX::XMFLOAT4 position = {};    // �|�C���g���C�g�̈ʒu
        DirectX::XMFLOAT4 color = {};       // �|�C���g���C�g�̐F
        float range = 0;                    // �|�C���g���C�g�̉e���͈�
        DirectX::XMFLOAT3 step = {};        // �_�~�[
    };


    // SPOT_LIGHT
    struct SpotLight
    {
        DirectX::XMFLOAT4 position = {};    // �X�|�b�g���C�g�̈ʒu
        DirectX::XMFLOAT4 color = {};       // �X�|�b�g���C�g�̐F
        float range = 0;                    // �X�|�b�g���C�g�̉e���͈�
        DirectX::XMFLOAT3 step = {};        // �_�~�[
        DirectX::XMFLOAT3 direction = {};   // �X�|�b�g���C�g�̎ˏo����
        float angle = 0;                    // �X�|�b�g���C�g�̎ˏo�p�x
    };

    // HEMISPHERE_LIGHT
    struct HemisphereLight
    {
        DirectX::XMFLOAT4 groundColor;  // �n�ʐF
        DirectX::XMFLOAT4 skyColor;     // �V���F
        DirectX::XMFLOAT4 groundNormal; // �n�ʂ̖@��
    };

    struct LightConstants
    {
        DirectionLight directionLight;      // �f�B���N�V�������C�g
        
#if POINT_LIGHT_ONE
        PointLight pointLight;              // �|�C���g���C�g
#else
        PointLight pointLight[pointLightMax];
#endif// POINT_LIGHT_ONE

        SpotLight spotLight;                // �X�|�b�g���C�g
        HemisphereLight hemisphereLight;    // �������C�g
    }lightConstant;

    struct PostEffectConstants
    {
        DirectX::XMFLOAT4 shiftSize = {};

        DirectX::XMFLOAT4 noiseColor{ 1.0f,1.0f,1.0f,1.0f };
        float noiseTimer = 0.0f;
        float scanLineTimer = 0.0f;
        
        float bokehAperture = 0.0f;
        float bokehFocus = 0.3f;

        float vignetteValue = 0.2f;
        DirectX::XMFLOAT3 dummy;
    }postEffectConstants;

public:
    Shader(ID3D11Device* device);
    ~Shader() {}

    void Initialize();

    // �`��J�n
    void Begin(ID3D11DeviceContext* dc);


    void DrawDebug();

public:// �萔�o�b�t�@�[
    void UpdatePostEffectConstants(int slot);
    void UpdateFogConstants(int slot);


public:// �e��X�e�[�g�ݒ�
    void SetDepthStencileState(DEPTH_STATE depthStencileState);
    void SetBlendState(BLEND_STATE blendState);
    void SetRasterizerState(RASTER_STATE rasterizerState);

    void SetSamplerState(ID3D11DeviceContext* deviceContext);

public:
    const DirectX::XMFLOAT4 GetViewPosition() { return view.position; }
    const DirectX::XMFLOAT4 GetViewCamera() { return view.camera; }


private:

    static const int MaxBones = 128;

    Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer[10];



    struct CBMesh
    {
        DirectX::XMFLOAT4X4 boneTransformas[MaxBones];
    };

    struct CBSubset
    {
        DirectX::XMFLOAT4 materialColor;
    };

    struct CBParametric
    {
        float extraction_threshold{ 0.8f };
        float gaussian_sigma{ 1.0f };
        float bloom_intensity{ 1.0f };
        float exposure{ 1.0f };
    };



    Microsoft::WRL::ComPtr<ID3D11Buffer> meshConstantBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> subsetConstantBuffer;

    Microsoft::WRL::ComPtr<ID3D11VertexShader>  vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>   pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>   inputLayout;

    Microsoft::WRL::ComPtr<ID3D11BlendState>        blendStates[4];
    Microsoft::WRL::ComPtr<ID3D11RasterizerState>   rasterizerStates[4];
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[4];

    Microsoft::WRL::ComPtr<ID3D11SamplerState>  samplerState[static_cast<UINT>(SAMPLER_STATE::COUNT)];
};


HRESULT CreateVsFromCso(ID3D11Device* device, const char* cso_name, ID3D11VertexShader** vertex_shader,
    ID3D11InputLayout** input_layout, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements);
HRESULT CreatePsFromCso(ID3D11Device* device, const char* cso_name, ID3D11PixelShader** pixel_shader);

HRESULT CreateGsFromCso(ID3D11Device* device, const char* csoName, ID3D11GeometryShader** geometryShader);
HRESULT CreateCsFromCso(ID3D11Device* device, const char* csoName, ID3D11ComputeShader** computeShader);

HRESULT CreateDsFromCso(ID3D11Device* device, const char* csoName, ID3D11DomainShader** domainShader);
HRESULT CreateHsFromCso(ID3D11Device* device, const char* csoName, ID3D11HullShader** hullShader);