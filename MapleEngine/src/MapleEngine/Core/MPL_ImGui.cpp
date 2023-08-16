#include <pch.h>
#include <MPL_ImGui.h>
#include <MPL_Engine.h>

namespace MPL {
	MPL_ImGui::MPL_ImGui() {}

	MPL_ImGui::~MPL_ImGui() {
		// Shutdown ImGui.
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void MPL_ImGui::Initialize() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiDockNodeFlags_NoDockingInCentralNode;

		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(MPL::Core.Window(), true);
		ImGui_ImplOpenGL3_Init("#version 450");

		Build_Default_Layout();
	}

	void MPL_ImGui::Draw() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		Set_Default_Layout();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void MPL_ImGui::Set_Default_Layout() {
		ImGui::Begin("Hierarchy");
		ImGui::End();

		ImGui::Begin("Inspector");
		ImGui::End();

		ImGui::Begin("Project");
		ImGui::End();

		ImGui::Begin("Scene");
		ImGui::End();
	}

	void  MPL_ImGui::Build_Default_Layout() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImVec2 workPos = ImGui::GetMainViewport()->WorkPos;
		ImVec2 workSize = ImGui::GetMainViewport()->WorkSize;

		ImVec2 workCenter = ImGui::GetMainViewport()->GetWorkCenter();

		ImGuiID id = ImGui::GetID("MainWindowGroup");

		ImGui::DockBuilderRemoveNode(id);
		ImGui::DockBuilderAddNode(id);

		ImVec2 size{ workSize.x, workSize.y };
		ImVec2 nodePos{ workCenter.x - size.x * 0.5f, workCenter.y - size.y * 0.5f };

		// Set the size and position:
		ImGui::DockBuilderSetNodeSize(id, size);
		ImGui::DockBuilderSetNodePos(id, nodePos);

		ImGuiID dock1 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.8f, nullptr, &id);

		ImGuiID dock2 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Right, 0.2f, nullptr, &id);

		ImGuiID dock3 = ImGui::DockBuilderSplitNode(dock1, ImGuiDir_Down, 0.3f, nullptr, &dock1);

		ImGuiID dock4 = ImGui::DockBuilderSplitNode(dock1, ImGuiDir_Right, 0.8f, nullptr, &dock1);

		ImGui::DockBuilderDockWindow("Hierarchy", dock1);
		ImGui::DockBuilderDockWindow("Inspector", dock2);
		ImGui::DockBuilderDockWindow("Project", dock3);
		ImGui::DockBuilderDockWindow("Scene", dock4);

		ImGui::DockBuilderFinish(id);
		ImGui::EndFrame();
	}
}

