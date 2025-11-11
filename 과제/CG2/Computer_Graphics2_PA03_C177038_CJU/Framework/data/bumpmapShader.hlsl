
#define NUM_LIGHTS 4
// GLOBALS //
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
    float padding;
};

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;

};

cbuffer LightPositionBuffer
{
    float4 lightPosition[NUM_LIGHTS];
};

cbuffer LightColorBuffer
{
    float4 diffuseColorPoint[NUM_LIGHTS];
    
    float4 specularColorPoint[NUM_LIGHTS];
};

Texture2D shaderTextures[4];
SamplerState SampleType;

// TYPEDEFS //
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
   	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
    
    float3 viewDirection : TEXCOORD1;
    
    
    
    float3 lightPos1 : TEXCOORD2;
    float3 lightPos2 : TEXCOORD3;
    float3 lightPos3 : TEXCOORD4;
    float3 lightPos4 : TEXCOORD5;
    
};

float3 SafeNormalize(float3 v)
{
    float len = length(v);
    return (len > 0.0001f) ? (v / len) : float3(0, 0, 0);
}


// Vertex Shader
PixelInputType BumpMapVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;
    

	// 적절한 행렬 계산을 위해 위치 벡터를 4 단위로 변경합니다.
    input.position.w = 1.0f;

	// 월드, 뷰 및 투영 행렬에 대한 정점의 위치를 ​​계산합니다.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// 픽셀 쉐이더의 텍스처 좌표를 저장한다.
	output.tex = input.tex;
    
    // 월드 행렬에 대해서만 법선 벡터를 계산 한 다음 최종 값을 정규화합니다.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);

	// 월드 행렬에 대해서만 접선 벡터를 계산 한 다음 최종 값을 정규화합니다.
    output.tangent = mul(input.tangent, (float3x3)worldMatrix);
    output.tangent = normalize(output.tangent);

    // 세계 행렬에 대해서만 비 유효 벡터를 계산 한 다음 최종 값을 정규화합니다.
    output.binormal = mul(input.binormal, (float3x3)worldMatrix);
    output.binormal = normalize(output.binormal);
    
    worldPosition = mul(input.position, worldMatrix);
    
    // Determine the light positions based on the position of the lights and the position of the vertex in the world.
    output.lightPos1.xyz = lightPosition[0].xyz - worldPosition.xyz;
    output.lightPos2.xyz = lightPosition[1].xyz - worldPosition.xyz;
    output.lightPos3.xyz = lightPosition[2].xyz - worldPosition.xyz;
    output.lightPos4.xyz = lightPosition[3].xyz - worldPosition.xyz;

    // Normalize the light position vectors.
    output.lightPos1 = SafeNormalize(output.lightPos1);
    output.lightPos2 = SafeNormalize(output.lightPos2);
    output.lightPos3 = SafeNormalize(output.lightPos3);
    output.lightPos4 = SafeNormalize(output.lightPos4);
    
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	
    // Normalize the viewing direction vector.
    output.viewDirection = SafeNormalize(output.viewDirection);
    
    return output;
}

