//
// Created by Dmitry Morozov on 23/9/22.
//

#include "load_api_keys.hpp"

#include <fstream>
#include <map>
#include <sstream>


/// Read API keys from env file
std::map<std::string, std::string> load_environment (const std::string &file_name = "bot.env") {
    std::map<std::string, std::string> env;
    std::ifstream env_file (file_name);

    if (env_file.is_open ()) {
        std::string line;

        while (std::getline (env_file, line)) {
            if (line.empty ()) {
                continue;
            }
            std::istringstream line_stream (line);
            std::string key;
            if (std::getline (line_stream, key, '=')) {
                std::string value;
                if (std::getline (line_stream, value)) {
                    env[key] = value;
                }
            }
        }
    }

    return env;
}