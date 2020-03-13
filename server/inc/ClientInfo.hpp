/*
** EPITECH PROJECT, 2019
** server
** File description:
** ClientInfo
*/

#pragma once

#include <string>
#include <iostream>

// #include "Includer.hpp"

class ClientInfo {
    public:
        ClientInfo(char **av);
        ~ClientInfo();

        std::string to_string();
        bool isIP(std::string n_ip);
        bool isJoinable();

        void setNext(ClientInfo *n_next) { next = n_next; };
        ClientInfo *getNext() { return (next); };

        std::string getID() { return id; };
        void setID(std::string n_id) { id = n_id; };
        std::string getIP() { return ip; };
        void setIP(std::string n_ip) { ip = n_ip; };
        std::string getName() { return name; };
        void setName(std::string n_name) { name = n_name; };
        std::string getCoStat() { return co_stat; };
        void setCoStat(std::string n_co_stat) { co_stat = n_co_stat; };

    private:
        std::string id, ip, name, co_stat;
        ClientInfo *next = nullptr;
        void *parent_ch;
};