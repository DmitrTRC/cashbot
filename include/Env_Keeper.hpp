//
// Created by Dmitry Morozov on 24/9/22.
//

#ifndef ECHOBOT_DEV_ENV_KEEPER_HPP
#define ECHOBOT_DEV_ENV_KEEPER_HPP

#include <string>
#include <map>
#include <nlohmann/json.hpp>


using json = nlohmann::json;


class EnvKeeper {
public:
    explicit EnvKeeper (const std::string & = ".config.json");

    ~EnvKeeper ();

    [[nodiscard]] std::string get_Token () const;

    [[nodiscard]] bool is_Empty () const;

    bool is_valid_user_id (const std::string &);

private:
    json _env;

    static json load_environment (const std::string & = ".config.json");

};


#endif //ECHOBOT_DEV_ENV_KEEPER_HPP
