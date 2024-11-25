#version 330 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

// out vec4 gl_Position;
out vec3 vColor;

void main()
{
	gl_Position = vec4(position, 0, 1);
	// vColor = vec3(1.f, 0.5f, 0.5);
	vColor = color;
}