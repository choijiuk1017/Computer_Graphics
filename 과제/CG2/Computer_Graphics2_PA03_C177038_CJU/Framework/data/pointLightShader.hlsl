// DEFINES //
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

Texture2D shaderTexture;
SamplerState SampleType;

// TYPEDEFS //
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    
    float3 viewDirection : TEXCOORD1;
    
    
    
    float3 lightPos1 : TEXCOORD2;
    float3 lightPos2 : TEXCOORD3;
    float3 lightPos3 : TEXCOORD4;
    float3 lightPos4 : TEXCOORD5;
};

// Vertex Shader
PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;


	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3) worldMatrix);
	
	// Normalize the normal vector.
    output.normal = normalize(output.normal);
	
    // Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix);

    // Determine the light positions based on the position of the lights and the position of the vertex in the world.
    output.lightPos1.xyz = lightPosition[0].xyz - worldPosition.xyz;
    output.lightPos2.xyz = lightPosition[1].xyz - worldPosition.xyz;
    output.lightPos3.xyz = lightPosition[2].xyz - worldPosition.xyz;
    output.lightPos4.xyz = lightPosition[3].xyz - worldPosition.xyz;
    
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	
    // Normalize the viewing direction vector.
    output.viewDirection = normalize(output.viewDirection);

    return output;
}

// Pixel Shader
float4 LightPixelShader(PixelInputType input) : SV_TARGET
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

    // °Å¸® °¨¼è (1 / (1 + k1*d + k2*d©÷))
    float att1 = (1.0f / (1.0f + 1.0f * dist1 + 0.07f * dist1 * dist1));
    float att2 = (1.0f / (1.0f + 1.0f * dist2 + 0.07f * dist2 * dist2));
    float att3 = (1.0f / (1.0f + 1.0f * dist3 + 0.07f * dist3 * dist3));
    float att4 = (1.0f / (1.0f + 1.0f * dist4 + 0.07f * dist4 * dist4));


    // Diffuse
    lightIntensity1 = saturate(dot(input.normal, normalize(input.lightPos1))) * att1;
    lightIntensity2 = saturate(dot(input.normal, normalize(input.lightPos2))) * att2;
    lightIntensity3 = saturate(dot(input.normal, normalize(input.lightPos3))) * att3;
    lightIntensity4 = saturate(dot(input.normal, normalize(input.lightPos4))) * att4;
    
    //lightIntensity1 = saturate(dot(input.normal, normalize(input.lightPos1)));
    //lightIntensity2 = saturate(dot(input.normal, normalize(input.lightPos2)));
    //lightIntensity3 = saturate(dot(input.normal, normalize(input.lightPos3)));
    //lightIntensity4 = saturate(dot(input.normal, normalize(input.lightPos4)));

    color1 = diffuseColorPoint[0] * lightIntensity1;
    color2 = diffuseColorPoint[1] * lightIntensity2;
    color3 = diffuseColorPoint[2] * lightIntensity3;
    color4 = diffuseColorPoint[3] * lightIntensity4;

    spec1 = pow(saturate(dot(normalize(reflect(-normalize(input.lightPos1), input.normal)), input.viewDirection)), specularPower / 2)
         * specularColorPoint[0] * att1;
    spec2 = pow(saturate(dot(normalize(reflect(-normalize(input.lightPos2), input.normal)), input.viewDirection)), specularPower / 2)
        * specularColorPoint[1] * att2;
    spec3 = pow(saturate(dot(normalize(reflect(-normalize(input.lightPos3), input.normal)), input.viewDirection)), specularPower / 2)
        * specularColorPoint[2] * att3;
    spec4 = pow(saturate(dot(normalize(reflect(-normalize(input.lightPos4), input.normal)), input.viewDirection)), specularPower / 2)
         * specularColorPoint[3] * att4;
    
    
    //spec1 = pow(saturate(dot(normalize(reflect(-normalize(input.lightPos1), input.normal)), input.viewDirection)), specularPower / 2)
    //     * specularColorPoint[0];
    //spec2 = pow(saturate(dot(normalize(reflect(-normalize(input.lightPos2), input.normal)), input.viewDirection)), specularPower / 2)
    //    * specularColorPoint[1];
    //spec3 = pow(saturate(dot(normalize(reflect(-normalize(input.lightPos3), input.normal)), input.viewDirection)), specularPower / 2)
    //    * specularColorPoint[2];
    //spec4 = pow(saturate(dot(normalize(reflect(-normalize(input.lightPos4), input.normal)), input.viewDirection)), specularPower / 2)
    //     * specularColorPoint[3];

    //textureColor = shaderTexture.Sample(SampleType, input.tex);
    //color += saturate(color1 + color2 + color3 + color4);
    //color = saturate(color * textureColor + specular);

    textureColor = shaderTexture.Sample(SampleType, input.tex);
    color += saturate(color1 + color2 + color3 + color4);
    specular += saturate(spec1 + spec2 + spec3 + spec4);
    color = saturate(color * textureColor + specular);
    
    return color;
}
