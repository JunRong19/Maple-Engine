#include <pch.h>
#include <VAO.h>

VAO::VAO() {
	glGenVertexArrays(1, &ID);
}

void VAO::LinkAttrib(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.Bind();
	// Specify the format and VBO (current active) to be used in VAO.
	// Calling this func will allow VAO to read and store the configuration.
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	// Enable vertex attribute so that OpenGL knows to use it.
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VAO::Bind() {
	// Bind VAO to the current rendering context.
	glBindVertexArray(ID);
}

void VAO::UnBind() {
	glBindVertexArray(0);
}

void VAO::Delete() {
	glDeleteVertexArrays(1, &ID);
}
