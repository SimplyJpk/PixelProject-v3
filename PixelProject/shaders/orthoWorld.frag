#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoord;

uniform usampler2D ourTexture;

struct PixelData {
    uint colour_count;
    vec4 colours[8];
};

struct MaskData {
    uint index;
    uint sub_index;
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

    // Get Pixel Type
    uint pixel_type = u_PixelMask.index & value;
    // Sub-Index (For Colours)
    uint pixel_subIndex = (u_PixelMask.sub_index & value) >> u_PixelBitOffset.index;

    vec4 available_colours[8] = u_Pixels[pixel_type].colours;
    uint colour_index = pixel_subIndex % u_Pixels[pixel_type].colour_count;   

    FragColor = available_colours[colour_index];

    if (FragColor == vec4(0.0)) {
        FragColor = vec4(0.0, 0.0, 0.0, 0.2);
    }

    // BrightColor logic commented out for now
}
