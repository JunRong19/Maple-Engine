#include <pch.h>
#include <glhelper.h>
#include <MPL_Engine.h>

void GLHelper::Key_Cb(GLFWwindow* pwin, int key, int scancode, int action, int mod) {
	if (GLFW_PRESS == action) {
#ifdef _DEBUG
		std::cout << "Key pressed" << std::endl;
#endif
		if (GLFW_KEY_ESCAPE == key) {
			glfwSetWindowShouldClose(pwin, GLFW_TRUE);
		}
	}
	else if (GLFW_REPEAT == action) {
#ifdef _DEBUG
		std::cout << "Key repeatedly pressed" << std::endl;
#endif
	}
	else if (GLFW_RELEASE == action) {
#ifdef _DEBUG
		std::cout << "Key released" << std::endl;
#endif
	}
}

void GLHelper::Mousebutton_Cb(GLFWwindow* pwin, int button, int action, int mod) {
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
#ifdef _DEBUG
		std::cout << "Left mouse button ";
#endif
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
#ifdef _DEBUG
		std::cout << "Right mouse button ";
#endif
		break;
	}
	switch (action) {
	case GLFW_PRESS:
#ifdef _DEBUG
		std::cout << "pressed!!!" << std::endl;
#endif
		break;
	case GLFW_RELEASE:
#ifdef _DEBUG
		std::cout << "released!!!" << std::endl;
#endif
		break;
	}
}

void GLHelper::Mousepos_Cb(GLFWwindow* pwin, double xpos, double ypos) {
#ifdef _DEBUG
	std::cout << "Mouse cursor position: (" << xpos << ", " << ypos << ")" << std::endl;
#endif
}

void GLHelper::Print_Specs() {
	GLubyte const* str = glGetString(GL_VENDOR);
	std::cout << "Vendor: " << str << std::endl;

	str = glGetString(GL_RENDERER);
	std::cout << "GL Renderer: " << str << std::endl;

	str = glGetString(GL_VERSION);
	std::cout << "GL Version: " << str << std::endl;

	str = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cout << "GL Shader Version: " << str << std::endl;

	GLint var;

	glGetIntegerv(GL_MAJOR_VERSION, &var);
	std::cout << "GL Major Version: " << var << std::endl;

	glGetIntegerv(GL_MINOR_VERSION, &var);
	std::cout << "GL Minor Version: " << var << std::endl;

	glGetIntegerv(GL_DOUBLEBUFFER, &var);
	std::cout << "Current OpenGL Context is ";
	if (var) { std::cout << "double buffered" << std::endl; }
	else { std::cout << "not double buffered" << std::endl; }

	glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &var);
	std::cout << "Maximum Vertex Count: " << var << std::endl;

	glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &var);
	std::cout << "Maximum Indices Count: " << var << std::endl;

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &var);
	std::cout << "Maximum texture size: " << var << std::endl;

	glGetIntegerv(GL_MAX_WIDTH, &var);
	std::cout << "Maximum Viewport Dimensions: " << var << " x ";
	glGetIntegerv(GL_MAX_HEIGHT, &var);
	std::cout << var << std::endl;
}

void GLHelper::Mousescroll_Cb(GLFWwindow* pwin, double xoffset, double yoffset) {
#ifdef _DEBUG
	std::cout << "Mouse scroll wheel offset: ("
		<< xoffset << ", " << yoffset << ")" << std::endl;
#endif
}

void GLHelper::Error_Cb(int error, char const* description) {
#ifdef _DEBUG
	std::cerr << "GLFW error: " << description << std::endl;
#endif
}

void GLHelper::Fbsize_Cb(GLFWwindow* ptr_win, int width, int height) {
#ifdef _DEBUG
	std::cout << "fbsize_cb getting called!!!" << std::endl;
#endif
	// use the entire framebuffer as drawing region
	glViewport(0, 0, width, height);
	// later, if working in 3D, we'll have to set the projection matrix here ...
}
