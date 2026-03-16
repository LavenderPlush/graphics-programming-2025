//Inputs
in vec2 TexCoord;

//Outputs
out vec4 FragColor;

//Uniforms
uniform sampler2D DepthTexture;
uniform sampler2D AlbedoTexture;
uniform sampler2D NormalTexture;
uniform sampler2D OthersTexture;
uniform mat4 InvViewMatrix;
uniform mat4 InvProjMatrix;

void main()
{
    vec2 normal = texture(NormalTexture, TexCoord).rg;
	FragColor = vec4(GetImplicitNormal(normal), 0.0f);
}
