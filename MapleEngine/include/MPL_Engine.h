#pragma once
namespace MPL {
	// Window
	const size_t WINDOW_WIDTH = 800;
	const size_t WINDOW_HEIGHT = 800;

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

	private:
		GLFWwindow* window;
		bool run;
	};

	static MPL_Engine& Core = MPL_Engine::Ref();
}
