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

		// Load and create UI layout for engine using ImGui.
		//Initialize_Engine_UI();
		curr_layout = "main";
		//Initialize_Dockspace();
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
		Start_ImGui_Render();
		//ImGui::ShowDemoWindow();
		ShowExampleAppDockSpace();
		Render_Engine_Layout();
		End_ImGui_Render();
	}

	void MPL_ImGui::Initialize_Dockspace() {
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;


		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", nullptr, window_flags);
		ImGui::PopStyleVar();
		ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID(DOCKSPACE_NAME);
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

			ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
			ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

			ImGui::DockBuilderFinish(dockspace_id);
		}
		ImGui::End();
	}

	void MPL_ImGui::Initialize_Docks() {
	}

	void MPL_ImGui::ShowExampleAppDockSpace()
	{
		// READ THIS !!!
		// TL;DR; this demo is more complicated than what most users you would normally use.
		// If we remove all options we are showcasing, this demo would become:
		//     void ShowExampleAppDockSpace()
		//     {
		//         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		//     }
		// In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
		// In this specific demo, we are not using DockSpaceOverViewport() because:
		// - (1) we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
		// - (2) we allow the host window to have padding (when opt_padding == true)
		// - (3) we expose many flags and need a way to have them visible.
		// - (4) we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport()
		//      in your code, but we don't here because we allow the window to be floating)

		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;


		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", nullptr, window_flags);
		ImGui::PopStyleVar();
		ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			dockspace_id = ImGui::GetID(DOCKSPACE_NAME);
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

			static auto first_time = true;
			if (first_time)
			{
				first_time = false;
				ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
				ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
				ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

				ImGui::DockBuilderFinish(dockspace_id);

				std::string path = "res/dock_layouts";
				// Iterate through all layouts in path.
				for (const auto& entry : std::filesystem::directory_iterator(path)) {
					std::ifstream file{ entry.path()  ,std::ios::in };
					if (!file) {
						std::cerr << "ERROR: Unable to load layout at: " << entry.path() << std::endl;
					}

					std::string layout_name;	// Name of this layout.
					std::string line;			// Line buffer when reading file contents.
					std::unordered_map<std::string, ImGuiID> dock_name_to_id;	// Store docks and their IDs.

					// Get name of layout.
					getline(file, line);
					std::istringstream ss_line{ line };
					ss_line >> layout_name;

					// Create docks.
					while (getline(file, line)) {

						std::istringstream ss_line{ line };

						char dock_build_mode;	// Create dock or dock tab.
						std::string dock_name;	// Name of dock.
						int direction;			// 0: LEFT | 1: RIGHT | 2: UP | 3: DOWN
						float size_ratio{};		// Size ratio of dock.
						std::string parent_dock;// Parent dock of dock tab.

						// Get type of dock to create.
						ss_line >> dock_build_mode;

						switch (dock_build_mode)
						{
						case 'd':
							// Get dock info.
							ss_line >> dock_name >> direction >> size_ratio;
							// Create ontop of dock space.
							if (static_cast<ImGuiDir>(direction) == ImGuiDir_None) {
								// Add to map.
								dock_name_to_id[dock_name] = dockspace_id;
								// Create dock.
								ImGui::DockBuilderDockWindow(dock_name.c_str(), dockspace_id);
							}
							else {
								// Split dockspace for new dock.
								ImGuiID dock = ImGui::DockBuilderSplitNode(dockspace_id, static_cast<ImGuiDir>(direction), size_ratio, nullptr, &dockspace_id);
								// Add to map.
								dock_name_to_id[dock_name] = dock;
								// Create dock.
								ImGui::DockBuilderDockWindow(dock_name.c_str(), dock);
							}
							break;
						case 't':
							// Create tab dock info.
							ss_line >> dock_name >> parent_dock;
							// Check if parent dock had been created.
							if (dock_name_to_id.count(parent_dock)) {
								// Get parent dock id.
								ImGuiID parent_id = dock_name_to_id[parent_dock];
								// Create tab dock in parent dock.
								ImGui::DockBuilderDockWindow(dock_name.c_str(), parent_id);
								// Add to map.
								dock_name_to_id[dock_name] = parent_id;
							}
							else {
								std::cerr << "ERROR: Parent dock not found. Unable to build " << dock_name << " dock." << std::endl;
							}
							break;
						default:
							std::cerr << "ERROR: Incorrect build mode prefix." << std::endl;
							break;
						}
					}
					// End docks building process.
					ImGui::DockBuilderFinish(dockspace_id);
					file.close();

					++layouts_count;
					layouts[layout_name] = dock_name_to_id;
				}
			}
		}
		ImGui::End();
		Render_Engine_Layout();
	}
	void MPL_ImGui::Start_ImGui_Render() {
		// Create new ImGui frame for rendering.
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void MPL_ImGui::End_ImGui_Render() {
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
			Start_ImGui_Render();

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
		//if (!layouts.count(curr_layout)) {
		//	Build_Default_Layout(true);
		//	std::cerr << "LAYOUT ERROR: \"" << curr_layout << "\" layout missing. Building new a new one." << std::endl;
		//}

		// Build engine's layout.
		//Start_Frame();


		for (auto& dock : layouts[curr_layout]) {
			ImGui::Begin(dock.first.c_str());
			ImGui::End();
		}

		//ImGui::SetNextWindowDockID(layouts[curr_layout]["Project"]);
		//ImGui::Begin("dawdaw");
		//ImGui::End();
		//Render_ImGui();
	}

	void MPL_ImGui::Build_Default_Layout(bool const set_active) {
		std::unordered_map<std::string, ImGuiID> window_name_to_dock_id;

		// Begin building process.
		Start_ImGui_Render();

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

