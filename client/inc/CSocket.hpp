/*
** EPITECH PROJECT, 2019
** NtekS
** File description:
** input.hpp
*/

#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include "main.hpp"
#include "main.hpp"

class CSocket
{
    public:
        enum role_enum {
            MASTER,
            SLAVE
        };
        CSocket(role_enum role, int portRead, int portWrite, std::string ipAdress);
        ~CSocket();
        int hello();
        int sendData(float *, int);
        float *readData();
        int getNbBytes();

        int nb_bytes;
        role_enum _role;
        int _portRead;
        int _portWrite;
        tcp::socket *_socketRead;
        tcp::socket *_socketWrite;
};
