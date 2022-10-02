//
// Created by Dmitry Morozov on 2/10/22.
//

#include "db.hpp"

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>


botDB::botDB () {
    _bot_db = _openDB ();
    _check_db_exists ();
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

///Init all tables in DB
void botDB::_initDB () {
    std::cout << "Init DB" << std::endl;

    std::string sql = "CREATE TABLE expenses ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "user_id INTEGER,"
                      "amount INTEGER,"
                      "category TEXT"
                      ");";

    char *zErrMsg = nullptr;

    int rc = sqlite3_exec (_bot_db, sql.c_str (), nullptr, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free (zErrMsg);
    }
    sql = "CREATE TABLE categories ("
          "codename TEXT PRIMARY KEY,"
          "name TEXT,"
          "is_base_expense INTEGER,"
          "aliases TEXT"
          ");";
    rc = sqlite3_exec (_bot_db, sql.c_str (), nullptr, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free (zErrMsg);
    }
    sql = "CREATE TABLE budgets ("
          "codename TEXT PRIMARY KEY,"
          "daily_limit INTEGER"
          ");";
    rc = sqlite3_exec (_bot_db, sql.c_str (), nullptr, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free (zErrMsg);
    }

    std::cout << "Init DB done" << std::endl;

}

void botDB::deleteRow (std::string &table, const long &id) {
    std::string sql = "DELETE FROM " + table + " WHERE id = " + std::to_string (id);
    char *zErrMsg = nullptr;
    int rc = sqlite3_exec (_bot_db, sql.c_str (), nullptr, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free (zErrMsg);
    }

}

//TODO: Refactor "Values" to "Data"
void botDB::insertRow (const std::string &table, const std::map<std::string, std::string> &values) {
    std::string sql = "INSERT INTO " + table + " (";
    for (auto &it: values) {
        sql += it.first + ", ";
    }
    sql.pop_back ();
    sql.pop_back ();
    sql += ") VALUES (";
    for (auto &it: values) {
        sql += it.second + ", ";
    }
    sql.pop_back ();
    sql.pop_back ();
    sql += ");";
    char *zErrMsg = nullptr;
    int rc = sqlite3_exec (_bot_db, sql.c_str (), nullptr, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free (zErrMsg);
    }

}

