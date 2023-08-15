#include <pch.h>
#include <glhelper.h>
GLint GLHelper::width;
GLint GLHelper::height;
std::string GLHelper::title;
GLFWwindow* GLHelper::ptr_window;

bool GLHelper::Init(GLint w, GLint h, std::string t) {
	GLHelper::width = w;
	GLHelper::height = h;
	GLHelper::title = t;

	if (!glfwInit()) {
		std::cout << "GLFW init has failed - abort program!!!" << std::endl;
		return false;
	}

	// In case a GLFW function fails, an error is reported to callback function
	glfwSetErrorCallback(GLHelper::Error_Cb);

	// Before asking GLFW to create an OpenGL context, we specify the minimum constraints
	// in that context:
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // window dimensions are static

	GLHelper::ptr_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!GLHelper::ptr_window) {
		std::cerr << "GLFW unable to create OpenGL context - abort program\n";
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(GLHelper::ptr_window);

	glfwSetFramebufferSizeCallback(GLHelper::ptr_window, GLHelper::Fbsize_Cb);
	glfwSetKeyCallback(GLHelper::ptr_window, GLHelper::Key_Cb);
	glfwSetMouseButtonCallback(GLHelper::ptr_window, GLHelper::Mousebutton_Cb);
	glfwSetCursorPosCallback(GLHelper::ptr_window, GLHelper::Mousepos_Cb);
	glfwSetScrollCallback(GLHelper::ptr_window, GLHelper::Mousescroll_Cb);

	// this is the default setting ...
	glfwSetInputMode(GLHelper::ptr_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Initialize entry points to OpenGL functions and extensions
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Unable to initialize GLEW - error: "
			<< glewGetErrorString(err) << " abort program" << std::endl;
		return false;
	}
	if (GLEW_VERSION_4_5) {
		std::cout << "Using glew version: " << glewGetString(GLEW_VERSION) << std::endl;
		std::cout << "Driver supports OpenGL 4.5\n" << std::endl;
	}
	else {
		std::cerr << "Driver doesn't support OpenGL 4.5 - abort program" << std::endl;
		return false;
	}

	return true;
}


void GLHelper::Clean_Up() {
	// Delete window.
	glfwDestroyWindow(GLHelper::ptr_window);
	glfwTerminate();
}

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
