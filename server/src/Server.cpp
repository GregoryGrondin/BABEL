/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Server
*/

#include "Server.hpp"

DBManager *Server::db_man;
std::vector<CoHandler::CH_shptr> Server::ch_list;

// ++ Constructor / Destructor ++
Server::Server(boost::asio::io_service& io_service)
    : acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 12345))
{
    db_man = new DBManager(this);
    start_accept();
}

Server::~Server() {
    db_man->close();
}
// -- Constructor / Destructor --


// ++ Connection management ++
void Server::handle_accept(CoHandler::CH_shptr connection, const boost::system::error_code& err) {

    if (connection->socket().remote_endpoint().address().to_string().compare("10.109.251.225") == 0)
        std::cout << "pierre connection addr : " << connection << "\n";

    if (!err)
        connection->start();
    start_accept();
}

void Server::start_accept() {
    CoHandler::CH_shptr connection = CoHandler::create(acceptor.get_io_service(), db_man);

    Server::ch_list.push_back(connection);

    acceptor.async_accept(Server::ch_list.back()->socket(),
    boost::bind(&Server::handle_accept, this, Server::ch_list.back(),
    boost::asio::placeholders::error));
}
// -- Connection management --


// ++ Respond to Client ++
void Server::remoteSetMsg(std::string rem_ip, std::string n_msg) {
    // std::vector<CoHandler::CH_shptr>::iterator it = ch_list.begin();

    // for (; it != ch_list.end(); ++it) {
    //     std::string test_ip((*it)->socket().remote_endpoint().address().to_string());
    //     if (test_ip.compare(rem_ip) == 0) {
    //         (*it)->setMsg(n_msg);
    //         break;
    //     }
    // }
    for (int i = 0; i < (int)ch_list.size(); ++i) {
        std::string test_ip(ch_list[i]->socket().remote_endpoint().address().to_string());
        if (test_ip.compare(rem_ip) == 0) {
            std::cout << "sending : " << n_msg << " as : " << ch_list[i] << '\n';
            ch_list[i]->setMsg(n_msg);
            break;
        }
    }
}

// -- Respond to Client --


// ++ Server actions ++
void Server::setName(CoHandler *caller_ch, std::string ip, std::string name) {

    std::cout << "setname method\n";

    DB_REQ_TYPE r_type = DB_REQ_TYPE::NAME_CHANGE;
    std::string s_sql = "update users set name = \"" + name + "\" where ip = \"" + ip + "\";";
    db_man->execSql(s_sql, (void *)(&r_type));

    caller_ch->setMsg("OK\n");
}

void Server::addClient(std::string ip, std::string name) {
    DB_REQ_TYPE r_type = DB_REQ_TYPE::NO_REQ;
    std::string s_sql = "insert into users (ip, name, co_stat) "
                        + (std::string)"values (\"" + ip + "\", \"" + name + "\", true);";
    db_man->execSql(s_sql, (void *)(&r_type));
}

void Server::changeCoStat(CoHandler *caller_ch, std::string ip, bool co_stat) {
    // check if first connexion ever (sets randBool accordingly)
    DB_REQ_TYPE r_type = DB_REQ_TYPE::FIRST_CO;
    std::string s_sql = "select count(ip) from users where ip = \"" + ip + "\";";
    db_man->execSql(s_sql, (void *)(&r_type));

    if (db_man->randBool) {
        db_man->randBool = false;
        addClient(ip, "default");
    } else {
        r_type = DB_REQ_TYPE::CO_STAT_MOD;
        s_sql = "update users set co_stat = \""
                + (std::string)(co_stat ? "true\"" : "false\"")
                + " where ip = \"" + ip + "\";";
        db_man->execSql(s_sql, (void *)(&r_type));
    }

    // send connection confirmation
    if (co_stat)
        caller_ch->setMsg((std::string)("OK\n"));
}

void Server::refreshCList(CoHandler *caller_ch, std::string ip) {
    DB_REQ_TYPE r_type = DB_REQ_TYPE::REF_CLIST;
    std::string s_sql = "select * from users;";
    db_man->execSql(s_sql, (void *)(&r_type));

    // send contact infos
    ClientInfo *c_it = db_man->getCList();
    std::string fin_msg;// = c_it->to_string();
    while (c_it->getNext() != nullptr) {
        if (!c_it->isIP(ip))
            fin_msg = fin_msg + c_it->to_string() + '\t';
        c_it = c_it->getNext();
    }
    if (!c_it->isIP(ip))
        fin_msg = fin_msg + c_it->to_string() + '\n';
    else if (!fin_msg.empty()) {
        fin_msg.pop_back();
        fin_msg.push_back('\n');
    }

    caller_ch->setMsg(fin_msg);
}

void Server::getInContact(CoHandler *caller_ch, std::string caller_ip, std::string callee_id) {
    ClientInfo *caller;
    ClientInfo *callee;
    
    for (ClientInfo *c_it = db_man->getCList(); c_it != nullptr; c_it = c_it->getNext()) {
        if (callee_id.compare(c_it->getID()) == 0)
            callee = c_it;
        else if (caller_ip.compare(c_it->getIP()) == 0)
            caller = c_it;
    }
    if (caller == nullptr || callee == nullptr || !callee->isJoinable())
        caller_ch->setMsg("KO\n");
    else {
        caller_ch->setMsg("OK\n");
        remoteSetMsg(callee->getIP(), "talkTo:" + caller->getID() + '\n');
    }
}

// -- Server actions --
