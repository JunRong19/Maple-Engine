#pragma once
namespace MPL {
	class MPL_Events {
	public:
		~MPL_Events();
		MPL_Events(MPL_Events const&) = delete;
		MPL_Events operator=(MPL_Events const&) = delete;

		static MPL_Events& Ref() {
			static MPL_Events ref;
			return ref;
		}

		void Initialize();
		void Poll();

	private:
		MPL_Events();
	};

	static MPL_Events& Event = MPL_Events::Ref();
}
