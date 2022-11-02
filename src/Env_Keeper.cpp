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
EnvKeeper::EnvKeeper(const std::string &path) {

    _path = path; // Save the path to the environment file.
    _env = load_environment(); // Load the environment.

}


/**
 * It reads a JSON file and returns a JSON object
 *
 * @param json_file_name The name of the JSON file that contains the environment parameters.
 *
 * @return A json object
 */
json EnvKeeper::load_environment() {

    std::ifstream json_file(_path);

    json env = json::parse(json_file);

    return env;
}


/**
 * It opens a file, writes the environment to it, and closes the file
 */
void EnvKeeper::save_environment() {

    std::ofstream json_file(_path);

    json_file << _env.dump(4);

}


/**
 * Returns true if the environment is empty, false otherwise.
 *
 * @return A boolean value.
 */
bool EnvKeeper::is_Empty() const {

    return _env.empty();
}

/**
 * It gets the token from the environment variable.
 *
 * @return A TOKEN string
 */
std::string EnvKeeper::get_Token() const {

    std::string token;
    try {
        token = _env["TELEGRAM_API_TOKEN"].get<std::string>();
    } catch (std::exception &e) {
        std::cout << e.what() << "Invalid Token INFO" << std::endl;

    }

    return token;
}

/**
 * It sets the value of the environment variable LAST_STOP_ID to the value of the parameter id
 *
 * @brief setter for the LAST_STOP_ID
 * @param id The id of the stop
 */
void EnvKeeper::set_last_stop_id(int id) {

    _env["LAST_STOP_ID"] = id;

    save_environment();

}

/**
 * This function returns the last stop id from the environment
 *
 * @brief getter for the LAST_STOP_ID
 * @return The last stop id.
 */
int EnvKeeper::get_last_stop_id() {

    return _env["LAST_STOP_ID"].get<int>();

}

/**
 * EnvKeeper::EnvKeeper() : EnvKeeper(DEFAULT_PATH) {}
 *
 * The above function is a constructor for the EnvKeeper class. It takes no arguments and calls the EnvKeeper constructor
 * that takes a string argument. The string argument is the default path
 */
EnvKeeper::EnvKeeper() : EnvKeeper(DEFAULT_PATH) {}

/// Return  a set of user ids from the environment
/**
 * It returns a set of user ids
 *
 * @return A set of user ids.
 */
std::set<long long> EnvKeeper::get_user_ids() {

    std::set<long long> user_ids;

    auto ids = _env["USER_ID"].get<std::vector<long long>>();

    for (auto id: ids) {
        user_ids.insert(id);
    }

    return user_ids;
}


/**
 * The destructor for the EnvKeeper class.
 */
EnvKeeper::~EnvKeeper() = default;
