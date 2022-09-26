//
// Created by Dmitry Morozov on 24/9/22.
//

#include "Env_Keeper.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>


/**
 * It loads the environment
 *
 * @param path The path to the environment file.
 */
EnvKeeper::EnvKeeper (const std::string &path) {
    _path = path;
    _env = load_environment ();
}


/**
 * It reads a JSON file and returns a JSON object
 *
 * @param json_file_name The name of the JSON file that contains the environment parameters.
 *
 * @return A json object
 */
json EnvKeeper::load_environment () {


    std::ifstream json_file (_path);

    json env = json::parse (json_file);


    return env;
}

///Save environment
void EnvKeeper::save_environment () {
    std::ofstream json_file (_path);
    json_file << _env.dump (4);

}


/**
 * Checks if the user id is valid
 *
 * @param user_id The user id to check
 *
 * @return True if the user id is valid, false otherwise
 */
bool EnvKeeper::is_valid_user_id (const std::string &user_id) {

    return _env["USER_ID"].contains (user_id);
}

/**
 * Returns true if the environment is empty, false otherwise.
 *
 * @return A boolean value.
 */
bool EnvKeeper::is_Empty () const {

    return _env.empty ();
}

std::string EnvKeeper::get_Token () const {
    std::string token;
    try {
        token = _env["TELEGRAM_API_TOKEN"].get<std::string> ();
    } catch (std::exception &e) {
        std::cout << e.what () << "Invalid Token INFO" << std::endl;

    }

    return token;
}

void EnvKeeper::set_last_stop_id (int id) {
    _env["LAST_STOP_ID"] = id;
    save_environment ();
}

int EnvKeeper::get_last_stop_id () {
    return _env["LAST_STOP_ID"].get<int> ();
}


/**
 * The destructor for the EnvKeeper class.
 */
EnvKeeper::~EnvKeeper () = default;
