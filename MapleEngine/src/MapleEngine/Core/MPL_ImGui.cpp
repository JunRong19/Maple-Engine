#include <pch.h>
#include <MPL_ImGui.h>
#include <MPL_Engine.h>
#include <MPL_Configs.h>
#include <MPL_Input.h>
#include <filesystem>

namespace MPL {
	// Disable all docks at the start.


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

		MPL::Input.Subscribe_To_Key('A', TRIGGERED, MPL_ImGui::Switch_Layout);
	}

	void MPL_ImGui::Switch_Layout() {
		curr_layout = "backup-layout";



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

		// Show windows that are part of current layout.
		layout_fmt& layout = layouts[curr_layout];
		for (auto const& dock : layout) {
			switch (dock.first)
			{
			case INSPECTOR: show_inspector = true; break;
			case PROJECT: show_project = true; break;
			case HIERARCHY: show_hierarchy = true; break;
			case SCENE: show_scene = true; break;
			case GAME: show_game = true; break;
			case LIGHTING: show_lighting = true; break;
			default: break;
			}
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
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options")) {
				if (ImGui::MenuItem("Lock Windows", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }

				if (ImGui::MenuItem("Show Background", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, true)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Windows"))
			{
				if (ImGui::MenuItem("Inspector", "", show_inspector)) { show_inspector = !show_inspector; }
				if (ImGui::MenuItem("Project", "", show_project)) { show_project = !show_project; }
				if (ImGui::MenuItem("Hierarchy", "", show_hierarchy)) { show_hierarchy = !show_hierarchy; }
				if (ImGui::MenuItem("Scene", "", show_scene)) { show_scene = !show_scene; }
				if (ImGui::MenuItem("Game", "", show_game)) { show_game = !show_game; }
				if (ImGui::MenuItem("Lighting", "", show_lighting)) { show_lighting = !show_lighting; }

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Layouts"))
			{
				ImGui::BeginListBox(" ");
				for (auto const& layout : layouts) {
					if (ImGui::Selectable(layout.first.c_str(), layout.first == curr_layout)) {
						curr_layout = layout.first.c_str();

						//Remove previous docks
						ImGui::DockBuilderRemoveNodeDockedWindows(dockspace_id);

						// Set new layout.
						for (auto const& dock : layouts[curr_layout]) {
							ImGui::DockBuilderDockWindow(dock.second.name.c_str(), dock.second.id);
						}
						ImGui::DockBuilderFinish(dockspace_id);
					}
				}
				ImGui::EndListBox();

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}

	void MPL_ImGui::Render_Dockspace() {
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

			// Load all layouts in resource folder and set a layout as active.
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
		std::unordered_map<DOCK_TYPE, Dock> dock_name_to_id;	// Store docks and their IDs.

		// Get name of layout.
		getline(file, line);
		std::istringstream ss_line{ line };
		ss_line >> layout_name;

		// Create docks.
		while (getline(file, line)) {
			// Mark as end of file content.
			if (line == "") break;

			std::istringstream ss_line{ line };

			char build_mode;		// Create dock or dock tab.
			int dock_type;			// Type of dock.
			int direction;			// 0: LEFT | 1: RIGHT | 2: UP | 3: DOWN | -1: DOCKSPACE
			float size_ratio{};		// Size ratio of dock.
			int parent;	// Parent dock of dock tab.
			Dock dock;

			// Get type of dock to create.
			ss_line >> build_mode;

			switch (build_mode)
			{
			case 'd':
				// Get dock info.
				ss_line >> dock.name >> dock_type >> direction >> size_ratio;
				// Check if the this type of dock has already been initialized.
				if (dock_name_to_id.count(static_cast<DOCK_TYPE>(dock_type))) {
					std::cerr << "ERROR: Dock type has already been initialized: " << dock.name << " dock." << std::endl;
					continue;
				}
				// Create ontop of dock space.
				if (static_cast<ImGuiDir>(direction) == ImGuiDir_None) {
					// Add to map.
					dock.id = dockspace_id;
					dock_name_to_id[static_cast<DOCK_TYPE>(dock_type)] = dock;
					// Create dock.
					ImGui::DockBuilderDockWindow(dock.name.c_str(), dockspace_id);
				}
				else {
					// Split dockspace for new dock.
					ImGuiID new_dock_id = ImGui::DockBuilderSplitNode(dockspace_id, static_cast<ImGuiDir>(direction), size_ratio, nullptr, &dockspace_id);
					// Add to map.
					dock.id = new_dock_id;
					dock_name_to_id[static_cast<DOCK_TYPE>(dock_type)] = dock;
					// Create dock.
					ImGui::DockBuilderDockWindow(dock.name.c_str(), new_dock_id);
				}
				break;
			case 't':
				// Create tab dock info.
				ss_line >> dock.name >> dock_type >> parent;
				// Check if the this type of dock has already been initialized.
				if (dock_name_to_id.count(static_cast<DOCK_TYPE>(dock_type))) {
					std::cerr << "ERROR: Dock type has already been initialized: " << dock.name << " dock." << std::endl;
					continue;
				}
				// Check if parent dock had been created.
				if (dock_name_to_id.count(static_cast<DOCK_TYPE>(parent))) {
					// Get parent dock id.
					Dock parent_dock = dock_name_to_id[static_cast<DOCK_TYPE>(parent)];
					// Create tab dock in parent dock.
					dock.id = parent_dock.id;
					ImGui::DockBuilderDockWindow(dock.name.c_str(), parent_dock.id);
					// Add to map.
					dock_name_to_id[static_cast<DOCK_TYPE>(dock_type)] = dock;
				}
				else {
					std::cerr << "ERROR: Parent dock not found. Unable to build " << dock.name << " dock." << std::endl;
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

		// Display enabled docks.
		if (show_inspector) Show_Inspector();
		if (show_project) Show_Project();
		if (show_hierarchy) Show_Hierarchy();
		if (show_scene) Show_Scene();
		if (show_game) Show_Game();
		if (show_lighting) Show_Lighting();
	}

	void MPL_ImGui::Show_Inspector() {
		layout_fmt& layout = layouts[curr_layout];
		std::string name;

		// Check if inspector dock is part of current layout...
		if ((layout.count(INSPECTOR))) {
			// Get dock title name.
			name = layout[INSPECTOR].name.c_str();
		}
		else {
			// Set default title for inspector dock.
			name = INSPECTOR_WINDOW_TITLE;
			// Add to dockspace.
			ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
		}

		// Start inspector dock.
		if (ImGui::Begin(name.c_str(), &show_inspector)) {

		}
		ImGui::End();
	}

	void MPL_ImGui::Show_Project() {
		layout_fmt& layout = layouts[curr_layout];
		std::string name;

		// Check if project dock is part of current layout...
		if ((layout.count(PROJECT))) {
			// Get dock title name.
			name = layout[PROJECT].name.c_str();
		}
		else {
			// Set default title for project dock.
			name = PROJECT_WINDOW_TITLE;
			// Add to dockspace.
			ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
		}

		// Start project dock.
		if (ImGui::Begin(name.c_str(), &show_project)) {

		}
		ImGui::End();
	}

	void MPL_ImGui::Show_Hierarchy() {
		layout_fmt& layout = layouts[curr_layout];
		std::string name;

		// Check if hierarchy dock is part of current layout...
		if ((layout.count(HIERARCHY))) {
			// Get dock title name.
			name = layout[HIERARCHY].name.c_str();
		}
		else {
			// Set default title for project dock.
			name = HIERARCHY_WINDOW_TITLE;
			// Add to dockspace.
			ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
		}

		// Start hierarchy dock.
		if (ImGui::Begin(name.c_str(), &show_hierarchy)) {

		}
		ImGui::End();
	}

	void MPL_ImGui::Show_Scene() {
		layout_fmt& layout = layouts[curr_layout];
		std::string name;

		// Check if scene dock is part of current layout...
		if ((layout.count(SCENE))) {
			// Get dock title name.
			name = layout[SCENE].name.c_str();
		}
		else {
			// Set default title for scene dock.
			name = SCENE_WINDOW_TITLE;
			// Add to dockspace.
			ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
		}

		// Start scene dock.
		if (ImGui::Begin(name.c_str(), &show_scene)) {

		}
		ImGui::End();
	}

	void MPL_ImGui::Show_Game() {
		layout_fmt& layout = layouts[curr_layout];
		std::string name;

		// Check if game dock is part of current layout...
		if ((layout.count(GAME))) {
			// Get dock title name.
			name = layout[GAME].name.c_str();
		}
		else {
			// Set default title for game dock.
			name = GAME_WINDOW_TITLE;
			// Add to dockspace.
			ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
		}

		// Start game dock.
		if (ImGui::Begin(name.c_str(), &show_game)) {

		}
		ImGui::End();
	}

	void MPL_ImGui::Show_Lighting() {
		layout_fmt& layout = layouts[curr_layout];
		std::string name;

		// Check if lighting dock is part of current layout...
		if ((layout.count(LIGHTING))) {
			// Get dock title name.
			name = layout[LIGHTING].name.c_str();
		}
		else {
			// Set default title for game dock.
			name = LIGHTING_WINDOW_TITLE;
			// Add to dockspace.
			ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
		}

		// Start lighting dock.
		if (ImGui::Begin(name.c_str(), &show_lighting)) {

		}
		ImGui::End();
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

