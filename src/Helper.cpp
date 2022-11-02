//
// Created by Dmitry Morozov on 31/10/22.
//

#include "Helper.hpp"


/**
 * @brief  It compose a string that contains the help message.
 *
 * @returns std::string that contains the help message.
 */
std::string Helper::helpMessage() {
    ///Greetings message
    std::string message = "Hello! I'm a bot that helps you to track your expenses. \n"
                          "Here is the list of commands that I can understand:\n\n"
                          "For adding an expense:\n"
                          "Example : 2000 food\n"
                          "The first number is the amount of money you spent, the second is the category\n\n";


    for (const auto &command: Helper::_bot_commands) {
        message += "/" + std::string(command.first) + " - " + std::string(command.second) + "\n";
    }

    return message;


}