#include <pch.h>
#include <MPL_Configs.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>


void MPL_Configs::Save(std::pair<std::string, std::string> const& new_config) {
	// Overwrite current config with new config.
	configs[new_config.first] = new_config.second;

	std::ofstream ofile{ CONFIGS_PATH };

	// Save new config to file.
	for (auto const& config : configs) {
		ofile << config.first << ": " << config.second << std::endl;
	}
	ofile.close();
}

void MPL_Configs::Save(std::vector<std::pair<std::string, std::string>> const& new_configs) {
	// Overwrite current config with new changes.
	for (auto& config : new_configs) {
		configs[config.first] = config.second;
	}

	std::ofstream ofile{ "configs.txt" };

	// Save new config to file.
	for (auto const& config : configs) {
		ofile << config.first << ": " << config.second << std::endl;
	}
	ofile.close();
}

template <typename T>
void Deserialize_Configs(std::string key, T value) {


}

void MPL_Configs::Load() {
	// Try load config files.
	std::ifstream config_file{ CONFIGS_PATH };
	// If config file not found, initialize with default configs.
	if (!config_file) {
		std::cout << "Missing config.txt file. Creating new config.txt file" << std::endl;
		configs = Create_Default_Configs_File();
		configs_loaded = true;
	}
	else {
		//rapidjson::Document d;
		//rapidjson::ParseResult ok = d.Parse(CONFIGS_PATH);
		//if (!ok) {
		//	std::cerr << "ERROR: Unable to load configs." << std::endl;
		//	std::cerr << "JSON parse error: " << rapidjson::GetParseError_En(ok.Code()) << std::endl;
		//}

		//for (rapidjson::Value::ConstValueIterator itr = d.Begin(); itr != d.End(); ++itr) {

		//}

		// Load configs.
		std::string line;
		while (getline(config_file, line)) {
			std::stringstream ss{ line };
			std::string config_name;
			std::string config_value;

			ss >> config_name >> config_value;
			// Remove the colon.
			config_name.erase(config_name.end() - 1);

			configs[config_name] = config_value;
		}
		configs_loaded = true;
	}
	config_file.close();
}

Configs MPL_Configs::Create_Default_Configs_File() {
	// Get default configs.
	Configs default_configs = Default_Configs();

	std::ofstream ofile{ CONFIGS_PATH };
	if (!ofile) {
		std::cerr << "ERROR: Unable to write to file path: " << CONFIGS_PATH << std::endl;
	}

	// Create a new config file.
	for (auto& config : default_configs) {
		ofile << config.first << ": " << config.second << std::endl;
	}
	ofile.close();
	return default_configs;
}

Configs MPL_Configs::Default_Configs() {
	Configs new_configs;
	new_configs["WINDOW_WIDTH"] = "1920";
	new_configs["WINDOW_HEIGHT"] = "1080";
	new_configs["LAYOUT"] = "main";
	new_configs["THEME"] = "2";		// Light theme.

	return new_configs;
}

