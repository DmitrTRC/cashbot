//
// Created by Dmitry Morozov on 8/10/22.
//
#include "db.hpp"
#include "db_structure.hpp"

#include <gtest/gtest.h>

#include <filesystem>


TEST(DBTest, DefaultConstructor_test) {

    botDB db;
    EXPECT_EQ(db.getDBPath(), DB_PATH);
}

TEST(DBTest, DB_CallState_test) {

    auto *db = new botDB("../db/cashbot_test.db");

    auto state = db->getCallState();

    db->setCallState(true);

    EXPECT_EQ(db->getCallState(), true);

    db->setCallState(false);

    EXPECT_EQ(db->getCallState(), false);

    db->setCallState(state);

}

TEST(DBTest, ClearDB_test) {

    auto *db = new botDB("../db/cashbot_test.db");

    db->clearDB();
    auto result = db->fetchAll("expense", {"id", "amount", "category", "date", "comment"});
    EXPECT_EQ(result.size(), 0);

    auto result2 = db->fetchAll("category", {"codename", "name", "is_base_expense", "aliases"});
    EXPECT_EQ(result2.size(), 0);

    db->insertRow("budget", {{"codename",    "test"},
                             {"daily_limit", "1000"}});
    auto result3 = db->fetchAll("budget", {"codename", "daily_limit"});
    EXPECT_EQ(result3.size(), 1);
    db->clearDB();
    auto result4 = db->fetchAll("budget", {"codename", "daily_limit"});
    EXPECT_EQ(result4.size(), 0);


}

TEST(DBTestInsert, DBInsertExpenseRow_test) {

    auto *db = new botDB("../db/cashbot_test.db");

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

TEST(DBTestInsert, DBInsertBudgetRow_test) {

    auto *db = new botDB("../db/cashbot_test.db");

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

TEST(DBTestInsert, DBInsertCategoryRow_test) {

    auto *db = new botDB("../db/cashbot_test.db");

    std::map<std::string, std::string> category = {
            {"codename",        "food"},
            {"name",            "Food"},
            {"is_base_expense", "1"},
            {"aliases",         "eat,drink"}
    };

    db->insertRow("category", category);

    std::vector<std::string> columns = {"codename", "name", "is_base_expense", "aliases"};

    auto result = db->fetchAll("category", columns);

    EXPECT_EQ(result.size(), 1);

    EXPECT_EQ(result[0]["codename"], "food");

    EXPECT_EQ(result[0]["name"], "Food");

    EXPECT_EQ(result[0]["is_base_expense"], "1");

    EXPECT_EQ(result[0]["aliases"], "eat,drink");

}

TEST(DB_FETCH_ONE, DBFetchOne_test) {

    auto *db = new botDB("../db/cashbot_test.db");

    std::map<std::string, std::string> expense = {
            {"user_id",           "641480282"},
            {"amount",            "100"},
            {"created",           "2021-08-10"},
            {"category_codename", "food"},
            {"raw_text",          "test"}
    };

    db->insertRow("expense", expense);

    std::vector<std::string> columns = {"id", "user_id", "amount", "created", "category_codename", "raw_text"};
    std::string sql = "SELECT * FROM expense WHERE user_id = 641480282";

    auto result = db->fetchOne(sql);

    EXPECT_EQ(result.size(), 6);

    EXPECT_EQ(result["user_id"], "641480282");

    EXPECT_EQ(result["amount"], "100");

    EXPECT_EQ(result["created"], "2021-08-10");


};





TEST(DB_FETCHALL, DBFetchAll_test) {

    auto *db = new botDB("../db/cashbot_test.db");

    db->clearDB();

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

TEST(TEST_FILL_SAMPLE, fill_test_tables_test) {

    auto *db = new botDB("../db/cashbot_test.db");

    db->clearDB();

    db->fill_test_tables();

    auto result = db->fetchAll("expense", {"id", "amount"});

    EXPECT_EQ(result.size(), 3);

    auto result2 = db->fetchAll("category", {"codename", "name", "is_base_expense", "aliases"});

    EXPECT_EQ(result2.size(), 3);

}

TEST(DB_TEST_DELETE, delete_db_file_test) {

    auto *db = new botDB("../db/cashbot_test.db");

    db->deleteDB();

    auto isDBFileExists = std::filesystem::exists("../db/cashbot_test.db");

    EXPECT_EQ(isDBFileExists, false);

}