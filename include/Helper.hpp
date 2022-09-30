//
// Created by Dmitry Morozov on 1/10/22.
//

#ifndef ECHOBOT_DEV_HELPER_HPP
#define ECHOBOT_DEV_HELPER_HPP

#include <string>
#include <initializer_list>


namespace Helper {
    auto onHelp = std::initializer_list<std::string>{"start", "help"};

    constexpr auto helpMessage = "CashFlow Bot\n\n"
                                 "Add expenses: 25000 Medical_Service\n"
                                 "Today Statistics: /today\n"
                                 "Month Statistics: /month\n"
                                 "Last Expenses: /expenses\n"
                                 "Categories: /categories";
}
#endif //ECHOBOT_DEV_HELPER_HPP
