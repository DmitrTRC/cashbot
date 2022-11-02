//
// Created by Dmitry Morozov on 2/10/22.
//

#ifndef CASHBOT_FEATURE_DB_CATEGORIES_HPP
#define CASHBOT_FEATURE_DB_CATEGORIES_HPP

#include "db.hpp"
#include "db_structure.hpp"

#include <map>
#include <string>
#include <vector>

/**
 * @brief Class for working with the categories table
 */
class Categories {
public:
    /**
     * @brief Constructor
     * @param db Pointer to the database
     */
    explicit Categories(botDB *db);


    ///<Destructor
    ~Categories() = default;

    /**
     * @brief Get all categories from the database
     * @returns Vector of categories
     */
    DB::TCategories getAllCategories();

    /**
     * @brief Get a category by its name
     * @returns Category
     */
    DB::Category getCategory(const std::string &);


private:
    ///< Pointer to the database
    botDB *_db_handler; ///< Pointer to the database

    ///< Vector of categories
    DB::TCategories _categories; ///< Vector of categories

    /**
     * @brief Fill alias names for categories
     * @returns Vector of categories
     */
    static DB::TCategories _fill_aliases(std::vector<std::map<std::string, std::string>> &categories);


    /**
     * @brief Load all categories from the database
     * @returns Vector of categories
     */
    DB::TCategories _getCategoriesFromDB();

};

#endif //CASHBOT_FEATURE_DB_CATEGORIES_HPP
