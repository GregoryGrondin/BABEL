/*
** EPITECH PROJECT, 2019
** server
** File description:
** ClientInfo
*/

#include "ClientInfo.hpp"

ClientInfo::ClientInfo(char **av) {
    id = (av[0] == NULL) ? "NULL" : av[0];
    ip = (av[1] == NULL) ? "NULL" : av[1];
    name = (av[2] == NULL) ? "default" : av[2];
    co_stat = (av[3] == NULL) ? "0" :
                (av[3][0] == 't') ? "1" : "0";
}

std::string ClientInfo::to_string() {
    return (id + "|" + ip + "|" + name + "|" + co_stat);
}

bool ClientInfo::isIP(std::string n_ip) {
    return (ip.compare(n_ip) == 0);
}

bool ClientInfo::isJoinable() {
    if (co_stat.compare("1") == 0)
        return (true);
    return (false);
}