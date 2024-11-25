#version 330 core

in  vec3 vColor;
out vec4 outCol;

void main(){
	outCol = vec4(vColor, 1.f);
	// outCol = vec4(1, 1, 1, 1.f);
}