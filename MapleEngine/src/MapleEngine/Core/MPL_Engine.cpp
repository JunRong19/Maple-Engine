#include <pch.h>
#include <MPL.h>
#include <glhelper.h>
#include <iomanip>

namespace MPL {
	MPL_Engine::MPL_Engine() {}

	MPL_Engine::~MPL_Engine() {
		// Delete window.
		glfwDestroyWindow(MPL::Core.Window());
		// Close program.
		glfwTerminate();
	}

	void MPL_Engine::Initialize() {
		if (!glfwInit()) {
			std::cout << "GLFW init has failed - abort program!!!" << std::endl;
			std::exit(EXIT_FAILURE);
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

		MPL_Configs configs;
		// Load window dimensions from configs.
		if (configs.Get_Data().count("WINDOW_HEIGHT") && configs.Get_Data().count("WINDOW_WIDTH")) {
			// Load window settings from configs.
			WINDOW_HEIGHT = std::stoul(configs.Get_Data().at("WINDOW_HEIGHT"));
			WINDOW_WIDTH = std::stoul(configs.Get_Data().at("WINDOW_WIDTH"));
		}
		else {
			WINDOW_HEIGHT = 1080;
			WINDOW_WIDTH = 1920;
			std::cerr << "Unable to load window setting from configs. Using default width and height " << std::endl;
		}

		window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, window_title.c_str(), NULL, NULL);
		if (!window) {
			std::cerr << "GLFW unable to create OpenGL context - abort program\n";
			glfwTerminate();
			std::exit(EXIT_FAILURE);
		}
		glfwMakeContextCurrent(window);

		// Configure viewport to fit the screen.
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		glfwSetFramebufferSizeCallback(window, GLHelper::Fbsize_Cb);
		glfwSetKeyCallback(window, GLHelper::Key_Cb);
		glfwSetMouseButtonCallback(window, GLHelper::Mousebutton_Cb);
		glfwSetCursorPosCallback(window, GLHelper::Mousepos_Cb);
		glfwSetScrollCallback(window, GLHelper::Mousescroll_Cb);

		// this is the default setting ...
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		// Initialize entry points to OpenGL functions and extensions
		GLenum err = glewInit();
		if (GLEW_OK != err) {
			std::cerr << "Unable to initialize GLEW - error: "
				<< glewGetErrorString(err) << " abort program" << std::endl;
			std::exit(EXIT_FAILURE);
		}
		if (GLEW_VERSION_4_5) {
			std::cout << "Using glew version: " << glewGetString(GLEW_VERSION) << std::endl;
			std::cout << "Driver supports OpenGL 4.5\n" << std::endl;
		}
		else {
			std::cerr << "Driver doesn't support OpenGL 4.5 - abort program" << std::endl;
			std::exit(EXIT_FAILURE);
		}
		// Start engine.
		run = true;
	}

	void MPL_Engine::Update() {
		Set_Window_Title();
	}

	void MPL_Engine::Quit() {
		run = false;
	}

	void MPL_Engine::Set_Window_Title() {
		// Update title of window.
		std::stringstream title;
		title << "MapleEngine | FPS: " << std::fixed << std::setprecision(2) << MPL::Time.Fps();
		glfwSetWindowTitle(window, title.str().c_str());
	}

}