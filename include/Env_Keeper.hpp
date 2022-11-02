//
// Created by Dmitry Morozov on 24/9/22.
//

#ifndef CASHBOT_DEV_ENV_KEEPER_HPP
#define CASHBOT_DEV_ENV_KEEPER_HPP

#include <map>
#include <nlohmann/json.hpp>
#include <set>
#include <string>

using json = nlohmann::json; /// JSON library

const std::string DEFAULT_PATH = ".config.json"; /// Default path to the config file



/**
 * @brief Class for working with the environment variables
 */
class EnvKeeper {

public:
    /**
     * @brief Constructor
     */
    EnvKeeper();

    /**
     * @brief Constructor
     * @param path Path to the config file
     */
    explicit EnvKeeper(const std::string &);

    /**
     * @brief Destructor
     */
    ~EnvKeeper();

    /**
     * @brief Get a TOKEN value from the environment
     *
     * @returns TOKEN value
     */
    [[nodiscard]] std::string get_Token() const;

    /**
     * @brief setter LastUpdateID value
     * @param id LastUpdateID value
     */
    void set_last_stop_id(int);

/**
     * @brief Get a LastUpdateID value from the environment
     * @returns LastUpdateID int value
     */
    int get_last_stop_id();


    /**
     * @brief check if the config file exists
     * @returns true if the config file exists
     */
    [[nodiscard]] bool is_Empty() const;

    //TODO: Refactor to size_t instead of long long
    /**
     * @brief Get a user ids from the environment
     *
     * @returns set of user id
     */
    [[nodiscard]]  std::set<long long> get_user_ids();


private:
    json _env; /// JSON object with the environment variables

    std::string _path; /// Path to the config file

    /**
     * @brief Read the config file
     * @returns JSON object with the environment variables
     */
    json load_environment();

    /**
     * @brief Write the config file
     *
     */
    void save_environment();

};

#endif //CASHBOT_DEV_ENV_KEEPER_HPP

