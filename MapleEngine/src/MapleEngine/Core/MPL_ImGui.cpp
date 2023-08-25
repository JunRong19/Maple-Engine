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
		Set_Theme(static_cast<THEME>(std::stoi(MPL_Configs::Get_Data().at("THEME"))));

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
				std::cerr << "ERROR: Unable to find saved layout. Using back-up layout." << std::endl;
			}
		}
		// No layout found...
		else {
			// Create and load a backup layout as fallback.
			Build_Default_Layout(true, true);
			// Replace old save layout with a default layout.
			configs.Save(std::make_pair("LAYOUT", BACKUP_LAYOUT));
			std::cerr << "ERROR: No layout found in res folder. Using back-up layout." << std::endl;
		}

		Set_Layout(curr_layout);
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

				if (ImGui::Selectable("Reset Layout")) { Set_Layout(curr_layout); }
				ImGui::Separator();

				for (auto const& layout : layouts) {
					if (ImGui::Selectable(layout.first.c_str(), layout.first == curr_layout)) {
						// Set new layout on dockspace.
						Set_Layout(layout.first.c_str());

						break;
					}
				}
				ImGui::EndListBox();

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Themes"))
			{
				ImGui::BeginListBox(" ");
				if (ImGui::Selectable("Classic", curr_theme == CLASSIC)) { Set_Theme(CLASSIC); }
				if (ImGui::Selectable("Light", curr_theme == LIGHT)) { Set_Theme(LIGHT); }
				if (ImGui::Selectable("Dark", curr_theme == DARK)) { Set_Theme(DARK); }

				ImGui::EndListBox();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}

	void MPL_ImGui::Set_Theme(THEME const& theme) {
		curr_theme = theme;

		switch (theme) {
		case CLASSIC: ImGui::StyleColorsClassic(); break;
		case LIGHT: ImGui::StyleColorsLight(); break;
		case DARK: ImGui::StyleColorsDark(); break;
		}

		MPL_Configs configs;
		configs.Save(std::make_pair("THEME", std::to_string(theme)));
	}

	void MPL_ImGui::Update_Dock_Visiblity() {
		// Show docks that are part of current layout.
		Layout& layout = layouts[curr_layout];
		show_inspector = layout.has_inspector ? true : false;
		show_project = layout.has_project ? true : false;
		show_hierarchy = layout.has_hierarchy ? true : false;
		show_scene = layout.has_scene ? true : false;
		show_game = layout.has_game ? true : false;
		show_lighting = layout.has_lighting ? true : false;

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
		ImGui::Begin("Window", nullptr, window_flags);
		ImGui::PopStyleVar();
		ImGui::PopStyleVar(2);

		Enable_Dockspace();

		// One-time initialization.
		// Dockspace has to be enabled once first to get it's id in order to modify it.
		static auto first_time = true;
		if (first_time)
		{
			first_time = false;
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

	void MPL_ImGui::Set_Layout(std::string new_layout) {

		// Reset dockspace. Unparent all child windows.
		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

		curr_layout = new_layout;
		Layout layout = layouts[curr_layout];

		std::unordered_map<std::string, ImGuiID> cache;

		for (auto const& dock : layout.docks) {
			ImGuiWindow* parent_window;
			ImGuiID parent_id;
			switch (dock.type)
			{
			case 'd':
				// Create ontop of dock space.
				if (static_cast<ImGuiDir>(dock.direction) == ImGuiDir_None) {
					// Create dock.
					cache[dock.name] = dockspace_id;
					ImGui::DockBuilderDockWindow(dock.name.c_str(), dockspace_id);
				}
				else {
					// Split dockspace for new dock.
					ImGuiID new_dock_id = ImGui::DockBuilderSplitNode(dockspace_id, static_cast<ImGuiDir>(dock.direction), dock.size_ratio, nullptr, &dockspace_id);
					// Create dock.
					cache[dock.name] = new_dock_id;
					ImGui::DockBuilderDockWindow(dock.name.c_str(), new_dock_id);
				}
				break;
			case 't':
				// Get parent id.
				parent_id = cache[dock.parent_dock];
				// Create tab dock in parent dock.
				ImGui::DockBuilderDockWindow(dock.name.c_str(), parent_id);
				cache[dock.name] = parent_id;
				break;
			default:
				std::cerr << "ERROR: Incorrect build mode prefix." << std::endl;
				break;
			}
		}
		ImGui::DockBuilderFinish(dockspace_id);
		// Update flags to only show docks that are part of the layout.
		Update_Dock_Visiblity();

		MPL_Configs configs;
		configs.Save(std::make_pair("LAYOUT", curr_layout));
	}

	void MPL_ImGui::Load_Layout(std::string file_path) {
		std::ifstream file{ file_path  ,std::ios::in };
		if (!file) {
			std::cerr << "ERROR: Unable to load layout at: " << file_path << std::endl;
		}

		std::string layout_name;	// Name of this layout.
		std::string line;			// Line buffer when reading file contents.
		Docks dock_name_to_id;	// Store docks and their IDs.

		// Get name of layout.
		getline(file, line);
		std::istringstream ss_line{ line };
		ss_line >> layout_name;

		// Create docks.
		while (getline(file, line)) {
			// Mark as end of file content.
			if (line == "") break;

			std::istringstream ss_line{ line };

			Dock dock;

			// Get type of dock to create.
			ss_line >> dock.type;

			switch (dock.type)
			{
			case 'd':
				// Get dock info.
				ss_line >> dock.name >> dock.window_type >> dock.direction >> dock.size_ratio;
				break;
			case 't':
				// Create tab dock info.
				ss_line >> dock.name >> dock.window_type >> dock.parent_dock;

			}

			switch (dock.window_type)
			{
			case 0:
				layouts[layout_name].has_inspector = true;
				break;
			case 1:
				layouts[layout_name].has_project = true;
				break;
			case 2:
				layouts[layout_name].has_hierarchy = true;
				break;
			case 3:
				layouts[layout_name].has_scene = true;
				break;
			case 4:
				layouts[layout_name].has_game = true;
				break;
			default:
				break;
			}
			dock_name_to_id.emplace_back(dock);
		}
		file.close();

		++layouts_count;
		layouts[layout_name].docks = dock_name_to_id;
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
		Docks& layout = layouts[curr_layout].docks;
		std::string name;

		// Check if inspector dock is part of current layout...
		//if ((layout.count(INSPECTOR))) {
		//	// Get dock title name.
		//	name = layout[INSPECTOR].name.c_str();
		//}
		//else {
		//	// Set default title for inspector dock.
		//	name = INSPECTOR_WINDOW_TITLE;
		//	// Add to dockspace.
		//	ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
		//}

		// Start inspector dock.
		if (ImGui::Begin(INSPECTOR_WINDOW_TITLE, &show_inspector)) {

		}
		ImGui::End();
	}

	void MPL_ImGui::Show_Project() {
		Docks& layout = layouts[curr_layout].docks;
		std::string name;

		//// Check if project dock is part of current layout...
		//if ((layout.count(PROJECT))) {
		//	// Get dock title name.
		//	name = layout[PROJECT].name.c_str();
		//}
		//else {
		//	// Set default title for project dock.
		//	name = PROJECT_WINDOW_TITLE;
		//	// Add to dockspace.
		//	ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
		//}

		// Start project dock.
		if (ImGui::Begin("Project", &show_project)) {

		}
		ImGui::End();
	}

	void MPL_ImGui::Show_Hierarchy() {
		Docks& layout = layouts[curr_layout].docks;
		std::string name;

		//// Check if hierarchy dock is part of current layout...
		//if ((layout.count(HIERARCHY))) {
		//	// Get dock title name.
		//	name = layout[HIERARCHY].name.c_str();
		//}
		//else {
		//	// Set default title for project dock.
		//	name = HIERARCHY_WINDOW_TITLE;
		//	// Add to dockspace.
		//	ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
		//}

		// Start hierarchy dock.
		if (ImGui::Begin("Hierarchy", &show_hierarchy)) {

		}
		ImGui::End();
	}

	void MPL_ImGui::Show_Scene() {
		Docks& layout = layouts[curr_layout].docks;
		std::string name;

		//// Check if scene dock is part of current layout...
		//if ((layout.count(SCENE))) {
		//	// Get dock title name.
		//	name = layout[SCENE].name.c_str();
		//}
		//else {
		//	// Set default title for scene dock.
		//	name = SCENE_WINDOW_TITLE;
		//	// Add to dockspace.
		//	ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
		//}

		// Start scene dock.
		if (ImGui::Begin("Scene", &show_scene)) {

		}
		ImGui::End();
	}

	void MPL_ImGui::Show_Game() {
		Docks& layout = layouts[curr_layout].docks;
		std::string name;

		//// Check if game dock is part of current layout...
		//if ((layout.count(GAME))) {
		//	// Get dock title name.
		//	name = layout[GAME].name.c_str();
		//}
		//else {
		//	// Set default title for game dock.
		//	name = GAME_WINDOW_TITLE;
		//	// Add to dockspace.
		//	ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
		//}

		// Start game dock.
		if (ImGui::Begin("Game", &show_game)) {

		}
		ImGui::End();
	}

	void MPL_ImGui::Show_Lighting() {
		Docks& layout = layouts[curr_layout].docks;
		std::string name;

		//// Check if lighting dock is part of current layout...
		//if ((layout.count(LIGHTING))) {
		//	// Get dock title name.
		//	name = layout[LIGHTING].name.c_str();
		//}
		//else {
		//	// Set default title for game dock.
		//	name = LIGHTING_WINDOW_TITLE;
		//	// Add to dockspace.
		//	ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
		//}

		// Start lighting dock.
		if (ImGui::Begin("Lighting", &show_lighting)) {

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

