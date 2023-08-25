#pragma once
namespace MPL {
	class MPL_ImGui {
		enum DOCK_TYPE {
			INSPECTOR = 0,
			PROJECT,
			HIERARCHY,
			SCENE,
			GAME,
			LIGHTING
		};

		struct Dock {
			ImGuiID id;

			std::string name;
			char type;
			int window_type;
			ImGuiDir direction;
			float size_ratio;

			std::string parent_dock;
		};
		using layout_fmt = std::vector<Dock>;

		struct Layout {
			layout_fmt docks;

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
		void Load_Layout(std::string file_path);
		bool const Load_Layouts_From_Resource();
		void Build_Default_Layout(bool const set_active = true, bool const create_file = false);

		static void Show_Inspector();
		static void Show_Project();
		static void Show_Hierarchy();
		static void Show_Scene();
		static void Show_Game();
		static void Show_Lighting();

		static void Switch_Layout();
		static void Set_Layout();
		static void Update_Dock_Visiblity();
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
			"d Inspector 0 1 0.2\n"
			"d Project 1 3 0.3\n"
			"d Hierarchy 2 0 0.2\n"
			"d Scene 3 -1 0\n"
			"t Game 4 Scene\n";

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
	};
	static MPL_ImGui& ImGui = MPL_ImGui::Ref();
}
