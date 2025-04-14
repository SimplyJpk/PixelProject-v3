#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D ourTexture;
uniform sampler2D noiseTextureIndex;

struct PixelData
{
	uint colour_count;
	vec4[4] colours;
};

struct MaskData
{
	uint index;
	uint lifetime;
	uint behaviour;
	uint light;
};

uniform PixelData[20] u_Pixels;
uniform MaskData u_PixelMask;
uniform MaskData u_PixelBitOffset;

//const float LIGHT_INVERVAL = 1.0 / 7.0;

void main()
{
	// FragColor = texture(ourTexture, TexCoord);
	uint value = floatBitsToUint(texture2D(ourTexture, TexCoord).r);
	// int index = value & 0000000000001111;

	// No shift since start of Bits
	int pixelType = int((u_PixelMask.index & value));
	
	vec4 colours[4] = u_Pixels[pixelType].colours;

	//	uint light = (u_PixelMask.light & value) >> u_PixelBitOffset.light;
	//	float addedLight = light * LIGHT_INVERVAL;

	float noiseValue = texture2D(noiseTextureIndex, TexCoord).r;
	uint noiseIndex = uint(noiseValue * 3.0);
	
	// Set FragColor to the index of colour
	uint index = noiseIndex % uint(u_Pixels[pixelType].colour_count);
	FragColor[0] = colours[index][0];
	FragColor[1] = colours[index][1];
	FragColor[2] = colours[index][2];
	FragColor[3] = colours[index][3];
	
	// TODO : (James) Remove ~ This just makes it easier to see chunks
	if (FragColor == vec4(0.0, 0.0, 0.0, 0.0)) {
		FragColor = vec4(0.0, 0.0, 0.0, 0.2);
	}
	
//	if (addedLight != 0) {
//		BrightColor = vec4(vec3(FragColor), addedLight);
//	}
}