// Pixel Shader
float4 BumpMapPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
    float4 textureColor2;
    
    float4 bumpMap;
    float3 bumpNormal;
    
    float3 lightDir;
    float lightIntensity;
    
    float4 alphaValue;
    float4 blendColor;
    
    float3 reflection;
    float4 specular;
    
    float lightIntensity1, lightIntensity2, lightIntensity3, lightIntensity4;
    
    float4 color, color1, color2, color3, color4;
    
    float4 spec1, spec2, spec3, spec4;
    
    // 이 위치에서 텍스처 픽셀을 샘플링합니다.
    textureColor = shaderTextures[0].Sample(SampleType, input.tex);
	
    textureColor2 = shaderTextures[1].Sample(SampleType, input.tex);
    
    // 알파 맵 텍스처에서 알파 값을 가져옵니다.
    alphaValue = shaderTextures[2].Sample(SampleType, input.tex);
    
    // 범프 맵에서 픽셀을 샘플링합니다.
    bumpMap = shaderTextures[3].Sample(SampleType, input.tex);
    
    
    float alpha = alphaValue.r; // 알파맵의 R 채널만 사용
    blendColor = (alpha * 1.5f * textureColor) + ((1.0f - alpha) * textureColor2);
    
    // 정상 값의 범위를 (0, +1)에서 (-1, +1)로 확장합니다.
    bumpMap = (bumpMap * 2.0f) - 1.0f;
    
    // 범프 맵의 데이터에서 법선을 계산합니다.
    bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);
    
    bumpNormal = -bumpNormal;
	
    // 결과 범프 법선을 표준화합니다.
    bumpNormal = SafeNormalize(bumpNormal);
    
    
    color = ambientColor;
    specular = float4(0, 0, 0, 0);

    lightDir = SafeNormalize(-lightDirection);
    lightIntensity = saturate(dot(input.normal, lightDir));

    // 계산을 위해 빛 방향을 반전시킵니다.
    lightDir = -lightDirection;

    // 범프 맵 일반 값을 기반으로 픽셀의 빛의 양을 계산합니다.
    lightIntensity = saturate(dot(bumpNormal, lightDir));
    
    if (lightIntensity > 0.0f)
    {
        color += diffuseColor * lightIntensity;
        reflection = normalize(reflect(-lightDir, bumpNormal));
        specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower) * specularColor;
    }
    
    if (any(isnan(bumpNormal)) || any(isnan(reflection)))
    {
        return float4(1, 0, 0, 1); // 빨간색 경고 출력
    }
    
    float dist1 = max(length(input.lightPos1), 0.001f);
    float dist2 = max(length(input.lightPos2), 0.001f);
    float dist3 = max(length(input.lightPos3), 0.001f);
    float dist4 = max(length(input.lightPos4), 0.001f);

    // 거리 감쇠 (1 / (1 + k1*d + k2*d²))
    float att1 =  saturate(1.0f / (1.0f + 10.0f * dist1 + 0.07f * dist1 * dist1));
    float att2 = saturate(1.0f / (1.0f + 10.0f * dist2 + 0.07f * dist2 * dist2));
    float att3 = saturate(1.0f / (1.0f + 10.0f * dist3 + 0.07f * dist3 * dist3));
    float att4 = saturate(1.0f / (1.0f + 10.0f * dist4 + 0.07f * dist4 * dist4));
    
    lightIntensity1 = saturate(dot(input.normal, SafeNormalize(input.lightPos1)));
    lightIntensity2 = saturate(dot(input.normal, SafeNormalize(input.lightPos2)));
    lightIntensity3 = saturate(dot(input.normal, SafeNormalize(input.lightPos3)));
    lightIntensity4 = saturate(dot(input.normal, SafeNormalize(input.lightPos4)));

    color1 = diffuseColorPoint[0] * lightIntensity1 * att1;
    color2 = diffuseColorPoint[1] * lightIntensity2 * att2;
    color3 = diffuseColorPoint[2] * lightIntensity3 * att3;
    color4 = diffuseColorPoint[3] * lightIntensity4 * att4;

    spec1 = pow(saturate(dot(SafeNormalize(reflect(-SafeNormalize(input.lightPos1), input.normal)), input.viewDirection)), specularPower / 2)
         * specularColorPoint[0] * att1;
    spec2 = pow(saturate(dot(SafeNormalize(reflect(-SafeNormalize(input.lightPos2), input.normal)), input.viewDirection)), specularPower / 2)
        * specularColorPoint[1] * att2;
    spec3 = pow(saturate(dot(SafeNormalize(reflect(-SafeNormalize(input.lightPos3), input.normal)), input.viewDirection)), specularPower / 2)
        * specularColorPoint[2] * att3;
    spec4 = pow(saturate(dot(SafeNormalize(reflect(-SafeNormalize(input.lightPos4), input.normal)), input.viewDirection)), specularPower / 2)
         * specularColorPoint[3] * att4;
    
    color += saturate(color1 + color2 + color3 + color4);
    specular += saturate(spec1 + spec2 + spec3 + spec4);
    color = saturate(color * blendColor + specular);
	
    return color;
}
