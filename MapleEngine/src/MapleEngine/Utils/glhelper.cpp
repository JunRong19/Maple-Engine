#include <pch.h>
#include <glhelper.h>
GLint GLHelper::width;
GLint GLHelper::height;
GLdouble GLHelper::fps;
GLdouble GLHelper::delta_time;
std::string GLHelper::title;
GLFWwindow* GLHelper::ptr_window;
GLboolean GLHelper::keystate_T = GL_FALSE;
GLboolean GLHelper::keystate_M = GL_FALSE;
GLboolean GLHelper::keystate_A = GL_FALSE;

bool GLHelper::init(GLint w, GLint h, std::string t) {
	GLHelper::width = w;
	GLHelper::height = h;
	GLHelper::title = t;

	if (!glfwInit()) {
		std::cout << "GLFW init has failed - abort program!!!" << std::endl;
		return false;
	}

	// In case a GLFW function fails, an error is reported to callback function
	glfwSetErrorCallback(GLHelper::error_cb);

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

	glfwSetFramebufferSizeCallback(GLHelper::ptr_window, GLHelper::fbsize_cb);
	glfwSetKeyCallback(GLHelper::ptr_window, GLHelper::key_cb);
	glfwSetMouseButtonCallback(GLHelper::ptr_window, GLHelper::mousebutton_cb);
	glfwSetCursorPosCallback(GLHelper::ptr_window, GLHelper::mousepos_cb);
	glfwSetScrollCallback(GLHelper::ptr_window, GLHelper::mousescroll_cb);

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


void GLHelper::cleanup() {
	glfwTerminate();
}

void GLHelper::key_cb(GLFWwindow* pwin, int key, int scancode, int action, int mod) {
	if (GLFW_PRESS == action) {
#ifdef _DEBUG
		std::cout << "Key pressed" << std::endl;
#endif
		if (GLFW_KEY_ESCAPE == key) {
			glfwSetWindowShouldClose(pwin, GLFW_TRUE);
		}
		keystate_T = (GLFW_KEY_T == key) ? GL_TRUE : GL_FALSE;
		keystate_M = (GLFW_KEY_M == key) ? GL_TRUE : GL_FALSE;
		keystate_A = (GLFW_KEY_A == key) ? GL_TRUE : GL_FALSE;
	}
	else if (GLFW_REPEAT == action) {
#ifdef _DEBUG
		std::cout << "Key repeatedly pressed" << std::endl;
#endif
		if (GLFW_KEY_T == key)keystate_T = GL_TRUE;
		if (GLFW_KEY_M == key)keystate_M = GL_TRUE;
		if (GLFW_KEY_A == key)keystate_A = GL_TRUE;
	}
	else if (GLFW_RELEASE == action) {
#ifdef _DEBUG
		std::cout << "Key released" << std::endl;
#endif
		if (GLFW_KEY_T == key) keystate_T = GL_FALSE;
		if (GLFW_KEY_M == key) keystate_M = GL_FALSE;
		if (GLFW_KEY_A == key) keystate_A = GL_FALSE;
	}
}

void GLHelper::mousebutton_cb(GLFWwindow* pwin, int button, int action, int mod) {
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

void GLHelper::mousepos_cb(GLFWwindow* pwin, double xpos, double ypos) {
#ifdef _DEBUG
	std::cout << "Mouse cursor position: (" << xpos << ", " << ypos << ")" << std::endl;
#endif
}

void GLHelper::print_specs() {
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

void GLHelper::mousescroll_cb(GLFWwindow* pwin, double xoffset, double yoffset) {
#ifdef _DEBUG
	std::cout << "Mouse scroll wheel offset: ("
		<< xoffset << ", " << yoffset << ")" << std::endl;
#endif
}

void GLHelper::error_cb(int error, char const* description) {
#ifdef _DEBUG
	std::cerr << "GLFW error: " << description << std::endl;
#endif
}

void GLHelper::fbsize_cb(GLFWwindow* ptr_win, int width, int height) {
#ifdef _DEBUG
	std::cout << "fbsize_cb getting called!!!" << std::endl;
#endif
	// use the entire framebuffer as drawing region
	glViewport(0, 0, width, height);
	// later, if working in 3D, we'll have to set the projection matrix here ...
}

void GLHelper::update_time(double fps_calc_interval) {
	// get elapsed time (in seconds) between previous and current frames
	static double prev_time = glfwGetTime();
	double curr_time = glfwGetTime();
	delta_time = curr_time - prev_time;
	prev_time = curr_time;

	// fps calculations
	static double count = 0.0; // number of game loop iterations
	static double start_time = glfwGetTime();
	// get elapsed time since very beginning (in seconds) ...
	double elapsed_time = curr_time - start_time;

	++count;

	// update fps at least every 10 seconds ...
	fps_calc_interval = (fps_calc_interval < 0.0) ? 0.0 : fps_calc_interval;
	fps_calc_interval = (fps_calc_interval > 10.0) ? 10.0 : fps_calc_interval;
	if (elapsed_time > fps_calc_interval) {
		GLHelper::fps = count / elapsed_time;
		start_time = curr_time;
		count = 0.0;
	}
}
