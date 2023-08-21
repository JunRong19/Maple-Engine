#pragma once

using configs_fmt = std::unordered_map<std::string, std::string>;

static class MPL_Configs {
public:
	void Load();
	void Save(std::pair<std::string, std::string> const& new_config);
	void Save(std::vector<std::pair<std::string, std::string>> const& new_configs);
	configs_fmt Create_Default_Configs_File();
	configs_fmt Default_Configs();

	inline bool const Status() { return configs_loaded; }
	inline static configs_fmt const Get_Data() { return configs; }

private:
	inline static configs_fmt configs;
	bool configs_loaded = false;
};

