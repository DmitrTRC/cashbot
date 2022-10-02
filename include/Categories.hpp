//
// Created by Dmitry Morozov on 2/10/22.
//

#ifndef CASHBOT_FEATURE_DB_CATEGORIES_HPP
#define CASHBOT_FEATURE_DB_CATEGORIES_HPP

#include "db_structure.hpp"

#include <vector>
#include <map>
#include <string>


class Categories {
public:
    Categories ();

    ~Categories ();

    DB::TCategories getAllCategories ();

    DB::Category getCategory (const std::string &);

    DB::TCategories _fill_aliases (std::vector<std::map<std::string, std::string>> &);

private:
    DB::TCategories _categories;


};


#endif //CASHBOT_FEATURE_DB_CATEGORIES_HPP
