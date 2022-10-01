//
// Created by Dmitry Morozov on 24/9/22.
//

#ifndef ECHOBOT_DEV_ENV_KEEPER_HPP
#define ECHOBOT_DEV_ENV_KEEPER_HPP

#include <string>
#include <map>
#include <nlohmann/json.hpp>
#include <set>


using json = nlohmann::json;

const std::string DEFAULT_PATH = ".config.json";


class EnvKeeper {
public:
    EnvKeeper ();

    explicit EnvKeeper (const std::string &);

    ~EnvKeeper ();

    [[nodiscard]] std::string get_Token () const;

    void set_last_stop_id (int);

    int get_last_stop_id ();

    [[nodiscard]] bool is_Empty () const;

    //TODO: Refactor to size_t instead of long long
    [[nodiscard]] std::set<long long> get_user_ids () const;


private:
    json _env;

    std::string _path;

    json load_environment ();

    void save_environment ();

};


#endif //ECHOBOT_DEV_ENV_KEEPER_HPP

