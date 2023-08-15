#include <pch.h>
#include <glapp.h>
#include <glhelper.h>

#include <MPL.h>

#include<VAO.h>
#include<VBO.h>
#include<EBO.h>

static void Initialize_Engine();
static void Cleanup();

int main() {
	Initialize_Engine();

	while (MPL::Core.Run()) {
		// Process engine's input.
		MPL::Input.ProcessInput();
		// Process engine's events.
		MPL::Event.Poll();
		// Update engine.
		MPL::Core.Update();
		// Tick engine's time.
		MPL::Time.Tick();
		// Update engine's graphics.
		MPL::Graphics.Update();

		GLApp::update();
		GLApp::draw();

		// Swap buffers: front <-> back
		glfwSwapBuffers(MPL::Core.Window());
	}

	Cleanup();

	// Terminate program.
	return EXIT_SUCCESS;
}

static void Initialize_Engine() {
	MPL::Core.Initialize();
	MPL::Input.Initialize();
	MPL::Time.Initialize();
	MPL::Event.Initialize();

	GLApp::init();
}

void Cleanup() {
	// Unsubscribe all keys.
	MPL::Input.Free();

	GLApp::cleanup();

	GLHelper::cleanup();

	// Delete window.
	glfwDestroyWindow(MPL::Core.Window());
}
