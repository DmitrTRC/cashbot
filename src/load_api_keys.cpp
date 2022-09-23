//
// Created by Dmitry Morozov on 23/9/22.
//

#include "load_api_keys.hpp"


#include <fstream>
#include <iostream>
#include <map>
#include <sstream>


/// Read API keys from env file
const Tokens load_environment (const std::string &file_name) {
    std::map<std::string, std::string> env;

    std::ifstream env_file;



    try {

        env_file.open (file_name);
        if (env_file.fail()) {
            throw std::runtime_error ("Failed to open file");
        }


        std::string line;

        while (std::getline (env_file, line)) {
            if (line.empty () or line.starts_with ("#")) {
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
    } catch (const std::runtime_error &e) {
        std::cerr << "Exception opening/reading file: " << e.what () << std::endl;
    }

    env_file.close ();

    return env;
}