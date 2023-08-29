#ifndef VBO_H
#define VBO_H

class VBO {
public:
	// Reference ID to this VBO obj.
	GLuint ID;
	/*!***********************************************************************
	\brief
		Construct a vertex buffer object.
	*************************************************************************/
	VBO(GLsizeiptr size, GLfloat* vertices = nullptr);


	/*!***********************************************************************
	\brief
		Delete this VBO.
	*************************************************************************/
	void Delete();
};

#endif // !VBO_H
