/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Server
*/

#pragma once

#include "CoHandler.hpp"

class Server {
    public:
        // ++ Constructor / Destructor ++
        Server(boost::asio::io_service& io_service);
        ~Server();

        // ++ Server actions ++
        static void setName(CoHandler *caller_ch, std::string ip, std::string name);
        static void addClient(std::string ip, std::string name);
        static void changeCoStat(CoHandler *caller_ch, std::string ip, bool co_stat);
        static void refreshCList(CoHandler *caller_ch, std::string ip);
        static void getInContact(CoHandler *caller_ch, std::string rem_ip, std::string id);

        // ++ Getters ++
        DBManager *getDBMan() { return db_man; };

    private:
        // ++ Connection management ++
        void handle_accept(CoHandler::CH_shptr connection, const boost::system::error_code& err);
        void start_accept();
        
        // ++ Respond to Client ++
        static void remoteSetMsg(std::string rem_ip, std::string n_msg);

    private:
        boost::asio::ip::tcp::acceptor acceptor;
        static std::vector<CoHandler::CH_shptr> ch_list;
        static DBManager *db_man;
};