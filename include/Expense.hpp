//
// Created by Dmitry Morozov on 1/10/22.
//

#ifndef CASHBOT_DEV_EXPENSE_HPP
#define CASHBOT_DEV_EXPENSE_HPP

#include <cstdlib>
#include <string>

#include "db_structure.hpp"
#include "Categories.hpp"
#include "db.hpp"

///Parsed Expense message
struct Message {
    long amount;
    std::string category_text;
};

class Expense {
public:
    explicit Expense();

    ~Expense();

    //DB::Expense getExpense();
    static std::map<std::string, std::string> getSQLExpense(const DB::DBExpense &);

    DB::DBExpense addExpense(const std::string &message, long);

    std::string get_today_stat();

    std::string get_month_stat();

    DB::TExpenses getLast();

    DB::TExpenses getAllExpenses(const int &limit = 100); //TODO: Add possibility to get all expenses

    void deleteExpense(const long &id);

    Categories *getCategoriesPtr();

private:
    botDB *_db_handler;

    Categories *_categories;

    static Message ParseMsg(const std::string &message);

    static std::string getDateToStr(time_t);

    long getBudgetLimit();

}; // Expense

#endif //CASHBOT_DEV_EXPENSE_HPP
