//
// Created by Dmitry Morozov on 2/10/22.
//

#include "Categories.hpp"

#include <boost/algorithm/string.hpp>


DB::TCategories Categories::getAllCategories () {
    return _categories;
}

DB::Category Categories::getCategory (const std::string &categoryName) {
    DB::Category resultCategory;
    DB::Category otherCategory;


    for (auto &category: _categories) {

        if (category.codename == categoryName) {
            otherCategory = category;
        }

        for (auto &alias: category.aliases) {
            if (std::find (category.aliases.begin (), category.aliases.end (), categoryName) !=
                category.aliases.end ()) {
                resultCategory = category;
            }

        }

    }
    if (resultCategory.codename.empty ()) {
        resultCategory = otherCategory;
    }
    return resultCategory;
}


DB::TCategories Categories::_fill_aliases (std::vector<std::map<std::string, std::string>> &categories) {
    DB::TCategories resultCategories;

    for (auto &category: categories) {
        std::vector<std::string> aliases;
        boost::split (aliases, category["aliases"], boost::is_any_of (","));
        aliases.push_back (category["codename"]);
        aliases.push_back (category["name"]);
        resultCategories.push_back (DB::Category{
                .codename = category["codename"],
                .name = category["name"],
                .is_base_expense = category["is_base_expense"] == "1",
                .aliases = aliases
        });
    }

    return resultCategories;

}

