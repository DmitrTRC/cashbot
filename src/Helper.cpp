//
// Created by Dmitry Morozov on 31/10/22.
//

#include "Helper.hpp"

#include <iomanip>
#include <sstream>


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

// Format message string to make it look better
    std::stringstream command_stream;
    for (const auto &command: Helper::_bot_commands) {

        command_stream << "/" << std::left << std::setw(20) << std::string(command.first)
                       << " - " << std::internal << std::setw(30) << std::string(command.second) << "\n";
    }

    return message + command_stream.str();
}