//Inputs
in vec2 TexCoord;

//Outputs
out vec4 FragColor;

//Uniforms
uniform sampler2D SourceTexture;
uniform vec2 Range;
uniform float Intensity;

void main()
{
    vec4 color = texture(SourceTexture, TexCoord);
    float luminance = GetLuminance(color.rgb);
    if (luminance < Range.x) {
        luminance = 0.0f;
    } else if (luminance > Range.y) {
        luminance = 1.0f;
    }
	FragColor = vec4(color.rgb * luminance * Intensity, color.a);
}
