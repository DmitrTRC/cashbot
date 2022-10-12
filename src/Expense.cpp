//
// Created by Dmitry Morozov on 1/10/22.
//

#include "Expense.hpp"

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
    strftime(date, 20, "%Y-%m-%d", now_tm);
    return {date};

}

Message Expense::ParseMsg(const std::string &message) {

    return Message();
}




