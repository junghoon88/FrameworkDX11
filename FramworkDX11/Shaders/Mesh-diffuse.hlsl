#include "CommonHeader.hlsl"

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

cbuffer PS_Color : register(b2)
{
    float4 _color : COLOR0;
    uint _viewMode : MODE0;
    float3 padding : PADDING0;
}



PixelInput VS(VertexTextureNormal input)
{
    PixelInput output;

    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.normal = GetWorldNormal(input.normal, _world);
    
    output.uv = input.uv;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 color = _diffuse * _shininess * _detailFactor * 0.01f;
    if (_viewMode == 1)         color = float4(1, 1, 0, 1);

    return color;
}