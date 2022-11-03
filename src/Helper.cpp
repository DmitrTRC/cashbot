//
// Created by Dmitry Morozov on 31/10/22.
//

#include "Helper.hpp"

#include <tgbot/tgbot.h>

#include <iomanip>
#include <sstream>


/**
 * @brief  It compose a string that contains the help message.
 *
 * @returns std::string that contains the help message.
 */
std::string Helper::helpMessage() {




    ///Greetings message
    auto botEmoji = "🏺";

    std::string message = "Hello. I'm a bot that helps you to track your expenses.  \n"
                          "Here is the list of commands that I can understand:\n\n"
                          "<i>For adding an expense:</i>\n"
                          "<b>Example</b> : 2000 food\n\n"
                          "The first number is the amount of money you spent, the second is the category\n\n";



// Format message string to make it look better
    std::stringstream command_stream;
    for (const auto &command: Helper::_bot_commands) {

        command_stream << "/" << std::left << std::setw(20) << std::string(command.first)
                       << " - " << std::internal << std::setw(30) << std::string(command.second) << "\n";
    }

    return botEmoji + message + command_stream.str();
}

/**
 * It returns a sticker object with the emoji "🤖"
 *
 * @param sticker_id The ID of the sticker to be sent.
 *
 * @returns A pointer to a TgBot::Sticker object.
 */
TgBot::Sticker::Ptr botSticker(const std::string &sticker_id) {

    TgBot::Sticker::Ptr sticker(new TgBot::Sticker);
    sticker->emoji = "🤖";
    return sticker;
}
