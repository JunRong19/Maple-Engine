#pragma once
namespace MPL {
	class MPL_ImGui {
		enum DOCK_TYPE {
			INSPECTOR = 0,
			PROJECT,
			HIERARCHY,
			SCENE,
			GAME,
			LIGHTING,
			NONE
		};

		enum THEME {
			CLASSIC,
			DARK,
			LIGHT
		};

		struct Dock {
			char build_type;	// Initial build mode: Dock / tab.
			DOCK_TYPE type;		// What type of dock this is?
			ImGuiDir direction;	// Build direction.
			float size_ratio;	// Build size ratio.

			DOCK_TYPE parent_dock;	// For tab docks.
		};

		using Docks = std::vector<Dock>;

		struct Layout {
			// Contains all docks in this layout.
			Docks docks;

			// Dock flags.
			bool has_inspector;
			bool has_project;
			bool has_hierarchy;
			bool has_scene;
			bool has_game;
			bool has_lighting;
		};

	public:
		static MPL_ImGui& Ref() {
			static MPL_ImGui ref;
			return ref;
		}

		void Initialize();
		void Draw();

	private:
		MPL_ImGui();
		~MPL_ImGui();
		MPL_ImGui(MPL_ImGui const&) = delete;
		MPL_ImGui operator=(MPL_ImGui const&) = delete;

	private:
		void Start_ImGui_Render();
		void End_ImGui_Render();

		void Enable_Dockspace();
		void Render_Dockspace();
		void Render_Engine_Layout();

		void Initialize_Layouts();
		void Build_Default_Layout(bool const set_active = true, bool const create_file = false);

		void Load_Layout(std::string const& file_path);
		bool const Load_Layouts_From_Resource();

		static void Show_Inspector();
		static void Show_Project();
		static void Show_Hierarchy();
		static void Show_Scene();
		static void Show_Game();
		static void Show_Lighting();

		static void Switch_Layout();
		static void Set_Layout(std::string const& new_layout);
		static void Update_Dock_Visiblity();

		static void Set_Theme(THEME const theme);

		static DOCK_TYPE Dock_Id_To_Type(int const id);
		static std::string Dock_Type_To_Name(DOCK_TYPE const type);
	private:

		inline static ImGuiID dockspace_id;
		static constexpr const char* DOCKSPACE_NAME = "main-dockspace";
		inline static ImGuiDockNodeFlags dockspace_flags;

		uint16_t layouts_count;
		inline static std::string curr_layout;
		inline static std::unordered_map<std::string, Layout> layouts;

		static constexpr const char* BACKUP_LAYOUT = "backup-layout";
		static constexpr const char* BACKUP_LAYOUT_PATH = "res/dock_layouts/backup-layout.txt";
		static constexpr const char* BACKUP_LAYOUT_FILE_CONTENT
			= "backup-layout\n"
			"d	0	1	0.2\n"
			"d	1	3	0.3\n"
			"d	2	0	0.2\n"
			"d	3  -1	0  \n"
			"t	4	3	   \n";

		static constexpr const char* INSPECTOR_WINDOW_TITLE = "Inspector";
		static constexpr const char* PROJECT_WINDOW_TITLE = "Project";
		static constexpr const char* HIERARCHY_WINDOW_TITLE = "Hierarchy";
		static constexpr const char* SCENE_WINDOW_TITLE = "Scene";
		static constexpr const char* GAME_WINDOW_TITLE = "Game";
		static constexpr const char* LIGHTING_WINDOW_TITLE = "Lighting";

		inline static bool show_inspector = false;
		inline static bool show_project = false;
		inline static bool show_hierarchy = false;
		inline static bool show_scene = false;
		inline static bool show_game = false;
		inline static bool show_lighting = false;

		inline static THEME curr_theme = LIGHT;
	};
	static MPL_ImGui& ImGui = MPL_ImGui::Ref();
}
