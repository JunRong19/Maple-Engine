#pragma once
namespace MPL {
	class MPL_ImGui {

	public:
		~MPL_ImGui();
		MPL_ImGui(MPL_ImGui const&) = delete;
		MPL_ImGui operator=(MPL_ImGui const&) = delete;

		static MPL_ImGui& Ref() {
			static MPL_ImGui ref;
			return ref;
		}

		void Initialize();
		void Draw();

	private:
		MPL_ImGui();
		void Build_Default_Layout();
		void Start_Frame();
		void Render_ImGui();
		void Load_Layouts();
		void Render_Engine_Layout();

	public:
		inline static std::unordered_map<std::string, std::unordered_map<std::string, ImGuiID>> layouts;
	};
	static MPL_ImGui& ImGui = MPL_ImGui::Ref();
}
