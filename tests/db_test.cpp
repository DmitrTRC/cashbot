//
// Created by Dmitry Morozov on 8/10/22.
//
#include <gtest/gtest.h>

#include "db.hpp"

TEST(DBTest, TestDB) {


    DB::Expense expense;
    botDB db;

    EXPECT_EQ(db.getCallState(), false);

    db.setCallState(true);

    EXPECT_EQ(db.getCallState(), true);

}