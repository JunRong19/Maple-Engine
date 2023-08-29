#include <pch.h>
#include <VBO.h>

//TODO: 2 VBO 

VBO::VBO(GLsizeiptr size, GLfloat* data) {
	glCreateBuffers(1, &ID);
	glNamedBufferStorage(ID, size, data, GL_DYNAMIC_STORAGE_BIT);
}



void VBO::Delete() {
	glDeleteBuffers(1, &ID);
}
