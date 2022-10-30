//
// Created by Dmitry Morozov on 1/10/22.
//

#ifndef CASHBOT_DEV_HELPER_HPP
#define CASHBOT_DEV_HELPER_HPP

#include <string>
#include <initializer_list>
#include <vector>

//TODO: Make associative array for handlers

namespace Helper {
    const std::initializer_list<std::string> onHelp = {"start", "help"};

    constexpr auto helpMessage = "CashFlow Bot\n\n"
                                 "Add expenses: 25000 Medical_Service\n"
                                 "Today Statistics: /today\n"
                                 "Month Statistics: /month\n"
                                 "Last Expenses: /expenses\n"
                                 "Categories: /categories";

    const std::vector<std::pair<std::string_view, std::string_view>> _bot_commands = {
            {"start",      "Start the bot"},
            {"help",       "Get help"},
            {"today",      "Get today statistics"},
            {"month",      "Get month statistics"},
            {"expenses",   "Get last expenses"},
            {"categories", "Get categories"},
            {"stop",       "Stop the bot"}

    };
}
#endif //CASHBOT_DEV_HELPER_HPP
