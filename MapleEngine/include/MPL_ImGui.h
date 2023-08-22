#pragma once
namespace MPL {
	class MPL_ImGui {
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

	public:
		inline static std::unordered_map<std::string, std::unordered_map<std::string, ImGuiID>> layouts;

	private:
		static constexpr const char* DOCKSPACE_NAME = "main-dockspace";
		inline static ImGuiDockNodeFlags dockspace_flags;
		std::string curr_layout;
		uint16_t layouts_count;
		ImGuiID dockspace_id;

		static constexpr const char* BACKUP_LAYOUT = "backup-layout";
		static constexpr const char* BACKUP_LAYOUT_PATH = "res/dock_layouts/backup-layout.txt";
		static constexpr const char* BACKUP_LAYOUT_FILE_CONTENT
			= "backup-layout\n"
			"d Inspector 1 0.2\n"
			"d Project 3 0.4\n"
			"d Hierarchy 0 0.2\n"
			"d Scene -1 0\n"
			"t Game Scene\n";
	};
	static MPL_ImGui& ImGui = MPL_ImGui::Ref();
}
