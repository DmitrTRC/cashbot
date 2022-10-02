//
// Created by Dmitry Morozov on 2/10/22.
//

#ifndef CASHBOT_FEATURE_DB_DB_HPP
#define CASHBOT_FEATURE_DB_DB_HPP


#include "db_structure.hpp"

#include <boost/algorithm/string/join.hpp>
#include <iostream>
#include <map>
#include <string>
#include <sqlite3.h>


constexpr auto DB_PATH = "../db/cashbot.db";


class botDB {
public:
    botDB ();

    ~botDB ();

    void insertExpense (const DB::Expense &expense);

    void insertCategory (const DB::Category &category);

    void insertBudget (const DB::Budget &budget);

    void deleteExpense (const long &id);

    DB::Expenses fetchAllExpenses ();

    DB::Categories fetchCategories ();

    DB::Budgets fetchBudgets ();

private:

    sqlite3 *_bot_db;

    static sqlite3 *_openDB ();

    void _closeDB ();

    void _initDB ();

    void _check_db_exists ();


};





//using namespace sqlite_orm;


#endif //CASHBOT_FEATURE_DB_DB_HPP
