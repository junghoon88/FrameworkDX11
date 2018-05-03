#include "CommonHeader.hlsl"

cbuffer PS_Sky : register(b3)
{
    float4 Center;
    float4 Apex;
    float  Height;
    float3 PaddingPS2;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 wPosition : OUTPUT0;
};


PixelInput VS(VertexTextureNormal input)
{
    PixelInput output;

    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.wPosition = input.position;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float height = saturate(input.wPosition.y);

    return lerp(Center, Apex, height * Height);
    //return float4(height, height, height, 1);
}