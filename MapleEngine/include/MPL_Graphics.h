#pragma once
namespace MPL {
	class MPL_Graphics {
	public:
		static MPL_Graphics& Ref() {
			static MPL_Graphics ref;
			return ref;
		}

		void Initialize();
		void Draw();

	private:
		MPL_Graphics();
		~MPL_Graphics();
		MPL_Graphics(MPL_Graphics const&) = delete;
		MPL_Graphics operator=(MPL_Graphics const&) = delete;
	};

	static MPL_Graphics& Graphics = MPL_Graphics::Ref();
}
