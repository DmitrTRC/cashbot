//
// Created by Dmitry Morozov on 2/10/22.
//

#ifndef CASHBOT_FEATURE_EXPENSE_DB_STRUCTURE_HPP
#define CASHBOT_FEATURE_EXPENSE_DB_STRUCTURE_HPP

#include <vector>
#include <string>


namespace DB {

//New DB record with Expense
    struct DBExpense {
        long id;
        long user_id;
        long amount;
        std::string created;
        std::string category_codename;
        std::string raw_text;
    };


    using TExpenses = std::vector<DBExpense>;

    struct Category {
        std::string codename;
        std::string name;
        bool is_base_expense;
        std::vector<std::string> aliases;
    };

    using TCategories = std::vector<Category>;

    struct Budget {
        std::string codename;
        long daily_limit;
    };

    using TBudgets = std::vector<Budget>;

}


#endif //CASHBOT_FEATURE_EXPENSE_DB_STRUCTURE_HPP
