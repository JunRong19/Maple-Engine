#pragma once
namespace MPL {
	// Window
	const size_t WINDOW_WIDTH = 1920;
	const size_t WINDOW_HEIGHT = 1080;

	class MPL_Engine {
	public:
		MPL_Engine();
		MPL_Engine(MPL_Engine const&) = delete;
		MPL_Engine operator=(MPL_Engine const&) = delete;
		~MPL_Engine();

		static MPL_Engine& Ref() {
			static MPL_Engine ref;
			return ref;
		}

		void Initialize();
		void Update();
		void Quit();

		inline GLFWwindow* Window() { return window; }
		inline const bool Run() const { return run; }

		inline const size_t Get_Window_Width() { return WINDOW_WIDTH; }
		inline const size_t Get_Window_Height() { return WINDOW_HEIGHT; }


	private:
		bool run;
		GLFWwindow* window;
		std::string window_title;

	private:
		void Set_Window_Title();
	};

	static MPL_Engine& Core = MPL_Engine::Ref();
}
