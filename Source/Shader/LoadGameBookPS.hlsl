#include "FbxModel.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2

SamplerState samplerStates[3] : register(s0);
Texture2D colorMap : register(t0);

float4 main(PSIn psIn) : SV_TARGET
{
    float4 color = colorMap.Sample(samplerStates[LINEAR], psIn.texcoord);
	
    return color;
}