#include <pch.h>
#include <MPL_Configs.h>

void MPL_Configs::Save(std::pair<std::string, std::string> const& new_config) {
	// Overwrite current config with new config.
	configs[new_config.first] = new_config.second;

	std::ofstream ofile{ "configs.txt" };

	// Save new config to file.
	for (auto& config : configs) {
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
	for (auto& config : configs) {
		ofile << config.first << ": " << config.second << std::endl;
	}
	ofile.close();
}

void MPL_Configs::Load() {
	// Try load config files.
	std::ifstream config_file{ "configs.txt" };
	// If config file not found, initialize with default configs.
	if (!config_file) {
		std::cout << "Missing config.txt file. Creating new config.txt file" << std::endl;
		configs = Create_Default_Configs_File();
		configs_loaded = true;
	}
	else {
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

configs_fmt MPL_Configs::Create_Default_Configs_File() {
	// Get default configs.
	configs_fmt default_configs = Default_Configs();

	const char* path = "res/dock_layouts/backup-layout.txt";
	std::ofstream ofile{ "configs.txt" };
	if (!ofile) {
		std::cerr << "ERROR: Unable to write to file path: " << path << std::endl;
	}

	// Create a new config file.
	for (auto& config : default_configs) {
		ofile << config.first << ": " << config.second << std::endl;
	}
	ofile.close();
	return default_configs;
}

configs_fmt MPL_Configs::Default_Configs() {
	configs_fmt new_configs;
	new_configs["WINDOW_WIDTH"] = "1920";
	new_configs["WINDOW_HEIGHT"] = "1080";
	new_configs["LAYOUT"] = "main";
	new_configs["THEME"] = "1";

	return new_configs;
}

