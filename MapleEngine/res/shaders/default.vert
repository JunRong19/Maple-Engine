#version 450 core

// 8 is vertex attribute index that is associated with per-vertex
// position coordinates in VBO; this association was performed by
// this call: glEnableVertexArrayAttrib(vaoid, 8);
layout (location=8) in vec2 vertexPosition;

// 9 is vertex attribute index that is associated with per-vertex
// position coordinates in VBO; this association was performed by
// this call: glEnableVertexArrayAttrib(vaoid, 9);
layout (location=9) in vec3 vertexColor;

layout (location=0) out vec3 vColor;

void main() {
	gl_Position = vec4(vertexPosition, 0.0, 1.0);
	vColor = vertexColor;
}
