#version 330 core

in vec3 WorldPosition;
in vec3 WorldNormal;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec4 Color;
uniform vec3 AmbientColor;
uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform float AmbientReflection;
uniform float DiffuseReflection;
uniform sampler2D ColorTexture;

vec3 GetDiffuseReflection(vec3 N, vec3 L)
{
    return LightColor * DiffuseReflection * Color.rgb * max(dot(N, L), 0);
}

vec3 GetAmbientReflection()
{
    return AmbientColor * AmbientReflection * Color.rgb;
}

vec3 GetBlinnPhongReflection(vec3 N, vec3 L)
{
    return GetAmbientReflection() + GetDiffuseReflection(N, L);
}

void main()
{
    vec3 N = normalize(WorldNormal);
    vec3 L = normalize(WorldPosition - LightPosition);
	FragColor = vec4(GetBlinnPhongReflection(N, L), 1.0) * texture(ColorTexture, TexCoord);
}
