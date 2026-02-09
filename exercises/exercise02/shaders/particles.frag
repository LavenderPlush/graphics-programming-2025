#version 330 core

out vec4 FragColor;

// (todo) 02.5: Add Color input variable here
in vec4 Color;

void main()
{
	// (todo) 02.3: Compute alpha using the built-in variable gl_PointCoord
	vec2 position = gl_PointCoord * 2.0 - 1.0;
    float opacity = 1.0 - length(position);

	FragColor = vec4(Color.rgb, opacity * Color.a);
}
