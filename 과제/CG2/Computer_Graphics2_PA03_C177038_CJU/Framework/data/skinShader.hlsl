#define NUM_LIGHTS 4

cbuffer CameraBuffer : register(b0)
{
    float4x4 matView;
    float4x4 matProj;
    float3 cameraPosition;
    float padding0;
};

cbuffer TransBuffer : register(b1)
{
    float4x4 matWorld;
};

cbuffer ColorBuffer : register(b2)
{
    float4 fColor;
};

cbuffer BoneBuffer : register(b3)
{
    float4x4 matBones[128];
};


cbuffer LightBuffer : register(b4)
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
};

cbuffer LightPositionBuffer : register(b5)
{
    float4 lightPosition[NUM_LIGHTS];
};

cbuffer LightColorBuffer : register(b6)
{
    float4 diffuseColorPoint[NUM_LIGHTS];
    float4 specularColorPoint[NUM_LIGHTS];
};

float4x4 IdentityMatrix =
{
    { 1, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 1 }
};

struct VertexInputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD;
    uint4 boneIdx : BONEID;
    float4 weight : WEIGHT;
    
    float3 normal : NORMAL;
    
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD;
    
    float3 normal : NORMAL;
    
    float3 viewDirection : TEXCOORD1;

    float3 lightPos1 : TEXCOORD2;
    float3 lightPos2 : TEXCOORD3;
    float3 lightPos3 : TEXCOORD4;
    float3 lightPos4 : TEXCOORD5;
};

Texture2D shaderTexture;
SamplerState SampleType;

// Vertex Shader
PixelInputType vsMain(VertexInputType input)
{
    PixelInputType output;
	
  // ============================
    // 1) Skinning
    // ============================
    float finalWeight = 1 - (input.weight[0] + input.weight[1] + input.weight[2]);

    float4x4 boneTransform = matBones[input.boneIdx[0]] * input.weight[0];
    boneTransform += matBones[input.boneIdx[1]] * input.weight[1];
    boneTransform += matBones[input.boneIdx[2]] * input.weight[2];
    boneTransform += matBones[input.boneIdx[3]] * finalWeight;

    float4 localPos = float4(input.position, 1.0f);
    float4 skinnedPos = mul(localPos, boneTransform);

    // ============================
    // 2) World Transform
    // ============================
    float4 worldPos = mul(skinnedPos, matWorld);

    // ============================
    // 3) Normal Transform
    // ============================
    float3 skinnedNormal = mul(input.normal, (float3x3) boneTransform);
    skinnedNormal = mul(skinnedNormal, (float3x3) matWorld);
    output.normal = normalize(skinnedNormal);

    // ============================
    // 4) View Direction
    // ============================
    output.viewDirection = normalize(cameraPosition - worldPos.xyz);

    // ============================
    // 5) Point Light Direction
    // ============================
    output.lightPos1 = lightPosition[0].xyz - worldPos.xyz;
    output.lightPos2 = lightPosition[1].xyz - worldPos.xyz;
    output.lightPos3 = lightPosition[2].xyz - worldPos.xyz;
    output.lightPos4 = lightPosition[3].xyz - worldPos.xyz;

    // ============================
    // 6) Clip Space Transform
    // ============================
    float4 viewPos = mul(worldPos, matView);
    output.position = mul(viewPos, matProj);

    // ============================
    // 7) UV 전달
    // ============================
    output.tex = input.tex;

    return output;
}

