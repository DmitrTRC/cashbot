//
// Created by Dmitry Morozov on 2/10/22.
//

#include "db.hpp"

#include <filesystem>
#include <iostream>
#include <map>
#include <vector>

/**
 * @brief Constructor
 * @param dbPath Path to the database
 */
botDB::botDB(const std::string &db_path) {

    _bot_db_path = db_path;
    _bot_db = _openDB();
    if (!_check_db_exists()) {
        std::cout << "DB not found, creating new one" << std::endl;
        _initDB();

    }

}

/**
 * @brief Destructor
 */
botDB::~botDB() {

    _closeDB();
}

/**
 * @brief Open the database
 * @returns Pointer to the database
 */
sqlite3 *botDB::_openDB() {

    sqlite3 *db;
    int rc = sqlite3_open(_bot_db_path.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        exit(1);
    }
    return db;
}


/**
 * @brief Close the database
 */
void botDB::_closeDB() {

    sqlite3_close(_bot_db);
}


/**
 * @brief Checks DB structure by execute query to DB and call _initDB if DB is empty
 * @returns True if DB is not empty
 */
bool botDB::_check_db_exists() {

    std::string sql("SELECT name FROM sqlite_master WHERE type='table' AND name='expense';");

    char *zErrMsg = nullptr;

    static auto callback = +[](void *inst, int argc, char **argv, char **azColName) -> int {
        for (int i = 0; i < argc; i++) {

            std::cout << i << " :  " << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
        }
        std::cout << "CHECK TABLE : DONE " << std::endl;
        static_cast<botDB *> ( inst )->_call_state = true; // Set operation result flag
        return 0;
    };
    _call_state = false;
    int rc = sqlite3_exec(_bot_db, sql.c_str(),
                          callback, this, &zErrMsg);

    if (rc != SQLITE_OK) {  // If error
        std::cerr << "SQL error from _check_DB_Exists : " << zErrMsg << std::endl;
        std::cout << "DB is empty, creating tables" << std::endl;

        sqlite3_free(zErrMsg); // Free memory
        setCallState(false); // Set operation result flag

    }

    return getCallState();
}


/**
 * @brief Init the database
 */
