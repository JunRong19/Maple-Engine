#include <pch.h>
#include <MPL_Configs.h>

namespace MPL {
	MPL_Configs::MPL_Configs() {}

	MPL_Configs::~MPL_Configs() {}

	void MPL_Configs::Load_Configs() {
		// Try load config files.
		std::ifstream config_file{ "configs.txt" };
		// If config file not found, initialize with default configs.
		if (!config_file) {
			std::cout << "Missing config.txt file. Creating new config.txt file" << std::endl;

			// LOAD DEFAULT CONFIGS
			configs["SCREEN_WIDTH"] = "1920";
			configs["SCREEN_HEIGHT"] = "1080";
			configs["LAYOUT"] = "main";

			std::ofstream ofile{ "configs.txt" };

			// Create a new config file.
			for (auto& config : configs) {
				ofile << config.first << ":" << config.second << std::endl;

			}
			ofile.close();
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
		}
		config_file.close();
	}
}
