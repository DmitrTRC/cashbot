//
// Created by Dmitry Morozov on 2/10/22.
//

#include "Categories.hpp"
#include "db.hpp"

#include <boost/algorithm/string.hpp>


DB::TCategories Categories::getAllCategories() {

    return _categories;
}

DB::Category Categories::getCategory(const std::string &categoryName) {

    DB::Category resultCategory;
    DB::Category otherCategory;


    for (auto &category: _categories) {

        if (category.codename == categoryName) {
            otherCategory = category;
        }

        for (auto &alias: category.aliases) {
            if (alias.find(categoryName) != std::string::npos) {
                resultCategory = category;
            }

        }

    }
    if (resultCategory.codename.empty()) {
        resultCategory = otherCategory;
    }
    return resultCategory;
}


DB::TCategories Categories::_fill_aliases(std::vector<std::map<std::string, std::string>> &categories) {

    DB::TCategories resultCategories;

    for (auto &category: categories) {
        std::vector<std::string> aliases;
        boost::split(aliases, category["aliases"], boost::is_any_of(","));
        aliases.push_back(category["codename"]);
        aliases.push_back(category["name"]);
        resultCategories.push_back(DB::Category{
                .codename = category["codename"],
                .name = category["name"],
                .is_base_expense = category["is_base_expense"] == "1",
                .aliases = aliases
        });
    }

    return resultCategories;

}

DB::TCategories Categories::_getCategoriesFromDB() {

    std::vector<std::string> columns = {"codename", "name", "is_base_expense", "aliases"};
    std::string query_name = "category";

    std::vector<std::map<std::string, std::string >> categories = _db_handler->fetchAll(query_name, columns);
    auto result = _fill_aliases(categories);

    return {}; //TODO: return result! (now it's empty)
}

Categories::Categories(botDB *db) {

    _db_handler = db;
    _categories = _getCategoriesFromDB();

}

