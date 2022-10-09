//
// Created by Dmitry Morozov on 8/10/22.
//
#include <gtest/gtest.h>

#include "db.hpp"
#include "db_structure.hpp"

TEST(DBTest, TestDB) {

    auto *db = new botDB();

    auto state = db->getCallState();

    db->setCallState(true);

    EXPECT_EQ(db->getCallState(), true);

    db->setCallState(false);

    EXPECT_EQ(db->getCallState(), false);

    db->setCallState(state);

}
/**
 sql = "create table expense("
          "    id integer primary key, "
          "    user_id integer default 641480282, "
          "    amount integer, "
          "    created datetime, "
          "    category_codename integer, "
          "    raw_text text, "
          "    FOREIGN KEY(category_codename) REFERENCES category(codename) "
          ");";
          */
TEST(DBTestInsert, TestDBInsertExpenseRow) {

    auto *db = new botDB();

    std::map<std::string, std::string> expense = {
            {"user_id",           "641480282"},
            {"amount",            "100"},
            {"created",           "2021-08-10"},
            {"category_codename", "food"},
            {"raw_text",          "test"}
    };

    db->insertRow("expense", expense);

    std::vector<std::string> columns = {"id", "user_id", "amount", "created", "category_codename", "raw_text"};

    auto result = db->fetchAll("expense", columns);

    EXPECT_EQ(result.size(), 1);

    EXPECT_EQ(result[0]["user_id"], "641480282");

    EXPECT_EQ(result[0]["amount"], "100");

    EXPECT_EQ(result[0]["created"], "2021-08-10");

    EXPECT_EQ(result[0]["category_codename"], "food");

    EXPECT_EQ(result[0]["raw_text"], "test");

}

TEST(DBTestInsert, TestDBInsertBudgetRow) {

    auto *db = new botDB();

    std::map<std::string, std::string> budget = {
            {"codename",    "Life"},
            {"daily_limit", "1000"}
    };

    db->insertRow("budget", budget);

    std::vector<std::string> columns = {"codename", "daily_limit"};

    auto result = db->fetchAll("budget", columns);

    EXPECT_EQ(result.size(), 1);

    EXPECT_EQ(result[0]["codename"], "Life");

    EXPECT_EQ(result[0]["daily_limit"], "1000");

}