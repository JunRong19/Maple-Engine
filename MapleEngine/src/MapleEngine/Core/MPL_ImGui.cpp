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
		// Initialize ImGui.
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(MPL::Core.Window(), true);
		ImGui_ImplOpenGL3_Init("#version 450");

		// Set ImGui window theme.
		ImGui::StyleColorsLight();

		// Access flags.
		ImGuiIO& io = ImGui::GetIO();

		// Set ImGui flags.
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiDockNodeFlags_NoDockingInCentralNode;

		// Load and create UI layout for engine using ImGui.
		Initialize_Engine_UI();
	}

	void MPL_ImGui::Initialize_Engine_UI() {
		// Load all layouts available in res folder.
		bool layout_loaded = Load_Layouts();

		// At least one layout is found in res folder.
		if (layout_loaded) {
			MPL_Configs configs;
			// Check if saved layout is loaded.
			if (layouts.count(configs.Get_Data().at("LAYOUT"))) {
				// Set current layout as saved layout.
				curr_layout = configs.Get_Data().at("LAYOUT");
			}
			else {
				// Create and load a default layout as fallback.
				Build_Default_Layout(true);
				// Replace old save layout with a default layout.
				configs.Save(std::make_pair("LAYOUT", DEFAULT_LAYOUT_NAME));
				std::cerr << "ERROR: Unable to find saved layout. Using default layout." << std::endl;
			}
		}
		// No layout found...
		else {
			// Create and load a default layout as fallback.
			Build_Default_Layout(true);
			std::cerr << "ERROR: No layout found in res folder. Using default layout." << std::endl;
		}
	}

	void MPL_ImGui::Draw() {
		Render_Engine_Layout();
	}

	void MPL_ImGui::Start_Frame() {
		// Create new ImGui frame for rendering.
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void MPL_ImGui::Render_ImGui() {
		// Render ImGui windows and end frame.
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	bool const MPL_ImGui::Load_Layouts() {
		std::string path = "res/dock_layouts";
		// Iterate through all layouts in path.
		for (const auto& entry : std::filesystem::directory_iterator(path)) {
			std::ifstream file{ entry.path()  ,std::ios::in };
			if (!file) {
				std::cout << "ERROR: Unable to load layout at: " << entry.path() << std::endl;
			}

			std::string line;
			std::string root_dock;
			std::unordered_map<std::string, ImGuiID> window_name_to_dock_id;

			// Get name of layout.
			getline(file, line);
			std::istringstream ss_line{ line };
			ss_line >> root_dock;

			// Start docks building process.
			Start_Frame();

			// Get screen dimensions.
			ImVec2 work_size = ImGui::GetMainViewport()->WorkSize;
			ImVec2 work_center = ImGui::GetMainViewport()->GetWorkCenter();
			// Get screen size.
			ImVec2 size{ work_size.x, work_size.y };
			// Get middle of screen.
			ImVec2 node_pos{ work_center.x - size.x * 0.5f, work_center.y - size.y * 0.5f };

			// Get root id.
			ImGuiID root_id = ImGui::GetID(root_dock.c_str());
			// Cache root node to map so other nodes can reference it.
			window_name_to_dock_id[root_dock] = root_id;
			// Clear previous dock with this root id.
			ImGui::DockBuilderRemoveNode(root_id);
			// Create a new root dock.
			ImGui::DockBuilderAddNode(root_id);
			// Set the size and position:
			ImGui::DockBuilderSetNodeSize(root_id, size);
			ImGui::DockBuilderSetNodePos(root_id, node_pos);

			// Create docks.
			while (getline(file, line)) {
				// Dock data:
				std::string dock_name;
				std::string parent_dock_str;
				int direction_int;	// 0: UP | 1: RIGHT | 2: DOWN | 3: LEFT
				ImGuiDir direction = ImGuiDir_None;
				float size_ratio{};

				// Get dock data from line.
				std::istringstream ss_line{ line };
				ss_line >> dock_name >> parent_dock_str >> direction_int >> size_ratio;

				// Initialize direction.
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

				// Get parent dock id. Note: Parent node should already been initialized.
				ImGuiID parent_id = window_name_to_dock_id.at(parent_dock_str);
				// Split parent dock into a new dock.
				ImGuiID dock = ImGui::DockBuilderSplitNode(parent_id, direction, size_ratio, nullptr, &parent_id);
				// Update id of parent dock. ImGui sometimes change parent id when spliting a dock.
				window_name_to_dock_id[parent_dock_str] = parent_id;
				// Add new dock to map.
				window_name_to_dock_id[dock_name] = dock;
				// Finish creating dock.
				ImGui::DockBuilderDockWindow(dock_name.c_str(), dock);
			}
			// End docks building process.
			ImGui::DockBuilderFinish(root_id);
			ImGui::EndFrame();

			// Remove root dock from being drawn.
			window_name_to_dock_id.erase(root_dock);
			layouts[root_dock] = window_name_to_dock_id;

			file.close();
			++layouts_count;
		}
		return (layouts_count > 0) ? true : false;
	}

	void MPL_ImGui::Render_Engine_Layout() {
		// If somehow current layout is missing, create and use the default layout.
		if (!layouts.count(curr_layout)) {
			Build_Default_Layout(true);
			std::cerr << "LAYOUT ERROR: \"" << curr_layout << "\" layout missing. Building new a new one." << std::endl;
		}

		// Build engine's layout.
		Start_Frame();
		for (auto& layout : layouts[curr_layout]) {
			ImGui::Begin(layout.first.c_str());
			ImGui::End();
		}
		Render_ImGui();
	}

	void MPL_ImGui::Build_Default_Layout(bool const set_active) {
		std::unordered_map<std::string, ImGuiID> window_name_to_dock_id;

		// Begin building process.
		Start_Frame();

		// Get screen dimensions.
		ImVec2 workSize = ImGui::GetMainViewport()->WorkSize;
		ImVec2 workCenter = ImGui::GetMainViewport()->GetWorkCenter();

		// Get root id.
		ImGuiID root_id = ImGui::GetID("default");
		// Set the size and position:
		ImGui::DockBuilderRemoveNode(root_id);
		ImGui::DockBuilderAddNode(root_id);

		// Get screen size.
		ImVec2 size{ workSize.x, workSize.y };
		// Get middle of screen.
		ImVec2 nodePos{ workCenter.x - size.x * 0.5f, workCenter.y - size.y * 0.5f };
		// Set the size and position:
		ImGui::DockBuilderSetNodeSize(root_id, size);
		ImGui::DockBuilderSetNodePos(root_id, nodePos);

		// Build docks:
		ImGuiID hierarchy_dock = ImGui::DockBuilderSplitNode(
			root_id, ImGuiDir_Left, 0.8f, nullptr, &root_id);

		ImGuiID inspector_dock = ImGui::DockBuilderSplitNode(
			root_id, ImGuiDir_Right, 0.2f, nullptr, &root_id);

		ImGuiID project_dock = ImGui::DockBuilderSplitNode(
			hierarchy_dock, ImGuiDir_Down, 0.3f, nullptr, &hierarchy_dock);

		ImGuiID scene_dock = ImGui::DockBuilderSplitNode(
			hierarchy_dock, ImGuiDir_Right, 0.8f, nullptr, &hierarchy_dock);

		// Finish creating dock.
		ImGui::DockBuilderDockWindow("Hierarchy", hierarchy_dock);
		ImGui::DockBuilderDockWindow("Inspector", inspector_dock);
		ImGui::DockBuilderDockWindow("Project", project_dock);
		ImGui::DockBuilderDockWindow("Scene", scene_dock);

		//window_name_to_dock_id["default"] = rootId;
		window_name_to_dock_id["Hierarchy"] = hierarchy_dock;
		window_name_to_dock_id["Inspector"] = inspector_dock;
		window_name_to_dock_id["Project"] = project_dock;
		window_name_to_dock_id["Scene"] = scene_dock;
		// Add dock to available layouts.
		layouts[DEFAULT_LAYOUT_NAME] = window_name_to_dock_id;

		// End docks building process.
		ImGui::DockBuilderFinish(root_id);
		ImGui::EndFrame();

		// Set default dock as current dock if needed.
		if (set_active) curr_layout = DEFAULT_LAYOUT_NAME;
	}
}

