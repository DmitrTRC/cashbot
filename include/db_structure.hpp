//
// Created by Dmitry Morozov on 2/10/22.
//

#ifndef CASHBOT_FEATURE_EXPENSE_DB_STRUCTURE_HPP
#define CASHBOT_FEATURE_EXPENSE_DB_STRUCTURE_HPP

#include <string>
#include <vector>

namespace DB { ///< The namespace for the database

    /**
     * The structure for the expense table
     */
    struct DBExpense {
        long id; ///< The ID of the expense
        long user_id; ///< The ID of the user who created the expense
        long amount; ///< The amount of the expense
        std::string created; ///< The date when the expense was created
        std::string category_codename; ///< The codename of the category
        std::string raw_text; ///< The raw text of the expense
    };


    using TExpenses = std::vector<DBExpense>; ///< The type for the vector of expenses

    /**
     * The structure for the category table
     */
    struct Category {
        std::string codename; ///< The codename of the category
        std::string name; ///< The name of the category
        bool is_base_expense; ///< Whether the category is a base expense
        std::vector<std::string> aliases; ///< The aliases of the category
    };

    using TCategories = std::vector<Category>; ///< The type for the categories vector

    /**
     * The structure for the Budget table
     */
    struct Budget {
        std::string codename; ///< The codename of the budget
        long daily_limit; ///< The daily limit of the budget
    };

    using TBudgets = std::vector<Budget>; ///< The type for the budgets vector

}

#endif //CASHBOT_FEATURE_EXPENSE_DB_STRUCTURE_HPP
