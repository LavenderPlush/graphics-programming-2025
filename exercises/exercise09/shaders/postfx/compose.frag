//Inputs
in vec2 TexCoord;
uniform float Exposure;
uniform float Contrast;
uniform float Hue;
uniform float Saturation;
uniform vec3 ColorFilter;


//Outputs
out vec4 FragColor;

//Uniforms
uniform sampler2D SourceTexture;
uniform sampler2D BloomTexture;

void main()
{
    vec4 color = texture(SourceTexture, TexCoord) + texture(BloomTexture, TexCoord);
	color = 1.0f - exp(-color*Exposure);

    // Contrast
    color.rgb = (color.rgb - vec3(0.5)) * Contrast + vec3(0.5);

    // Hue shift
    vec3 hsv = RGBToHSV(color.rgb);
    hsv.x = fract(hsv.x + Hue);
    color = vec4(HSVToRGB(hsv), color.a);

    // Saturation
    float luminance = GetLuminance(color.rgb);
    vec3 saturated = (color.rgb - vec3(luminance)) * Saturation + vec3(luminance);
    color.rgb = clamp(saturated, vec3(0.0f), vec3(1.0f));

    // Color filter
    color.rgb *= ColorFilter;

	FragColor = color;
}
