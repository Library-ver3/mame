#include "FbxModel.hlsli"

// �萔
#define POINT_LIGHT 1
#define SPOT_LIGHT 0
#define LIM_LIGHT 0
#define HEMISPHERE_LIGHT 1

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2

SamplerState samplerStates[5] : register(s0);
Texture2D textureMaps[4] : register(t0);

// SHADOW
SamplerComparisonState comparisonSamplerState : register(s5);
Texture2D shadowMap : register(t8);

// DISSOLVE
Texture2D maskTexture : register(t15);



float4 main(PSIn psIn) : SV_TARGET
{
    float mask_value = maskTexture.Sample(samplerStates[0], psIn.texcoord) * psIn.color;

    float4 color = textureMaps[0].Sample(samplerStates[ANISOTROPIC], psIn.texcoord);
    float alpha = color.a;


    const float GAMMA = 2.2;
    color.rgb = pow(color.rgb, GAMMA);


//    float3 N = normalize(psIn.worldNormal.xyz);
//#if 1
//    float3 T = normalize(psIn.worldTangent.xyz);
//    float sigma = psIn.worldTangent.w;
//    T = normalize(T - N * dot(N, T));
//    float3 B = normalize(cross(N, T) * sigma);

//    float4 normal = textureMaps[1].Sample(samplerStates[LINEAR], psIn.texcoord);
//    normal = (normal * 2.0) - 1.0;
//    N = normalize((normal.x * T) + (normal.y * B) + (normal.z * N));
//#endif
    

    // ������������{�ɍ���Ă݂�
    float3 directionLight = color.rgb * CalcLightFromDirectionLight(psIn);

    // dissolve
#if 0
    alpha = step(parameters.x, mask_value);
#endif

    // SHADOW
#if 1
    const float shadowDepthBias = 0.01;
#else
    // �����ł́A�T�[�t�F�X�̖@���ƌ��̕����Ɋ�Â��āA�o�C�A�X�̍ő�l�� 0.01�A�ŏ��l�� 0.001 �ɂȂ��Ă��܂��B
    const float shadowDepthBias = max(0.01 * (1.0 - dot(N, L)), 0.001);
#endif

    float4 lightViewPosition = mul(psIn.worldPosition, lightViewProjection);
    lightViewPosition = lightViewPosition / lightViewPosition.w;
    float2 lightViewTexcoord = 0;
    lightViewTexcoord.x = lightViewPosition.x * 0.5 + 0.5;
    lightViewTexcoord.y = lightViewPosition.y * -0.5 + 0.5;
    float depth = saturate(lightViewPosition.z - shadowDepthBias);
     
    
    // SHADOW
    float shadowWidth;
    float shadowHight;
    shadowMap.GetDimensions(shadowWidth, shadowHight);

    float shadowWidthOffset = 1 / shadowWidth;
    float shadowHightOffset = 1 / shadowHight;
    
    float3 shadowFactor = 1.0f;
    // shdowFactor�ЂƂ������ƁA�܂�������ĕ��ϒl���o�����Ƃł��ꂢ�Ɍ�����B
    {
        shadowFactor = shadowMap.SampleCmpLevelZero(comparisonSamplerState, lightViewTexcoord, depth).xxx;
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x + shadowWidthOffset, lightViewTexcoord.y), depth).xxx; // �E
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x - shadowWidthOffset, lightViewTexcoord.y), depth).xxx; // ��
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x, lightViewTexcoord.y - shadowHightOffset), depth).xxx; // ��
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x, lightViewTexcoord.y + shadowHightOffset), depth).xxx; // ��
        
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x + shadowWidthOffset, lightViewTexcoord.y - shadowHightOffset), depth).xxx; // �E��
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x - shadowWidthOffset, lightViewTexcoord.y - shadowHightOffset), depth).xxx; // ����
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x + shadowWidthOffset, lightViewTexcoord.y + shadowHightOffset), depth).xxx; // �E��
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x - shadowWidthOffset, lightViewTexcoord.y + shadowHightOffset), depth).xxx; // ����
        
        shadowFactor /= 9;
        //shadowFactor /= 2;
    }
    
    // �ŏI�I�ȃJ���[
    float3 finalColor = directionLight;
    
    
    // HEMISPHERE_LIGHT
#if HEMISPHERE_LIGHT
    float3 hemisphereLight = color.rgb * CalcHemisphereLight(psIn);
    finalColor += hemisphereLight;
#endif

    // �����ł̓��f���̓񎟔��ˌ����^���I�ɏo��
    finalColor.x += 0.2f;
    finalColor.y += 0.2f;
    finalColor.z += 0.2f;
    
    return float4(max(0, finalColor * shadowFactor), alpha) * psIn.color;
}