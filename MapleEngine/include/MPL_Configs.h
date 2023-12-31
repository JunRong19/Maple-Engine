#pragma once

using Configs = std::unordered_map<std::string, std::string>;

static class MPL_Configs {
public:
	void Load();
	void Save(std::pair<std::string, std::string> const& new_config);
	void Save(std::vector<std::pair<std::string, std::string>> const& new_configs);
	Configs Create_Default_Configs_File();
	Configs Default_Configs();

	template <typename T>
	void Deserialize_Configs(std::string key, T value);

	inline bool const Status() { return configs_loaded; }
	inline static Configs const Get_Data() { return configs; }

private:
	inline static Configs configs;
	bool configs_loaded = false;

	static constexpr const char* CONFIGS_PATH = "configs.txt";

};

