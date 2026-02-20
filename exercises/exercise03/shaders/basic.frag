#version 330 core

// (todo) 03.X: Add the in variables here
in vec4 Normal;
in vec4 Position;

out vec4 FragColor;

uniform vec3 Color = vec3(1);

void main()
{
	// FragColor = dot(normalize(Normal).xyz, vec3(1)) * vec4(Color, 1);
	FragColor = normalize(Position) * 20 * dot(normalize(Normal).xyz, vec3(1)) * vec4(Color, 1);
}
