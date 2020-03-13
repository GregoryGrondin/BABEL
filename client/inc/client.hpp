/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** client
*/

#include "Audio.hpp"
#include "CSocket.hpp"
#include "boost/asio.hpp"
#include <time.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <boost/asio.hpp>
#include <math.h>
#include <portaudio.h>
#include <stdio.h>
#include <stdlib.h>

class Contact_info
{
    public:
        Contact_info(int id, std::string ip, std::string name, int online):id_(id), ip_(ip), name_(name), online_(online){}
        int get_id() {return id_;}
        int get_online() {return online_;}
        std::string get_ip() {return ip_;}
        std::string get_name() {return name_;}

    private:
        int id_;
        std::string ip_;
        std::string name_;
        int online_;
};

class Client {
    public:
        std::mutex mtx;
        Client(boost::asio::io_service & io_service)
            : calling(0)
        {
            s_socket = new boost::asio::ip::tcp::socket(io_service);
        };
        Client(Client *c2) {s_socket = c2->s_socket;
                        contact_tab = c2->contact_tab;};
        ~Client() {};

        int begin_call(int);
        std::vector<Contact_info> getContactTab(void) {return contact_tab;};
        int ask_for_call(int);
        int connectToServer();
        int write_online();
        int get_contact_list();
        int parse_contact_l(std::string);
        int write_offline();
        int set_name();
        std::string get_ip_from_id(int);
        int receive_call();
        void call_receive_call() {receive_call();}
        int calling;
        Audio *audio_stream;
        std::thread member1Thread() { return std::thread([=] { call_receive_call(); });}
    private:
        boost::asio::ip::tcp::socket *s_socket;
        boost::asio::streambuf receive_buffer;
        std::vector<Contact_info> contact_tab;
        std::string name;
};
