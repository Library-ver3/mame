#include "FullScreenQuad.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2

SamplerState samplerStates[3] : register(s0);
Texture2D colorMap : register(t0);
Texture2D bloomMap : register(t1);

float3 reinhardToneMapping(float3 color)
{
    float luma = dot(color, float3(0.2126, 0.7152, 0.0722));
    float toneMappedLuma = luma / (1. + luma);
    color *= toneMappedLuma / luma;
    return color;
}

float4 main(PSIn psIn) : SV_TARGET
{
    float4 color = colorMap.Sample(samplerStates[LINEAR], psIn.texcoord);
    float4 bloom = bloomMap.Sample(samplerStates[LINEAR], psIn.texcoord);
	
    float3 finalColor = color.rgb + bloom.rgb;
    float alpha = color.a;
    
    // Tone Map
    finalColor = reinhardToneMapping(finalColor);
    
    return float4(finalColor, alpha);
}