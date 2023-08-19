#pragma once

namespace MPL {
	class MPL_Configs {

	public:
		static MPL_Configs& Ref() {
			static MPL_Configs ref;
			return ref;
		}

		void Load_Configs();

	private:
		MPL_Configs(MPL_Configs const&) = delete;
		MPL_Configs operator=(MPL_Configs const&) = delete;
		MPL_Configs();
		~MPL_Configs();

	};
	static MPL_Configs& Config = MPL_Configs::Ref();

	inline static std::unordered_map<std::string, std::string> configs;
}
