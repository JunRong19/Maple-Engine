#include <pch.h>
#include <MPL_ImGui.h>
#include <MPL_Engine.h>
#include <MPL_Configs.h>
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

		// Load all layouts available in res folder.
		if (Load_Layouts() == false) {
			// Create a default layout if no layout found.
			Build_Default_Layout(true);
			std::cerr << "LAYOUT ERROR: No layout found in res folder. Using default layout." << std::endl;
		}

		MPL_Configs configs;
		// Check if saved layout is loaded.
		if (layouts.count(configs.Get_Data().at("LAYOUT"))) {
			// Set current layout as saved layout.
			curr_layout = configs.Get_Data().at("LAYOUT");
		}
		else {
			// Saved layout not found. Use default layout.
			curr_layout = DEFAULT_LAYOUT_NAME;
			configs.Set_Data(std::make_pair("LAYOUT", DEFAULT_LAYOUT_NAME));
			std::cerr << "LAYOUT ERROR: Unable to find saved layout. Using default." << std::endl;
		}
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

	bool const MPL_ImGui::Load_Layouts() {
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

			file.close();
			++layouts_count;
		}

		return (layouts_count > 0) ? true : false;
	}

	void MPL_ImGui::Render_Engine_Layout() {
		// If current layout is deleted or missing, use default one.
		if (!layouts.count(curr_layout)) {
			Build_Default_Layout();
			std::cerr << "LAYOUT ERROR: \"" << curr_layout << "\" layout missing. Building new a new one." << std::endl;
		}

		Start_Frame();
		for (auto& layout : layouts[curr_layout]) {
			ImGui::Begin(layout.first.c_str());
			ImGui::End();
		}

		Render_ImGui();
	}

	void MPL_ImGui::Build_Default_Layout(bool const set_active) {

		std::unordered_map<std::string, ImGuiID> window_name_to_dock_id;

		Start_Frame();

		ImVec2 workSize = ImGui::GetMainViewport()->WorkSize;

		ImVec2 workCenter = ImGui::GetMainViewport()->GetWorkCenter();

		ImGuiID rootId = ImGui::GetID("default");

		ImGui::DockBuilderRemoveNode(rootId);
		ImGui::DockBuilderAddNode(rootId);

		ImVec2 size{ workSize.x, workSize.y };
		ImVec2 nodePos{ workCenter.x - size.x * 0.5f, workCenter.y - size.y * 0.5f };

		// Set the size and position:
		ImGui::DockBuilderSetNodeSize(rootId, size);
		ImGui::DockBuilderSetNodePos(rootId, nodePos);

		ImGuiID hierarchy_dock = ImGui::DockBuilderSplitNode(
			rootId, ImGuiDir_Left, 0.8f, nullptr, &rootId);

		ImGuiID inspector_dock = ImGui::DockBuilderSplitNode(
			rootId, ImGuiDir_Right, 0.2f, nullptr, &rootId);

		ImGuiID project_dock = ImGui::DockBuilderSplitNode(
			hierarchy_dock, ImGuiDir_Down, 0.3f, nullptr, &hierarchy_dock);

		ImGuiID scene_dock = ImGui::DockBuilderSplitNode(
			hierarchy_dock, ImGuiDir_Right, 0.8f, nullptr, &hierarchy_dock);

		ImGui::DockBuilderDockWindow("Hierarchy", hierarchy_dock);
		ImGui::DockBuilderDockWindow("Inspector", inspector_dock);
		ImGui::DockBuilderDockWindow("Project", project_dock);
		ImGui::DockBuilderDockWindow("Scene", scene_dock);

		window_name_to_dock_id["default"] = rootId;
		window_name_to_dock_id["Hierarchy"] = hierarchy_dock;
		window_name_to_dock_id["Inspector"] = inspector_dock;
		window_name_to_dock_id["Project"] = project_dock;
		window_name_to_dock_id["Scene"] = scene_dock;
		layouts[DEFAULT_LAYOUT_NAME] = window_name_to_dock_id;

		ImGui::DockBuilderFinish(rootId);

		ImGui::EndFrame();

		if (set_active) curr_layout = DEFAULT_LAYOUT_NAME;
	}
}

