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

DB::DBExpense Expense::addExpense(const std::string &message, long user) {

    Message parsedMessage = ParseMsg(message);

    DB::Category category = _categories->getCategory(parsedMessage.category_text);
    //TODO: Return time without seconds
    DB::DBExpense expense = {
            .user_id = user,
            .amount = parsedMessage.amount,
            .created = getDateToStr(time(nullptr)),
            .category_codename = category.codename,
            .raw_text = message
    };


    std::map<std::string, std::string> sqlExpense = getSQLExpense(expense);

    _db_handler->insertRow("expense", sqlExpense);

    return expense;

}


///Converts current date to string
std::string Expense::getDateToStr(time_t now_time) {

    struct tm *now_tm = localtime(&now_time);
    char date[20];
    strftime(date, 20, "%Y-%m-%d", now_tm);
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
    std::string SQL = "select sum(amount) from expense where date(created) = date('now', 'localtime');";
    auto result = _db_handler->fetchOne(SQL);

    if (result.empty()) {
        return "No expenses today";
    }

    std::string result_str = "Today expenses: \nTotal: " + result.begin()->second + " RUB" + "\n";

    SQL = "select sum(amount) "
          "from expense where date(created)=date('now', 'localtime') "
          "and category_codename in (select codename "
          "from category where is_base_expense=true)";

    result = _db_handler->fetchOne(SQL);

    std::string base_expenses;
    if (result.empty()) {
        base_expenses = "0";
    } else {
        base_expenses = result.begin()->second;
    }

    result_str += "Base expenses: " + base_expenses + " RUB" + "\n";
    result_str += "FROM BUDGET : " + std::to_string(getBudgetLimit() - std::stol(base_expenses)) + " RUB" + "\n";
    result_str += "LAST MONTH :  /month";

    return result_str;
}

std::map<std::string, std::string> Expense::getSQLExpense(const DB::DBExpense &expense) {

    std::map<std::string, std::string> result;

    result["user_id"] = std::to_string(expense.user_id);
    result["amount"] = std::to_string(expense.amount);
    result["created"] = expense.created;
    result["category_codename"] = expense.category_codename;
    result["raw_text"] = expense.raw_text;

    return result;

}

long Expense::getBudgetLimit() {

    return 0;
}

std::vector<DB::Expense> Expense::getLast() {

//    std::string SQL = "select e.id, e.amount, c.name "
//                      "from expense e left join category c "
//                      "on c.codename=e.category_codename "
//                      "order by created desc limit 10";
//    auto result = _db_handler->fetchMANY(SQL);
//    return std::vector<DB::Expense>();
    return {};
}

DB::TExpenses Expense::getAllExpenses(const int &limit) {


    std::string SQL = "select e.id, e.amount, c.name, e.created "
                      "from expense e left join category c "
                      "on c.codename=e.category_codename "
                      "order by created desc limit " + std::to_string(limit);

    auto result = _db_handler->fetchMany(SQL);
    DB::TExpenses expenses;
    for (auto row: result) {
        DB::DBExpense expense = {
                .user_id = 0, // ! FIXME: DO not filled!
                .amount = std::stol(row["amount"]),
                .created = row["created"],
                .category_codename = row["category_codename"],
                .raw_text = row["raw_text"]

        };
        expenses.push_back(expense);
    }

    return expenses;

}






