#version 330 core

in vec3 WorldPosition;
in vec3 WorldNormal;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec4 Color;
uniform vec3 AmbientColor;
uniform float AmbientReflection;
uniform sampler2D ColorTexture;

vec3 GetAmbientReflection()
{
    return AmbientColor * AmbientReflection * Color.rgb;
}

vec3 GetBlinnPhongReflection()
{
    return GetAmbientReflection();
}

void main()
{
	FragColor = vec4(GetBlinnPhongReflection(), 1.0) * texture(ColorTexture, TexCoord);
}
