//Inputs
in vec2 TexCoord;
uniform float Exposure;

//Outputs
out vec4 FragColor;

//Uniforms
uniform sampler2D SourceTexture;

void main()
{
	FragColor = 1.0f - exp(-texture(SourceTexture, TexCoord)*Exposure);
}
