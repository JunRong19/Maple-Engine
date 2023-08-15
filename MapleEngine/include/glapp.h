#ifndef GLAPP_H
#define GLAPP_H

#include <glslshader.h>

struct GLApp {
	static void init();
	static void update();
	static void draw();
	static void cleanup();

	struct GLModel {
		GLenum primitive_type; // Which OpenGL primitive to be rendered?
		GLSLShader shdr_pgm; // Which shader program?
		GLuint vaoid; // Handle to VAO
		GLuint idx_elem_cnt;

		void setup_vao();
		void setup_shdrpgm();
		void draw();
	};

	static GLModel mdl;
};



#endif /* GLAPP_H */
