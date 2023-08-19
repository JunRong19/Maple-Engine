#include <pch.h>
#include <MPL_ImGui.h>
#include <MPL_Engine.h>
#include <filesystem>

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
		//Build_Default_Layout();
		Load_Layouts();
	}

	void MPL_ImGui::Draw() {
		Render_Engine_Layout();
	}

	void MPL_ImGui::Start_Frame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void MPL_ImGui::Render_ImGui() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void MPL_ImGui::Load_Layouts() {
		std::string path = "res/dock_layouts";
		for (const auto& entry : std::filesystem::directory_iterator(path)) {

			std::ifstream file{ entry.path()  ,std::ios::in };
			if (!file) {
				std::cout << "UNABLE TO LOAD DOCK LAYOUT AT: " << entry.path() << std::endl;
			}

			std::string line;
			std::string root_dock;
			std::unordered_map<std::string, ImGuiID> window_name_to_dock_id;

			getline(file, line);
			std::istringstream ss_line{ line };
			ss_line >> root_dock;

			Start_Frame();

			// Get screen dimensions.
			ImVec2 workSize = ImGui::GetMainViewport()->WorkSize;
			ImVec2 workCenter = ImGui::GetMainViewport()->GetWorkCenter();
			// Get screen size.
			ImVec2 size{ workSize.x, workSize.y };
			// Get middle of screen.
			ImVec2 nodePos{ workCenter.x - size.x * 0.5f, workCenter.y - size.y * 0.5f };

			// Set root node.
			ImGuiID rootId = ImGui::GetID(root_dock.c_str());
			window_name_to_dock_id[root_dock] = rootId;
			// Clear previous dock with this root id.
			ImGui::DockBuilderRemoveNode(rootId);
			// Create a new root node.
			ImGui::DockBuilderAddNode(rootId);

			// Set the size and position:
			ImGui::DockBuilderSetNodeSize(rootId, size);
			ImGui::DockBuilderSetNodePos(rootId, nodePos);

			// Set windows.
			while (getline(file, line)) {
				std::string node_name;
				std::string parent_node_str;
				int direction_int;	// 0: UP | 1: RIGHT | 2: DOWN | 3: LEFT
				ImGuiDir direction = ImGuiDir_None;
				float size_ratio{};

				std::istringstream ss_line{ line };
				ss_line >> node_name >> parent_node_str >> direction_int >> size_ratio;

				switch (direction_int)
				{
				case 0:
					direction = ImGuiDir_Up;
					break;
				case 1:
					direction = ImGuiDir_Right;
					break;
				case 2:
					direction = ImGuiDir_Down;
					break;
				case 3:
					direction = ImGuiDir_Left;
					break;
				default:
					break;
				}

				ImGuiID parent_node = window_name_to_dock_id[parent_node_str];
				ImGuiID dock = ImGui::DockBuilderSplitNode(parent_node, direction, size_ratio, nullptr, &parent_node);
				window_name_to_dock_id[parent_node_str] = parent_node;
				window_name_to_dock_id[node_name] = dock;
				ImGui::DockBuilderDockWindow(node_name.c_str(), dock);
			}
			ImGui::DockBuilderFinish(rootId);

			ImGui::EndFrame();

			layouts[root_dock] = window_name_to_dock_id;
		}
	}

	void MPL_ImGui::Render_Engine_Layout() {
		// If "main" layout is deleted or missing, build a new one.
		if (!layouts.count("main")) {
			Build_Default_Layout();
			std::cerr << "LAYOUT ERROR: \"main\" layout missing. Building new a new one." << std::endl;
		}

		Start_Frame();
		for (auto& layout : layouts["main"]) {
			ImGui::Begin(layout.first.c_str());
			ImGui::End();
		}

		Render_ImGui();
	}

	void MPL_ImGui::Build_Default_Layout() {

		std::unordered_map<std::string, ImGuiID> window_name_to_dock_id;

		Start_Frame();

		ImVec2 workSize = ImGui::GetMainViewport()->WorkSize;

		ImVec2 workCenter = ImGui::GetMainViewport()->GetWorkCenter();

		ImGuiID id = ImGui::GetID("main");

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

		window_name_to_dock_id["main"] = id;
		window_name_to_dock_id["Hierarchy"] = dock1;
		window_name_to_dock_id["Inspector"] = dock2;
		window_name_to_dock_id["Project"] = dock3;
		window_name_to_dock_id["Scene"] = dock4;
		layouts["main"] = window_name_to_dock_id;

		ImGui::DockBuilderFinish(id);

		ImGui::EndFrame();


	}
}

