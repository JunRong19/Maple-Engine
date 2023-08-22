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
		void Build_Default_Layout(bool const set_active = true);
		void Start_ImGui_Render();
		void End_ImGui_Render();
		void ShowExampleAppDockSpace();

		void Render_Engine_Layout();
		void Initialize_Engine_UI();

		void Initialize_Dockspace();
		void Initialize_Docks();
		bool const Load_Layouts();


	public:
		inline static std::unordered_map<std::string, std::unordered_map<std::string, ImGuiID>> layouts;

	private:
		static constexpr const char* DOCKSPACE_NAME = "main";
		static constexpr const char* DEFAULT_LAYOUT_NAME = "main";
		std::string curr_layout;
		uint16_t layouts_count;
		ImGuiID dockspace_id;
	};
	static MPL_ImGui& ImGui = MPL_ImGui::Ref();
}
