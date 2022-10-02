//
// Created by Dmitry Morozov on 2/10/22.
//

#include "db.hpp"

#include <boost/algorithm/string/join.hpp>
#include <iostream>
#include <map>
#include <vector>


botDB::botDB () {
    _bot_db = _openDB ();
}

botDB::~botDB () {
    _closeDB ();
}

sqlite3 *botDB::_openDB () {
    sqlite3 *db;
    int rc = sqlite3_open (DB_PATH, &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg (db) << std::endl;
        sqlite3_close (db);
        exit (1);
    }
    return db;
}


void botDB::_closeDB () {
    sqlite3_close (_bot_db);
}


