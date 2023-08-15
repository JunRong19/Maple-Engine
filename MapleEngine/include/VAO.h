#ifndef VAO_H
#define VAO_H
#include<VBO.h>
#include<EBO.h>

class VAO {
public:
	// Reference ID to this VAO obj.
	GLuint ID;
	/*!***********************************************************************
	\brief
		Construct a vertex array object.
	*************************************************************************/
	VAO();

	/*!***********************************************************************
	\brief
		Link VBO to this VAO to read and store its configuration.
	*************************************************************************/
	void Link_Attrib(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);

	/*!***********************************************************************
	\brief
		Set this VAO as active.
	*************************************************************************/
	void Bind();

	/*!***********************************************************************
	\brief
		Deactive this VAO.
	*************************************************************************/
	void UnBind();

	/*!***********************************************************************
	\brief
		Delete this VAO.
	*************************************************************************/
	void Delete();
};

#endif // !VAO_H
