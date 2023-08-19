#include <pch.h>
#include <glapp.h>
#include <glhelper.h>

#include <MPL.h>

#include<VAO.h>
#include<VBO.h>
#include<EBO.h>

static void Initialize_Engine();
static void Clean_Up();

int main() {
	Initialize_Engine();

	GLApp::Init();

	while (MPL::Core.Run()) {
		// Process engine's input.
		MPL::Input.Process_Input();
		// Process engine's events.
		MPL::Event.Poll();
		// Tick engine's time.
		MPL::Time.Tick();
		// Update engine.
		MPL::Core.Update();
		// Update app.
		GLApp::Update();
		MPL::Graphics.Draw();
		// Draw UI.
		MPL::ImGui.Draw();
		// Swap buffers: front <-> back
		glfwSwapBuffers(MPL::Core.Window());
	}

	Clean_Up();

	// Terminate program.
	return EXIT_SUCCESS;
}

static void Initialize_Engine() {
	// Load saved config for engine.
	MPL::Config.Load_Configs();

	// Load main engine functionalities.
	MPL::Core.Initialize();
	MPL::Input.Initialize();
	MPL::Time.Initialize();
	MPL::Event.Initialize();
	MPL::Graphics.Initialize();
	MPL::ImGui.Initialize();
}

void Clean_Up() {
	GLApp::Clean_Up();
}
