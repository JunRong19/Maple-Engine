#include <pch.h>
#include <MPL_Configs.h>

void MPL_Configs::Set_Data(std::pair<std::string, std::string> const& config) {
	configs[config.first] = config.second;
	Save(config);
}

void MPL_Configs::Save(std::pair<std::string, std::string> const& config) {
	Create_Default_Configs_File(config);
}

void MPL_Configs::Load() {
	// Try load config files.
	std::ifstream config_file{ "configs.txt" };
	// If config file not found, initialize with default configs.
	if (!config_file) {
		std::cout << "Missing config.txt file. Creating new config.txt file" << std::endl;
		configs = Create_Default_Configs_File();
		configs_loaded = true;
		config_file.close();
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
	// GET DEFAULT CONFIGS
	configs_fmt config_cpy = Default_Configs();

	std::ofstream ofile{ "configs.txt" };

	// Create a new config file.
	for (auto& config : config_cpy) {
		ofile << config.first << ": " << config.second << std::endl;
	}
	ofile.close();
	return config_cpy;
}

configs_fmt MPL_Configs::Create_Default_Configs_File(std::pair<std::string, std::string> new_config) {
	// GET DEFAULT CONFIGS
	configs_fmt config_cpy = Default_Configs();

	// Overwrite default config with new config.
	config_cpy[new_config.first] = new_config.second;

	std::ofstream ofile{ "configs.txt" };

	// Create a new config file.
	for (auto& config : config_cpy) {
		ofile << config.first << ": " << config.second << std::endl;
	}
	ofile.close();
	return config_cpy;
}

configs_fmt MPL_Configs::Create_Default_Configs_File(std::vector<std::pair<std::string, std::string>> new_configs) {
	// GET DEFAULT CONFIGS
	configs_fmt config_cpy = Default_Configs();

	// Overwrite default config with new changes.
	for (auto& config : new_configs) {
		config_cpy[config.first] = config.second;
	}

	std::ofstream ofile{ "configs.txt" };

	// Create a new config file.
	for (auto& config : config_cpy) {
		ofile << config.first << ": " << config.second << std::endl;
	}
	ofile.close();
	return config_cpy;
}

configs_fmt MPL_Configs::Default_Configs() {
	configs_fmt new_configs;
	new_configs["WINDOW_WIDTH"] = "1920";
	new_configs["WINDOW_HEIGHT"] = "1080";
	new_configs["LAYOUT"] = "main";

	return new_configs;
}

