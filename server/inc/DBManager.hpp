/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** DBManager
*/

#pragma once

#include <string>
#include <cstring>
#include <iostream>
#include "ClientInfo.hpp"

extern "C" {
    #include "sqlite3.h"
}

typedef enum {
    NO_REQ = 0,
    FIRST_CO,
    REF_CLIST,
    CO_STAT_MOD,
    NAME_CHANGE
} DB_REQ_TYPE;

class DBManager {
	public:
		DBManager(void *serv);
		~DBManager();

        void open();
        void close() { sqlite3_close(db); }

        void execSql(std::string statement, void *data);
        static int callback(void *data, int ac, char **av, char **azColName);

        ClientInfo *getCList() { return c_list; };

        static bool randBool;
    
    private:
        static void addClientInfo(char **av);

        static ClientInfo *c_list;
        sqlite3 *db;
};
