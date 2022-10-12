//
// Created by Dmitry Morozov on 1/10/22.
//

#ifndef CASHBOT_DEV_EXPENSE_HPP
#define CASHBOT_DEV_EXPENSE_HPP

#include <cstdlib>
#include <string>

#include "db_structure.hpp"

class Expense {
public:
    Expense(long user_id, long amount, std::string category);

    ~Expense();

    DB::Expense getExpense();

    DB::Expense addExpense(const std::string &message);

    std::string get_today_stat();

    std::string get_month_stat();

    std::vector<DB::Expense> getLast();

    void deleteExpense(const long &id);

private:

    ///Parsed Expense message
    struct Message {
        long amount;
        std::string category;
    };


    Message ParseMsg(const std::string &message);

    std::string _get_date_to_string();

    long getBudgetLimit();

}; // Expense

#endif //CASHBOT_DEV_EXPENSE_HPP
