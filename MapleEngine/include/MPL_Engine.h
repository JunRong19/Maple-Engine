#pragma once
namespace MPL {
	class MPL_Engine {
	public:
		static MPL_Engine& Ref() {
			static MPL_Engine ref;
			return ref;
		}

		void Initialize();
		void Update();
		void Quit();

		inline GLFWwindow* Window() { return window; }
		inline bool const Run() const { return run; }

		inline  size_t const Get_Window_Width() { return WINDOW_WIDTH; }
		inline  size_t const Get_Window_Height() { return WINDOW_HEIGHT; }

	private:
		MPL_Engine();
		MPL_Engine(MPL_Engine const&) = delete;
		MPL_Engine operator=(MPL_Engine const&) = delete;
		~MPL_Engine();

	private:
		void Set_Window_Title();

	private:
		bool run;
		GLFWwindow* window;
		std::string window_title;
		// Window
		uint32_t WINDOW_WIDTH = 1920;
		uint32_t WINDOW_HEIGHT = 1080;
	};

	static MPL_Engine& Core = MPL_Engine::Ref();
}
