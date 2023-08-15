#include <pch.h>
#include <MPL_Events.h>
#include <MPL_Engine.h>

namespace MPL {

	void WindowCloseCallback(GLFWwindow* window);

	MPL_Events::MPL_Events() {}
	MPL_Events::~MPL_Events() {}

	void MPL_Events::Initialize() {
		glfwSetWindowCloseCallback(Core.Window(), WindowCloseCallback);
	}

	void MPL_Events::Poll() {
		glfwPollEvents();
	}

	void WindowCloseCallback(GLFWwindow* window) {
		MPL::Core.Quit();
	}
}
