//
// Created by Dmitry Morozov on 8/10/22.
//
#include <gtest/gtest.h>

#include "db.hpp"

TEST(DBTest, TestDB) {

    auto *db = new botDB();

    EXPECT_EQ(db->getCallState(), false);

    db->setCallState(true);

    EXPECT_EQ(db->getCallState(), true);

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
TEST(DBTestInsert, TestDBInsertExpense) {

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

    EXPECT_EQ(result[0]["created"], "2021-08-10 12:00:00");

    EXPECT_EQ(result[0]["category_codename"], "food");

    EXPECT_EQ(result[0]["raw_text"], "test");

}