#version 330 core

layout (location = 0) in vec2 ParticlePosition;

// (todo) 02.X: Add more vertex attributes
layout (location = 1) in float ParticleSize;
layout (location = 2) in float Birth;
layout (location = 3) in float Duration;
layout (location = 4) in vec4 InColor;
layout (location = 5) in vec2 Velocity;

// (todo) 02.5: Add Color output variable here
out vec4 Color;

// (todo) 02.X: Add uniforms
uniform float CurrentTime;
uniform vec2 Gravity;

void main()
{
    float age = CurrentTime - Birth;
    Color = InColor;

    float t = age * 1.5;
    vec2 xt = ParticlePosition + Velocity * t + 0.5 * Gravity * t * t;

	gl_Position = vec4(xt, 0.0, 1.0);
	gl_PointSize = (1.0 - (age / Duration)) * ParticleSize;
}
