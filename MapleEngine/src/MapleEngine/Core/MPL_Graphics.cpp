#include <pch.h>
#include <glhelper.h>
#include <glapp.h>
#include <MPL_Engine.h>
#include <MPL_Graphics.h>

namespace MPL {
	MPL_Graphics::MPL_Graphics() {}

	MPL_Graphics::~MPL_Graphics() {
		// Shutdown ImGui.
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void MPL_Graphics::Initialize() {
		Initialize_ImGui();
	}

	void MPL_Graphics::Draw() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void  MPL_Graphics::Initialize_ImGui() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(MPL::Core.Window(), true);
		ImGui_ImplOpenGL3_Init("#version 450");
	}
}
