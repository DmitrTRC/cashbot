//
// Created by Dmitry Morozov on 1/10/22.
//

#include "Bot_Exceptions.hpp"
#include "Categories.hpp"
#include "Expense.hpp"

#include <regex>

Expense::Expense() {

    _db_handler = new botDB();
    _categories = new Categories(_db_handler);
};

Expense::~Expense() {

    delete _db_handler;
    delete _categories;
}

DB::Expense Expense::addExpense(const std::string &message, long user) {

    Message parsedMessage = ParseMsg(message);

    DB::Category category = _categories->getCategory(parsedMessage.category_text);

    DB::Expense expense = {
            .id = {},
            .user_id = user,
            .amount = parsedMessage.amount,
            .category = parsedMessage.category_text
    };

    std::map<std::string, std::string> sqlExpense = getSQLExpense(expense);
    _db_handler->insertRow("expense", sqlExpense);

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
            .category_text = match[2]
    };

    return resultMessage;
}


// Returns string with today expenses
std::string Expense::get_today_stat() { //TODO: implement
    std::string SQL = "select sum(amount) from expenses where date(date) = date('now');";
    auto result = _db_handler->fetchOne(SQL);

    if (result.empty()) {
        return "No expenses today";
    }

    std::string result_str = "Today expenses: \n ";
    for (auto &[key, value]: result) {
        result_str += key + ": " + value + "\n";
        std::cout << "Debug : " << key << ": " << value << std::endl;
    }

    return result_str;
}

std::map<std::string, std::string> Expense::getSQLExpense(const DB::Expense &expense) {

    std::map<std::string, std::string> result;

    result["user_id"] = std::to_string(expense.user_id);
    result["amount"] = std::to_string(expense.amount);
    result["category"] = expense.category;

    return result;

}





