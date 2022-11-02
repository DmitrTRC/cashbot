//
// Created by Dmitry Morozov on 1/10/22.
//

#ifndef CASHBOT_DEV_EXPENSE_HPP
#define CASHBOT_DEV_EXPENSE_HPP

#include "Categories.hpp"
#include "db.hpp"
#include "db_structure.hpp"

#include <cstdlib>
#include <string>

///<Parsed Expense message
struct Message {
    long amount;
    std::string category_text;
};

/**
 * @brief Class for expense management
 */
class Expense {

public:
    /**
     * @brief Constructor
     */
    explicit Expense();

    /**
     * @brief Destructor
     */
    ~Expense();

    /**
     * @brief Convert DB::DBExpense to Expense of SQL format
     *
     * @param expense DB::DBExpense object
     *
     * @return A Map of SQL format values
     */
    static std::map<std::string, std::string> getSQLExpense(const DB::DBExpense &);


    /**
     * @brief Add new expense to DB
     *
     * @param message  to parse
     * @param id user id
     *
     * @return DB::DBExpense object
     */
    DB::DBExpense addExpense(const std::string &message, long);

    /**
     * @brief Get TODAY expenses statistics
     * @return string with expenses
     */
    std::string get_today_stat();

    /**
     * @brief Get MONTH expenses statistics
     * @return string with expenses
     */
    std::string get_month_stat();

    /**
     * @brief Get LAST expenses statistics
     * @return string with expenses
     */
    DB::TExpenses getLast();

    /**
     * @brief Get ALL expenses statistics
     * @return string with expenses
     */
    DB::TExpenses getAllExpenses(const int &limit = 100); //TODO: Add possibility to get all expenses

    /**
     * @brief Delete expense by id
     * @param id expense id
     */
    void deleteExpense(const long &id);

    /**
     * @brief getter for Categories object pointer
     * @return  Categories object pointer
     */
    Categories *getCategoriesPtr();

private:
    botDB *_db_handler; ///< DB handler

    Categories *_categories; ///< Categories object pointer

    /**
     * @brief Parse message to get amount and category
     * @param message to parse
     * @return Parsed message
     */
    static Message ParseMsg(const std::string &message);

    /**
     * @brief Converts current date to string
     * @param now_time current time
     * @return string with date
     */
    static std::string getDateToStr(time_t);

    /**
     * @brief getter for Budget limit
     * @return  Budget limit
     */
    long getBudgetLimit();

};

#endif //CASHBOT_DEV_EXPENSE_HPP
