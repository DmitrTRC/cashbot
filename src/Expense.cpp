//
// Created by Dmitry Morozov on 1/10/22.
//

#include "Bot_Exceptions.hpp"
#include "Expense.hpp"

#include <regex>

Expense::Expense() = default;

Expense::~Expense() = default;

DB::Expense Expense::addExpense(const std::string &message) {

    Message parsedMessage = ParseMsg(message);

    DB::Expense expense = {
            .id = 0,
            .user_id = 0,
            .amount = parsedMessage.amount,
            .category = parsedMessage.category
    };

    return expense;
}


///Converts current date to string
std::string Expense::getDateToStr(time_t now_time) {

    struct tm *now_tm = localtime(&now_time);
    char date[20];
    strftime(date, 20, "%Y-%m-%d %H:%M%S", now_tm);
    return {date};

}

///Parse message to get amount and category
Message Expense::ParseMsg(const std::string &message) {

    std::regex r(R"(([\d ]+) (.*))");

    std::smatch match;
    std::regex_search(message, match, r);

    if (match.size() != 3) {
        throw WrongMsgException("Wrong message format");
    }

    Message resultMessage = {
            .amount = std::stol(match[1]),
            .category = match[2]
    };

    return resultMessage;
}





