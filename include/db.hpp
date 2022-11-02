//
// Created by Dmitry Morozov on 2/10/22.
//

#ifndef CASHBOT_FEATURE_DB_DB_HPP
#define CASHBOT_FEATURE_DB_DB_HPP


#include "db_structure.hpp"

#include <boost/algorithm/string/join.hpp>
#include <iostream>
#include <map>
#include <sqlite3.h>
#include <string>


/// Constexpr for the database name
constexpr auto DB_PATH = "../db/cashbot.db";


/**
 * @brief Class ORM for working with the database
 */
class botDB {

public:
    /**
     * @brief Constructor
     * @param db_path Path to the database file
     */
    botDB() : botDB(DB_PATH) {};

    /// Constructor with path to the database file
    explicit botDB(const std::string &db_path);

    /// Destructor
    ~botDB();

    /**
     * @brief Insert a new record into the database
     * @param table Table name
     * @param values Map of values
     */
    void insertRow(const std::string &table, const std::map<std::string, std::string> &);

/**
 * @brief delete a record from the database
 * @param table Table name
 * @param id ID of the record
 */
    void deleteRow(const std::string &, const long &id);

    /**
     * @brief Fetch one record from the database
     * @param SQL_query SQL query
     * @returns Map of values
     */
    std::map<std::string, std::string> fetchOne(std::string);

    /**
     * @brief Fills base categories table with default values
     */
    void fill_base_categories();


    /**
     * @brief clear all tables
     */
    void clearDB();


    /**
     * @brief Fetch all records from the database
     * @param table Table name
     * @param columns Vector of columns
     * @returns Vector of maps of values
     */
    std::vector<std::map<std::string, std::string >>
    fetchAll(const std::string &table, const std::vector<std::string> &);

    /**
     * @brief Fetch many records from the database
     * @param SQL_request SQL request
     * @returns Vector of maps of values
     */
    std::vector<std::map<std::string, std::string>> fetchMany(const std::string &SQL_request);

    /**
     * @brief Getter for CallState
     * @return bool CallState value
     */
    [[nodiscard]] bool getCallState() const;

    /**
     * @brief Setter for CallState
     * @param CallState value
     */
    void setCallState(bool callState);

    /**
     * @brief Returns current DB Path
     * @return std::string DB Path
     */
    [[nodiscard]] std::string getDBPath() const;

    /**
     * @brief Fill Example DB
     */
    void fill_test_tables();

    /**
     * @brief delete all records from the database
     */
    void deleteDB();


private:

    sqlite3 *_bot_db; /// Pointer to the database

    std::string _bot_db_path; /// Path to the database file

    /**
     * @brief Open the database
     * @return Pointer to the database
     */
    sqlite3 *_openDB();

    /**
     * @brief Close the database
     */
    void _closeDB();

    /**
     * @brief Init the database
     */
    void _initDB();

    /**
     * @brief Check if the database is exist
     * @return True if the database is exist
     */
    bool _check_db_exists();

    bool _call_state = false; /// Call state for the database Exec callback

};

#endif //CASHBOT_FEATURE_DB_DB_HPP
