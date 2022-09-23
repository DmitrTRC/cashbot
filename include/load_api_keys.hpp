//
// Created by Dmitry Morozov on 23/9/22.
//

#ifndef ECHOBOT_DEV_LOAD_API_KEYS_HPP
#define ECHOBOT_DEV_LOAD_API_KEYS_HPP

#include <string>
#include <map>


using Tokens = std::map<std::string, std::string>;

const Tokens load_environment (const std::string & = ".env");


#endif //ECHOBOT_DEV_LOAD_API_KEYS_HPP

