/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** main
*/

#include <stdio.h>
#include <unistd.h>
#include "Server.hpp"

Server *server;

void signalHandler(int signum) {
    server->getDBMan()->~DBManager();
    // for (ClientInfo *c_it = c_list; c_it != nullptr; c_it = c_it->getNext())
    //     changeCoStat(nullptr, c_it->getIP(), false);
    exit(signum);  
}

int main() {

    signal(SIGINT, signalHandler);  

    try {
        boost::asio::io_service io_service;  
        server = new Server(io_service);
        io_service.run();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return (84);
    }

    return (0);
}