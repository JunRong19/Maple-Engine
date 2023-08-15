#include <pch.h>
#include <glapp.h>
#include <glhelper.h>

#include <MPL.h>

#include<VAO.h>
#include<VBO.h>
#include<EBO.h>

static void Initialize_Engine();
static void Initialize_ImGui();
static void Clean_Up();

int main() {
	Initialize_Engine();

	GLApp::Init();

	while (MPL::Core.Run()) {
		// Process engine's input.
		MPL::Input.Process_Input();
		// Process engine's events.
		MPL::Event.Poll();
		// Update engine.
		MPL::Core.Update();
		// Tick engine's time.
		MPL::Time.Tick();
		// Update engine's graphics.
		MPL::Graphics.Update();

		GLApp::Update();
		GLApp::Draw();

		// Swap buffers: front <-> back
		glfwSwapBuffers(MPL::Core.Window());
	}

	Clean_Up();

	// Terminate program.
	return EXIT_SUCCESS;
}

static void Initialize_Engine() {
	MPL::Core.Initialize();
	MPL::Input.Initialize();
	MPL::Time.Initialize();
	MPL::Event.Initialize();
	Initialize_ImGui();
}

static void Initialize_ImGui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(MPL::Core.Window(), true);
	ImGui_ImplOpenGL3_Init("#version 450");
}

void Clean_Up() {
	// Unsubscribe all keys.
	MPL::Input.Free();

	GLApp::Clean_Up();

	GLHelper::Clean_Up();

	// Delete window.
	glfwDestroyWindow(MPL::Core.Window());
}
