//
// Created by Dmitry Morozov on 2/10/22.
//

#ifndef CASHBOT_FEATURE_DB_CATEGORIES_HPP
#define CASHBOT_FEATURE_DB_CATEGORIES_HPP

#include "db.hpp"
#include "db_structure.hpp"

#include <vector>
#include <map>
#include <string>


class Categories {
public:
    explicit Categories(botDB *db);

    ~Categories();

    DB::TCategories getAllCategories();

    static DB::Category getCategory(const std::string &);


private:
    botDB *_db_handler;

    DB::TCategories _categories;

    static DB::TCategories _fill_aliases(std::vector<std::map<std::string, std::string>> &categories);

    DB::TCategories _getCategoriesFromDB();


};


#endif //CASHBOT_FEATURE_DB_CATEGORIES_HPP