void botDB::_initDB() {

    std::cout << "Init DB" << std::endl;

    std::string sql = "create table budget("
                      "codename varchar(255) primary key,"
                      "  daily_limit integer );";

    char *zErrMsg = nullptr;

    int rc = sqlite3_exec(_bot_db, sql.c_str(), nullptr, nullptr, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

    sql = "create table category("
          "    codename varchar(255) primary key, "
          "    name varchar(255), "
          "    is_base_expense boolean, "
          "    aliases text "
          ");";
    rc = sqlite3_exec(_bot_db, sql.c_str(), nullptr, nullptr, &zErrMsg);

    if (rc != SQLITE_OK) { // If error
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg); // Free memory
    }

    sql = "create table expense("
          "    id integer primary key, "
          "    user_id integer default 641480282, "
          "    amount integer, "
          "    created datetime, "
          "    category_codename integer, "
          "    raw_text text, "
          "    FOREIGN KEY(category_codename) REFERENCES category(codename) "
          ");";

    rc = sqlite3_exec(_bot_db, sql.c_str(), nullptr, nullptr, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error from _InitDB : " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

    fill_base_categories();

    std::cout << "Init DB done" << std::endl;
}


/**
 * Delete a row from the database
 *
 * @param table The name of the table you want to delete from.
 * @param id The id of the row to delete.
 */
void botDB::deleteRow(const std::string &table, const long &id) {

    std::string sql = "DELETE FROM " + table + " WHERE id = " + std::to_string(id);
    char *zErrMsg = nullptr;
    int rc = sqlite3_exec(_bot_db, sql.c_str(), nullptr, nullptr, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error from deleteRow : " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

}


/**
 * @brief It takes a table name and a map of column names and values, and inserts a row into the table with the values
 *
 * @param table The name of the table to insert into.
 * @param values a map of column names and values to insert into the table.
 */
void botDB::insertRow(const std::string &table, const std::map<std::string, std::string> &values) {

    std::string sql = "INSERT INTO " + table + " ("; // Start of the SQL statement
    for (auto &it: values) {
        sql += it.first + ", ";
    }
    sql.pop_back(); // Remove the last comma
    sql.pop_back(); // Remove the last space
    sql += ") VALUES ('";

    for (auto &it: values) {
        sql += it.second + "','";
    }

    sql.pop_back(); // Remove last comma
    sql.pop_back(); // Remove last space
    sql += ");";
    char *zErrMsg = nullptr;

    int rc = sqlite3_exec(_bot_db, sql.c_str(), nullptr, nullptr, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error from InsertRow: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

}

/**
 * It takes a table name and a vector of column names, and returns a vector of maps, where each map is a row of the table,
 * and the keys of the map are the column names
 *
 * @param table The table to fetch from
 * @param columns a vector of strings that represent the columns you want to fetch.
 *
 * @return A vector of maps.
 */
std::vector<std::map<std::string, std::string >>
botDB::fetchAll(const std::string &table, const std::vector<std::string> &columns) {

    std::string sql = "SELECT ";

    for (auto &it: columns) {
        sql += it + ", ";
    }

    sql.pop_back(); // Remove the last comma
    sql.pop_back(); // Remove the last space

    sql += " FROM " + table + ";";
    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(_bot_db, sql.c_str(), -1, &stmt, nullptr);

    std::vector<std::map<std::string, std::string>> result;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::map<std::string, std::string> row;

        for (int i = 0; i < columns.size(); i++) {
            row[columns[i]] = std::string((char *) sqlite3_column_text(stmt, i));
        }

        result.push_back(row);
    }

    sqlite3_finalize(stmt);

    return result;
}

/**
 * @brief getter for the call state
 *
 * @return The value of the private variable _call_state. true if the last operation was successful, false otherwise.
 */
bool botDB::getCallState() const {

    return _call_state;
}

/**
 * This function sets the call state of the bot.
 *
 * @brief setter for the call state
 * @param callState This is a boolean value that indicates last operation result.
 */
void botDB::setCallState(bool callState) {

    _call_state = callState;
}

/**
 * It fills the database with test data
 */
void botDB::fill_test_tables() {

    std::string sql = "insert into category (codename, name, is_base_expense, aliases)  values \n"
                      "('products', 'продукты', true, 'еда'), \n "
                      "('coffee', 'кофе', true, ''), \n"
                      "('dinner', 'обед', true, 'столовая, ланч, бизнес-ланч, бизнес ланч'); "

                      "insert into budget (codename, daily_limit) values \n"
                      "('default', 1000);"

                      "insert into expense (amount, user_id, created,  category_codename, raw_text) values \n"
                      "('100', '123456789', '2020-01-01',  'products', 'продукты'), \n"
                      "('200', '234567891', '2020-01-01',  'coffee', 'кофе'), \n"
                      "('300', '', '2022-03-06', 'dinner', 'обед');";

    char *zErrMsg = nullptr;

    int rc = sqlite3_exec(_bot_db, sql.c_str(), nullptr, nullptr, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error from  fill_test_db : " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

}

/**
 * It deletes the database file
 */
void botDB::deleteDB() {

    try {
        std::filesystem::remove(_bot_db_path);
    } catch (std::filesystem::filesystem_error &e) {
        std::cerr << e.what() << std::endl;
    }

}


/**
 * It deletes all the data from the database
 */
void botDB::clearDB() {

    std::string sql = "DELETE FROM expense; "
                      "DELETE FROM category; "
                      "DELETE FROM budget;";

    char *zErrMsg = nullptr;

    int rc = sqlite3_exec(_bot_db, sql.c_str(), nullptr, nullptr, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

}

/**
 * @brief getter for the database path
 *
 * @return The path to the database.
 */
std::string botDB::getDBPath() const {

    return _bot_db_path;
}


/**
 * It takes a SQL request as a string, executes it, and returns the result as a map of strings
 *
 * @param SQL_request The SQL request you want to execute.
 *
 * @return A map of strings.
 */
std::map<std::string, std::string> botDB::fetchOne(std::string SQL_request) {

    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(_bot_db, SQL_request.c_str(), -1, &stmt, nullptr);

    std::map<std::string, std::string> result;

    if (sqlite3_step(stmt) == SQLITE_ROW) {

        for (int i = 0; i < sqlite3_column_count(stmt); i++) {
            result[std::string((char *) sqlite3_column_name(stmt, i))] =
                    std::string((char *) sqlite3_column_text(stmt, i));
        }

    }

    sqlite3_finalize(stmt);

    return result;
}

/**
 * It inserts some base categories into the database
 */
void botDB::fill_base_categories() {

    std::string sql = "insert into category (codename, name, is_base_expense, aliases)  values \n"
                      "('products', 'продукты', true, 'еда'), \n "
                      "('food', 'еда', true, ''), \n"
                      "('fuel', 'топливо', true, ''), \n"
                      "('cash', 'наличные', true, ''), \n"
                      "('coffee', 'кофе', true, ''), \n"
                      "('dinner', 'обед', true, 'столовая, ланч, бизнес-ланч, бизнес ланч'), \n"
                      "('other', 'другое', true, '');";

    char *zErrMsg = nullptr;

    int rc = sqlite3_exec(_bot_db, sql.c_str(), nullptr, nullptr, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error from  fill_base_categories : " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

}

/**
 * It takes a SQL request as a string, executes it, and returns the result as a vector of maps
 *
 * @param SQL_request The SQL request you want to execute.
 *
 * @return A vector of maps.
 */
auto botDB::fetchMany(const std::string &SQL_request) -> std::vector<std::map<std::string, std::string>> {

    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(_bot_db, SQL_request.c_str(), -1, &stmt, nullptr);

    std::vector<std::map<std::string, std::string>> result;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::map<std::string, std::string> row;

        for (int i = 0; i < sqlite3_column_count(stmt); i++) {
            row[std::string((char *) sqlite3_column_name(stmt, i))] =
                    std::string((char *) sqlite3_column_text(stmt, i));
        }

        result.push_back(row);
    }

    sqlite3_finalize(stmt);

    return result;
}








