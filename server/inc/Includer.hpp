/*
** EPITECH PROJECT, 2019
** server
** File description:
** Includer
*/

#pragma once

#include <string>
#include <iostream>
#include <cstring>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "CoHandler.hpp"
#include "ClientInfo.hpp"
#include "DBManager.hpp"
#include "Server.hpp"

extern "C" {
    #include "sqlite3.h"
}