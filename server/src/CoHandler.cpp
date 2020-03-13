/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** CoHandler
*/

#include "CoHandler.hpp"
#include "Server.hpp"

CoHandler::CoHandler(boost::asio::io_service& io_service, DBManager *dbm)
    : sock(io_service) {
        // boost::system::error_code ec;
        // sock.open(boost::asio::ip::tcp::v4(), ec);
        // if (ec) {
        //     std::cerr << "error on socket opening : " << ec.message() << '\n';
        //     exit(84);
        // }
        db_man = dbm;
}

CoHandler::~CoHandler() {}

boost::asio::ip::tcp::socket& CoHandler::socket() {
    return sock;
}

void CoHandler::start() {

    // std::cout << "CoHandler reading and writing\n";

    sock.async_read_some( boost::asio::buffer(data, max_length),
                        boost::bind(&CoHandler::handle_read,
                        shared_from_this(),
                        boost::asio::placeholders::error));/*,
                        boost::asio::placeholders::bytes_transferred));*/
    
    if (!message.empty()) {

        std::cout << "sending : " << message << " to : " << sock.remote_endpoint().address().to_string() << '\n';

        sock.async_write_some( boost::asio::buffer(message, max_length),
                            boost::bind(&CoHandler::handle_write,
                            shared_from_this(),
                            boost::asio::placeholders::error));/*,
                            boost::asio::placeholders::bytes_transferred));*/
    }
}

void CoHandler::handle_read(const boost::system::error_code& err) {
    std::string rem_ip(sock.remote_endpoint().address().to_string());
    std::string s_data(data);

    /* what if:

    // old
    A map of strings and function pointers is given
    as parameter in the cohandler constructor instead of the 
    db manager

    the function pointers point towards Server::methods
    so that he (the Server) makes the calls to the db_man
    // old


    DBManager methods moved to Server class and made static
    so accessible by CoHandler

    db_man would only have exeqSql method and something for the 
    Server to get the responses from the sql requests made by the db

    */

    if (!err && s_data.size() < 72) {
        std::cout << rem_ip << " sent : " << data;
        if (s_data.compare("amOnline\n") == 0)
            Server::changeCoStat(this, rem_ip, true);
        else if (s_data.compare("amOffline\n") == 0) {
            Server::changeCoStat(this, rem_ip, false);
            return;
        } else if (s_data.compare("refreshC\n") == 0)
            Server::refreshCList(this, rem_ip);
        else if (s_data.size() > 8 && s_data.substr(0, 7).compare("nameIs:") == 0)
            Server::setName(this, rem_ip, s_data.substr(7, s_data.size() - 8));
        else if (s_data.size() > 6 && s_data.substr(0, 5).compare("call:") == 0)
            Server::getInContact(this, rem_ip, s_data.substr(5, s_data.size() - 6));
        for (int i = 0; i < max_length; ++i)
            data[i] = 0;
        start();
    } else {
        if (err == boost::asio::error::eof) {
            std::cerr << "Client has disconnected\n";
            Server::changeCoStat(this, rem_ip, false);
        } else
            std::cerr << "error: " << err.message() << std::endl;
    }
}

void CoHandler::handle_write(const boost::system::error_code& err) {
    message.clear();
    if (!err) {
        // std::cout << "no error on msg launch\n";
        start();
    } else {
        std::cerr << "error: " << err.message() << std::endl;
        sock.close();
    }
}