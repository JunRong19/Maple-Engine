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

		// Enable docking.
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}

	void MPL_ImGui::Initialize_Layouts() {

		// Load all layouts available in res folder.
		bool layouts_loaded = Load_Layouts_From_Resource();
		MPL_Configs configs;

		// At least one layout is found in res folder.
		if (layouts_loaded) {
			// Check if saved layout is loaded.
			if (layouts.count(configs.Get_Data().at("LAYOUT"))) {
				// Set current layout as saved layout.
				curr_layout = configs.Get_Data().at("LAYOUT");
			}
			else {
				// Create and load a backup layout as fallback.
				Build_Default_Layout(true, true);
				// Replace old save layout with a default layout.
				configs.Save(std::make_pair("LAYOUT", BACKUP_LAYOUT));
				std::cerr << "ERROR: Unable to find saved layout. Using default layout." << std::endl;
			}
		}
		// No layout found...
		else {
			// Create and load a backup layout as fallback.
			Build_Default_Layout(true, true);
			// Replace old save layout with a default layout.
			configs.Save(std::make_pair("LAYOUT", BACKUP_LAYOUT));
			std::cerr << "ERROR: No layout found in res folder. Using default layout." << std::endl;
		}
	}

	void MPL_ImGui::Draw() {
		Start_ImGui_Render();
		//ImGui::ShowDemoWindow();
		Render_Dockspace();
		Render_Engine_Layout();

		End_ImGui_Render();
	}

	void MPL_ImGui::Enable_Dockspace() {
		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			dockspace_id = ImGui::GetID(DOCKSPACE_NAME);
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
	}

	void MPL_ImGui::Render_Dockspace() {
		dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

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

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", nullptr, window_flags);
		ImGui::PopStyleVar();
		ImGui::PopStyleVar(2);

		Enable_Dockspace();

		// One-time initialization.
		// Dockspace has to be enabled once first to get it's id in order to modify it.
		static auto first_time = true;
		if (first_time)
		{
			first_time = false;
			// Configure dockspace.
			ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
			ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);
			ImGui::DockBuilderFinish(dockspace_id);

			// Load all layouts in resource folder set a layout as active.
			Initialize_Layouts();
		}

		ImGui::End();
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

	bool const MPL_ImGui::Load_Layouts_From_Resource() {
		std::string path = "res/dock_layouts";
		// Iterate through all layouts in path.
		for (const auto& entry : std::filesystem::directory_iterator(path)) {
			Load_Layout(entry.path().string());
		}
		return (layouts_count > 0) ? true : false;
	}

	void MPL_ImGui::Load_Layout(std::string file_path) {
		std::ifstream file{ file_path  ,std::ios::in };
		if (!file) {
			std::cerr << "ERROR: Unable to load layout at: " << file_path << std::endl;
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

	void MPL_ImGui::Render_Engine_Layout() {
		if (!layouts.count(curr_layout)) {
			std::cerr << "ERROR: \"" << curr_layout << "\" layout missing." << std::endl;
			return;
		}

		for (auto& dock : layouts[curr_layout]) {
			ImGui::Begin(dock.first.c_str());
			ImGui::End();
		}
	}

	void MPL_ImGui::Build_Default_Layout(bool const set_active, bool const create_file) {

		const char* path = "res/dock_layouts/temp-layout.txt";
		std::ofstream ofile{ path };
		if (!ofile) {
			std::cerr << "ERROR: Unable to write to file path: " << path << std::endl;
		}
		// Create a new config file.
		ofile << BACKUP_LAYOUT_FILE_CONTENT;

		ofile.close();

		Load_Layout(path);

		remove(path);

		// Set as current layout if needed.
		if (set_active) curr_layout = BACKUP_LAYOUT;

		if (create_file) {
			const char* path = BACKUP_LAYOUT_PATH;
			std::ofstream ofile{ path };
			if (!ofile) {
				std::cerr << "ERROR: Unable to write to file path: " << path << std::endl;
			}

			// Create a new config file.
			ofile << BACKUP_LAYOUT_FILE_CONTENT;

			ofile.close();
		}
	}
}

