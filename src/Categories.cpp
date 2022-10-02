//
// Created by Dmitry Morozov on 2/10/22.
//

#include "Categories.hpp"


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

DB::TCategories Categories::_fill_aliases (std::vector<std::map<std::string, std::string>> &) {
    DB::TCategories categories_result;

    size_t index = 0;
    //FIXME: Wrong logic
//    for (auto &category: _categories) {
//        std::vector<std::string> aliases;
//        for (auto &alias: _categories[index].aliases) {
//            aliases.push_back (alias);
//        }
//        categories_result.push_back (DB::Category {
//                .codename = category.codename,
//                .name = category.name,
//                .is_base_expense = category.is_base_expense,
//                .aliases = aliases
//        });
//        index++;
//    }

    return categories_result;
}


