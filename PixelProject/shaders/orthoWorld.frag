#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoord;

uniform usampler2D ourTexture;
uniform usampler2D noiseTextureIndex;

struct PixelData {
    uint colour_count;
    vec4 colours[4];
};

struct MaskData {
    uint index;
    uint lifetime;
    uint behaviour;
    uint light;
};

uniform PixelData u_Pixels[20];
uniform MaskData u_PixelMask;
uniform MaskData u_PixelBitOffset;

void main()
{
    uvec2 pixelValue = texture(ourTexture, TexCoord).xy;
    uint largeValue = pixelValue.x;
    uint smallValue = pixelValue.y;

    if (smallValue == 0u && largeValue == 0u) {
        FragColor = vec4(0.0);
        return;
    }

    // Use one of the uint components as your value
    uint value = largeValue; // or smallValue depending on your logic

    // Bitwise AND with mask index
    uint pixelType = u_PixelMask.index & value;

    vec4 colours[4] = u_Pixels[pixelType].colours;

    uint noiseIndex = texture(noiseTextureIndex, TexCoord).r;
    
    uint index = noiseIndex % u_Pixels[pixelType].colour_count;

    FragColor = colours[index];

    if (FragColor == vec4(0.0)) {
        FragColor = vec4(0.0, 0.0, 0.0, 0.2);
    }

    // BrightColor logic commented out for now
}
