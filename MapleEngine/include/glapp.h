#ifndef GLAPP_H
#define GLAPP_H

#include <glslshader.h>

struct GLApp {
	static void Init();
	static void Update();
	static void Draw();
	static void Clean_Up();

	struct GLModel {
		GLenum primitive_type; // Which OpenGL primitive to be rendered?
		GLSLShader shdr_pgm; // Which shader program?
		GLuint vaoid; // Handle to VAO
		GLuint idx_elem_cnt;

		void Setup_Vao();
		void Setup_Shdrpgm();
		void Draw();
	};

	static GLModel mdl;
};



#endif /* GLAPP_H */
