#include <pch.h>
#include <MPL.h>
#include <glhelper.h>
#include <iomanip>

namespace MPL {
	MPL_Engine::MPL_Engine() : run(true), window(NULL) {}

	MPL_Engine::~MPL_Engine() {
		glfwTerminate();
	}

	void MPL_Engine::Initialize() {
		// Initialize opengl backend.
		if (!GLHelper::Init(1920, 1080, "MapleEngine")) {
			std::cout << "Unable to create OpenGL context" << std::endl;
			std::exit(EXIT_FAILURE);
		}

		GLHelper::Print_Specs();

		// Get window.
		window = GLHelper::ptr_window;
		// Configure viewport to fit the screen.
		glViewport(0, 0, MPL::WINDOW_WIDTH, MPL::WINDOW_HEIGHT);
	}

	void MPL_Engine::Update() {
		WindowTitle();
	}

	void MPL_Engine::Quit() {
		run = false;
	}

	void MPL_Engine::WindowTitle() {
		// Update title of window.
		std::stringstream title;
		title << "MapleEngine | FPS: " << std::fixed << std::setprecision(2) << MPL::Time.Fps();
		glfwSetWindowTitle(GLHelper::ptr_window, title.str().c_str());
	}
}