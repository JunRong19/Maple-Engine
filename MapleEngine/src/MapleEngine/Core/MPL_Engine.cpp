#include <pch.h>
#include <MPL_Engine.h>
#include <MPL_Input.h>
#include <glhelper.h>

namespace MPL {
	MPL_Engine::MPL_Engine() : run(true), window(NULL) {}

	MPL_Engine::~MPL_Engine() {
		glfwTerminate();
	}

	void MPL_Engine::Initialize() {
		// Initialize opengl backend.
		if (!GLHelper::init(1920, 1080, "MapleEngine")) {
			std::cout << "Unable to create OpenGL context" << std::endl;
			std::exit(EXIT_FAILURE);
		}

		GLHelper::print_specs();

		// Get window.
		window = GLHelper::ptr_window;
		// Configure viewport to fit the screen.
		glViewport(0, 0, MPL::WINDOW_WIDTH, MPL::WINDOW_HEIGHT);
	}

	void MPL_Engine::Update() {
		return;
	}

	void MPL_Engine::Quit() {
		run = false;
	}
}