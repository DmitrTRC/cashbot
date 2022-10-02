//
// Created by Dmitry Morozov on 2/10/22.
//

#include "db.hpp"

#include <boost/algorithm/string/join.hpp>
#include <iostream>
#include <map>
#include <vector>


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

