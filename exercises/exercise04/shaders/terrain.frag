#version 330 core

in vec3 WorldPosition;
in vec3 WorldNormal;
in vec2 TexCoord;
in float Height;

out vec4 FragColor;

uniform vec4 Color;
uniform sampler2D DirtTexture;
uniform sampler2D GrassTexture;
uniform sampler2D RockTexture;
uniform sampler2D SnowTexture;

uniform vec2 DirtGrassRange;
uniform vec2 GrassRockRange;
uniform vec2 RockSnowRange;

uniform vec2 ColorTextureScale;


float interp(vec2 range, float height) {
    if (height <= range.x) {
        return 0;
    } else if (height >= range.y) {
        return 1;
    } else {
        return (height - range.x) / (range.y - range.x);
    }
}

void main()
{
    vec4 DirtSample = texture(DirtTexture, TexCoord * ColorTextureScale);
    vec4 GrassSample = texture(GrassTexture, TexCoord * ColorTextureScale);
    vec4 RockSample = texture(RockTexture, TexCoord * ColorTextureScale);
    vec4 SnowSample = texture(SnowTexture, TexCoord * ColorTextureScale);

    vec4 blendedColor = DirtSample;
    blendedColor = mix(blendedColor, GrassSample, interp(DirtGrassRange, Height));
    blendedColor = mix(blendedColor, RockSample, interp(GrassRockRange, Height));
    blendedColor = mix(blendedColor, SnowSample, interp(RockSnowRange, Height));

	FragColor = blendedColor * Color;
}
