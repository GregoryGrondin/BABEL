/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** CoHandler
*/

#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "DBManager.hpp"

// #include "Includer.hpp"

class CoHandler : public boost::enable_shared_from_this<CoHandler> {

    // METHODS
    public:
        typedef boost::shared_ptr<CoHandler> CH_shptr;

        CoHandler(boost::asio::io_service& io_service, DBManager *dbm);
        ~CoHandler();

        static CH_shptr create(boost::asio::io_service& io_service, DBManager *dbm) {
            return CH_shptr(new CoHandler(io_service, dbm));
        }

        boost::asio::ip::tcp::socket& socket();
        void start();
        void setMsg(std::string n_msg) { 
            std::cout << "CH to ip : " << sock.remote_endpoint().address().to_string();
            std::cout << "\n has n_msg = " << n_msg << '\n';
            message = n_msg;
        };

    private:
        void handle_read(const boost::system::error_code& err);
        void handle_write(const boost::system::error_code& err);

    // VARIABLES
    public:
        DBManager *db_man;

    private:
        boost::asio::ip::tcp::socket sock;
        std::string message;
        enum { max_length = 1024 };
        char data[max_length];
};