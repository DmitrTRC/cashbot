//
// Created by Dmitry Morozov on 2/10/22.
//

#include "db.hpp"

#include <boost/algorithm/string/join.hpp>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>


botDB::botDB () {
    _bot_db = _openDB ();
}

botDB::~botDB () {
    _closeDB ();
}

sqlite3 *botDB::_openDB () {
    sqlite3 *db;
    int rc = sqlite3_open (DB_PATH, &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg (db) << std::endl;
        sqlite3_close (db);
        exit (1);
    }
    return db;
}


void botDB::_closeDB () {
    sqlite3_close (_bot_db);
}

DB::Expenses botDB::fetchAllExpenses () {
    DB::Expenses expenses;
    std::string sql = "SELECT * FROM expenses";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2 (_bot_db, sql.c_str (), -1, &stmt, nullptr);
    while (sqlite3_step (stmt) == SQLITE_ROW) {
        DB::Expense expense;
        expense.id = sqlite3_column_int (stmt, 0);
        expense.user_id = sqlite3_column_int (stmt, 1);
        expense.amount = sqlite3_column_int (stmt, 2);
        expense.category = std::string ((char *) sqlite3_column_text (stmt, 3));
        expenses.push_back (expense);
    }
    sqlite3_finalize (stmt);
    return expenses;

}

DB::Categories botDB::fetchCategories () {
    DB::Categories categories;
    std::string sql = "SELECT * FROM categories";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2 (_bot_db, sql.c_str (), -1, &stmt, nullptr);
    while (sqlite3_step (stmt) == SQLITE_ROW) {
        DB::Category category;
        category.codename = std::string ((char *) sqlite3_column_text (stmt, 0));
        category.name = std::string ((char *) sqlite3_column_text (stmt, 1));
        category.is_base_expense = sqlite3_column_int (stmt, 2);
        std::string aliases = std::string ((char *) sqlite3_column_text (stmt, 3));
        category.aliases = std::vector<std::string> ();
        boost::split (category.aliases, aliases, boost::is_any_of (","));
        categories.push_back (category);
    }
    sqlite3_finalize (stmt);
    return categories;

}

DB::Budgets botDB::fetchBudgets () {
    DB::Budgets budgets;
    std::string sql = "SELECT * FROM budgets";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2 (_bot_db, sql.c_str (), -1, &stmt, nullptr);
    while (sqlite3_step (stmt) == SQLITE_ROW) {
        DB::Budget budget;
        budget.codename = std::string ((char *) sqlite3_column_text (stmt, 0));
        budget.daily_limit = sqlite3_column_int (stmt, 1);
        budgets.push_back (budget);
    }
    sqlite3_finalize (stmt);
    return budgets;

}

///Checks DB structure by execute query to DB and call _initDB if DB is empty
void botDB::_check_db_exists () {
    std::string sql = "SELECT * FROM expenses";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2 (_bot_db, sql.c_str (), -1, &stmt, nullptr);
    if (sqlite3_step (stmt) != SQLITE_ROW) {
        std::cout << "DB is empty, creating tables" << std::endl;
        _initDB ();
    }
    sqlite3_finalize (stmt);

}

///Run the script DB_PATH/createdb.sql
void botDB::_initDB () {
    std::string path = DB_PATH + std::string ("/createdb.sql");
    auto qslInitScript = std::ifstream (path);

    sqlite3_exec (_bot_db, qslInitScript.str ().c_str (), nullptr, nullptr, nullptr);

}