// Pixel Shader
float4 psMain(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float3 reflection;
    float4 specular;
    
    float lightIntensity1, lightIntensity2, lightIntensity3, lightIntensity4;
    float4 color, color1, color2, color3, color4;
    
    float4 spec1, spec2, spec3, spec4;
    
    // Ambient base
    color = ambientColor;
    specular = float4(0, 0, 0, 0);

    lightDir = normalize(-lightDirection);
    lightIntensity = saturate(dot(input.normal, lightDir));

    if (lightIntensity > 0.0f)
    {
        color += diffuseColor * lightIntensity;
        reflection = normalize(2 * lightIntensity * input.normal - lightDir);
        specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower) * specularColor;
    }

    float dist1 = length(input.lightPos1);
    float dist2 = length(input.lightPos2);
    float dist3 = length(input.lightPos3);
    float dist4 = length(input.lightPos4);

    // 거리 감쇠 (1 / (1 + k1*d + k2*d²))
    float att1 = (1.0f / (1.0f + 1.0f * dist1 + 0.07f * dist1 * dist1));
    float att2 = (1.0f / (1.0f + 1.0f * dist2 + 0.07f * dist2 * dist2));
    float att3 = (1.0f / (1.0f + 1.0f * dist3 + 0.07f * dist3 * dist3));
    float att4 = (1.0f / (1.0f + 1.0f * dist4 + 0.07f * dist4 * dist4));


    // Diffuse
    //lightIntensity1 = saturate(dot(input.normal, normalize(input.lightPos1))) * att1;
    //lightIntensity2 = saturate(dot(input.normal, normalize(input.lightPos2))) * att2;
    //lightIntensity3 = saturate(dot(input.normal, normalize(input.lightPos3))) * att3;
    //lightIntensity4 = saturate(dot(input.normal, normalize(input.lightPos4))) * att4;
    
    lightIntensity1 = saturate(dot(input.normal, normalize(input.lightPos1)));
    lightIntensity2 = saturate(dot(input.normal, normalize(input.lightPos2)));
    lightIntensity3 = saturate(dot(input.normal, normalize(input.lightPos3)));
    lightIntensity4 = saturate(dot(input.normal, normalize(input.lightPos4)));

    color1 = diffuseColorPoint[0] * lightIntensity1;
    color2 = diffuseColorPoint[1] * lightIntensity2;
    color3 = diffuseColorPoint[2] * lightIntensity3;
    color4 = diffuseColorPoint[3] * lightIntensity4;

    //spec1 = pow(saturate(dot(normalize(reflect(-normalize(input.lightPos1), input.normal)), input.viewDirection)), specularPower / 2)
    //     * specularColorPoint[0] * att1;
    //spec2 = pow(saturate(dot(normalize(reflect(-normalize(input.lightPos2), input.normal)), input.viewDirection)), specularPower / 2)
    //    * specularColorPoint[1] * att2;
    //spec3 = pow(saturate(dot(normalize(reflect(-normalize(input.lightPos3), input.normal)), input.viewDirection)), specularPower / 2)
    //    * specularColorPoint[2] * att3;
    //spec4 = pow(saturate(dot(normalize(reflect(-normalize(input.lightPos4), input.normal)), input.viewDirection)), specularPower / 2)
    //     * specularColorPoint[3] * att4;
    
    
    spec1 = pow(saturate(dot(normalize(reflect(-normalize(input.lightPos1), input.normal)), input.viewDirection)), specularPower / 2)
         * specularColorPoint[0];
    spec2 = pow(saturate(dot(normalize(reflect(-normalize(input.lightPos2), input.normal)), input.viewDirection)), specularPower / 2)
        * specularColorPoint[1];
    spec3 = pow(saturate(dot(normalize(reflect(-normalize(input.lightPos3), input.normal)), input.viewDirection)), specularPower / 2)
        * specularColorPoint[2];
    spec4 = pow(saturate(dot(normalize(reflect(-normalize(input.lightPos4), input.normal)), input.viewDirection)), specularPower / 2)
         * specularColorPoint[3];

    //textureColor = shaderTexture.Sample(SampleType, input.tex);
    //color += saturate(color1 + color2 + color3 + color4);
    //color = saturate(color * textureColor + specular);

    textureColor = shaderTexture.Sample(SampleType, input.tex);
    color += saturate(color1 + color2 + color3 + color4);
    specular += saturate(spec1 + spec2 + spec3 + spec4);
    color = saturate(color * textureColor + specular);
    
    return color;
}
