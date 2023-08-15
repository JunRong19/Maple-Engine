#version 450 core

layout (location=0) in vec3 vInterpColor;

layout (location=0) out vec4 fragColor;

void main () {
	fragColor = vec4(vInterpColor, 1.0);
}