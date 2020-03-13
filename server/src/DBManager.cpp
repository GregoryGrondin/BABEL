/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** DBManager
*/

#include "DBManager.hpp"
#include "CoHandler.hpp"
#include "Server.hpp"

bool DBManager::randBool;
ClientInfo *DBManager::c_list;

DBManager::DBManager(void *serv) {
    (void)serv;
    // server = serv;

    // std::cout << "server : " << *((Server *)server) << '\n';
    // std::cout << "serv : " << *((Server *)serv) << '\n';

    randBool = false;
    open();

    // set first ClientInfo
    DB_REQ_TYPE r_type = DB_REQ_TYPE::REF_CLIST;
    std::string s_sql = "select * from users";
    execSql(s_sql, (void *)(&r_type));
}

DBManager::~DBManager() {
    for (ClientInfo *c_it = c_list; c_it != nullptr; c_it = c_it->getNext())
        Server::changeCoStat(nullptr, c_it->getIP(), false);
    
    std::cout << "closing db\n";
    close();
}

int DBManager::callback(void *data, int ac, char **av, char **azColName) {
	// for (int i = 0; i < ac; i++)
	// 	std::cout << azColName[i] << " = " << (av[i] ? av[i] : "NULL") << "\n";
    // std::cout << '\n';

    (void)azColName;

    switch (*(DB_REQ_TYPE *)data) {
        case 1: // test first connexion
            if (ac > 0 && av[0][0] == '0')
                randBool = true;
            break;
        case 2: // refresh contact list
            if (ac % 4 == 0)
                addClientInfo(av);
            else
                std::cerr << "error: new client info incorrect\n";
            break;
        default:
            break;
    }

	return (0);

    /*
    **  typedef int (*sqlite3_callback)(
    **  void*,    // Data provided in the 4th argument of sqlite3_exec()
    **  int,      // The number of columns in row
    **  char**,   // An array of strings representing fields in the row
    **  char**    // An array of strings representing column names
    **  );
    */
}

void DBManager::execSql(std::string statement, void *data) {
    int rc = 0;
    char *sql = 0;
    char *errMsg = 0;

    sql = strdup(statement.c_str());
    rc = sqlite3_exec(db, sql, DBManager::callback, data, &errMsg);

    std::cout << "sql statement : " + statement + "\n";

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s | rc = %i\n", errMsg, rc);
        sqlite3_free(errMsg);
    } else
        std::cout << "Operation done successfully\n";
}

void DBManager::open() {
    int rc = sqlite3_open("server/db/s_babel", &db);

    if (rc) {
        std::cout << "DB Error: " << sqlite3_errmsg(db) << '\n';
        sqlite3_close(db);
        exit(84);
    }
}

// void DBManager::addClient(std::string ip, std::string name) {
//     DB_REQ_TYPE r_type = DB_REQ_TYPE::NO_REQ;
//     std::string s_sql = "insert into users (ip, name, co_stat) "
//                         + (std::string)"values (\"" + ip + "\", \"" + name + "\", true);";
//     execSql(s_sql, (void *)(&r_type));
// }

// void DBManager::changeCoStat(void *caller_ch, std::string ip, bool co_stat) {
//     // check if first connexion ever (sets randBool accordingly)
//     DB_REQ_TYPE r_type = DB_REQ_TYPE::FIRST_CO;
//     std::string s_sql = "select count(ip) from users where ip = \"" + ip + "\";";
//     execSql(s_sql, (void *)(&r_type));

//     if (randBool) {
//         randBool = false;
//         addClient(ip, "default");
//     } else {
//         r_type = DB_REQ_TYPE::CO_STAT_MOD;
//         s_sql = "update users set co_stat = \""
//                 + (std::string)(co_stat ? "true\"" : "false\"")
//                 + " where ip = \"" + ip + "\";";
//         execSql(s_sql, (void *)(&r_type));
//     }

//     // send connection confirmation
//     if (co_stat)
//         ((CoHandler *)caller_ch)->setMsg((std::string)("OK\n"));
// }

void updateServerInfo(ClientInfo *client, char **av) {
    if (av[2] != NULL && client->getName().compare((std::string)av[2]) != 0)
        client->setName((std::string)av[2]);
    if (av[3] != NULL)
        client->setCoStat((std::string)(av[3][0] == 't' ? "1" : "0"));
}

void DBManager::addClientInfo(char **av) {
    ClientInfo *c_it = c_list;
    ClientInfo *n_client = new ClientInfo(av);

    if (c_it == nullptr) {
        c_list = n_client;
        return;
    }

    bool verif = false;
    while (c_it->getNext() != nullptr) {
        if (!verif && c_it->isIP((std::string)av[1])) {
            updateServerInfo(c_it, av);
            verif = true;
        }
        c_it = c_it->getNext();
    }
    if (!verif) {
        if (!c_it->isIP((std::string)av[1]))
            c_it->setNext(n_client);
        else
            updateServerInfo(c_it, av);
    }

    // std::cout << "full c_list :\n";
    // for (c_it = c_list; c_it != nullptr; c_it = c_it->getNext()) {
    //     std::cout << c_it->to_string() << '\n';
    // }
    // std::cout << "end of c_list\n";
}

// void DBManager::refreshCList(void *caller_ch, std::string ip) {
//     DB_REQ_TYPE r_type = DB_REQ_TYPE::REF_CLIST;
//     std::string s_sql = "select * from users;";
//     execSql(s_sql, (void *)(&r_type));

//     // send contact infos
//     ClientInfo *c_it = c_list;
//     std::string fin_msg;// = c_it->to_string();
//     while (c_it->getNext() != nullptr) {
//         if (!c_it->isIP(ip))
//             fin_msg = fin_msg + c_it->to_string() + '\t';
//         c_it = c_it->getNext();
//     }
//     if (!c_it->isIP(ip))
//         fin_msg = fin_msg + c_it->to_string() + '\n';
//     else if (!fin_msg.empty()) {
//         fin_msg.pop_back();
//         fin_msg.push_back('\n');
//     }

//     ((CoHandler *)caller_ch)->setMsg(fin_msg);
// }

// void DBManager::setName(void *caller_ch, std::string ip, std::string name) {
//     DB_REQ_TYPE r_type = DB_REQ_TYPE::NAME_CHANGE;
//     std::string s_sql = "update users set name = \"" + name + "\" where ip = \"" + ip + "\";";
//     execSql(s_sql, (void *)(&r_type));

//     ((CoHandler *)caller_ch)->setMsg("OK\n");
// }

// void DBManager::getInContact(void *caller_ch, std::string caller_ip, std::string callee_id) {
//     ClientInfo *caller;
//     ClientInfo *callee;
    
//     for (ClientInfo *c_it = c_list; c_it != nullptr; c_it = c_it->getNext()) {
//         if (callee_id.compare(c_it->getID()) == 0)
//             callee = c_it;
//         else if (caller_ip.compare(c_it->getIP()) == 0)
//             caller = c_it;
//     }
//     if (caller == nullptr || callee == nullptr || !callee->isJoinable())
//         ((CoHandler *)caller_ch)->setMsg("KO\n");
//     else {
//         ((CoHandler *)caller_ch)->setMsg("OK\n");
//         // ((Server *)server)->remoteSetMsg(callee->getIP(), "talkTo:" + caller->getID() + '\n');
//     }
// }