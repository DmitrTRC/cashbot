//
// Created by Dmitry Morozov on 8/10/22.
//
#include <gtest/gtest.h>

#include "db.hpp"

TEST(DBTest, TestDB) {


    botDB db;

    EXPECT_EQ(db.getCallState(), true);

    db.setCallState(false);

    EXPECT_EQ(db.getCallState(), false);

}