#version 330 core

layout (location = 0) in vec2 ParticlePosition;
layout (location = 1) in float ParticleSize;
layout (location = 2) in float Birth;
layout (location = 3) in float Duration;
// (todo) 02.X: Add more vertex attributes


// (todo) 02.5: Add Color output variable here


// (todo) 02.X: Add uniforms
uniform float CurrentTime;

void main()
{
    float age = CurrentTime - Birth;

	gl_Position = vec4(ParticlePosition, 0.0, 1.0);
	// gl_PointSize = ParticleSize;
	gl_PointSize = (1.0 - (age / Duration)) * ParticleSize;
}
