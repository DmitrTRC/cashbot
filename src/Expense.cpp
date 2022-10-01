//
// Created by Dmitry Morozov on 1/10/22.
//

#include "Expense.hpp"


namespace Expense {

    Expense addExpense (const std::string &message) {
        Message parsed_message = _ParseMessage (message);
        Expense expense = {
                .id = 0,
                .user_id = 0,
                .amount = parsed_message.amount,
                .category = parsed_message.category
        };

        return expense;
    }

    std::string get_today_stat () {
        return "Today stat";
    }

    std::string get_month_stat () {
        return "Month stat";
    }


    Message _ParseMessage (const std::string &message) {
        Message parsed_message;
        parsed_message.amount = 0;
        parsed_message.category = "Category";
        return parsed_message;
    }

    ///Converts current date to string
    std::string _get_Date_To_String (time_t now_time) {
        struct tm *now_tm = localtime (&now_time);
        char date[20];
        strftime (date, 20, "%Y-%m-%d", now_tm);
        return std::string (date);

    }


    long getBudgetLimit () {
        return 0;
    }

} // Expense