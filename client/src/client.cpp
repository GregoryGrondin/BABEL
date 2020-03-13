/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** client
*/

#include "client.hpp"

int Client::begin_call(int id)
{
    std::string ip = get_ip_from_id(id);
    CSocket *s1;
    if (ip == "NULL")
        exit(84);
    std::cout << "before socket\n";
    s1 = new CSocket(CSocket::role_enum::MASTER, 2422, 2432, ip);
    //s1 = new CSocket(CSocket::role_enum::MASTER, 2422, 2432, "127.0.0.1");
    std::cout << "before new audio\n";
    audio_stream = new Audio(s1);
    audio_stream->startStream();
    calling = 1;
    return (1);
}

std::string Client::get_ip_from_id(int id)
{
    for (auto iter : contact_tab) {
        if (iter.get_id() == id) {
            std::cout << "Waiting for : Name: " << iter.get_name() << "  ID :" << iter.get_id()<< "  IP: "<< iter.get_ip() << "  Online: "<< iter.get_online() << "\n";
            return (iter.get_ip());
        }
    }
    return ("NULL");
}

int Client::ask_for_call(int id)
{
    char data[1024];
    std::string call = "call:" + std::to_string(id);

    boost::system::error_code error;
    boost::asio::write(*s_socket, boost::asio::buffer(call), error);
    if (!error)
        std::cout << "Client send begin call!" << std::endl;
    else {
        std::cout << "send failed: " << error.message() << std::endl;
        return (84);
    }
    
    std::string tmp;
    begin_call(id);
    std::cout << "papa c'est moi\n";
    s_socket->read_some(boost::asio::buffer(data, 1024), error);
    std::string s = data;
    std::cout << "ok ici" << std::endl;
    if (!error) {
        if (s == "OK\n" || s == "OK") {
             std::cout << "Success to call" << std::endl;
             begin_call(id);
             return (1);
        } else {
            std::cout << "Call not possible: " << std::endl;
            return (0);
        }
    } else {
        std::cout << "Call not possible: " << error.message() << std::endl;
        return (84);
    }
    return (0);
}

int Client::receive_call()
{

    // while (1 == 1) {
    //     boost::system::error_code error;
    //     char data[1024];
    //     std::string tmp;
    //     //mtx.lock();
    //     s_socket->read_some(boost::asio::buffer(data, 1024), error);
    //     std::string s = data;
    //     s = s.substr(0, 2);
    //     std::cout << "I'm in the tread"<< '\n';
    //     if (!error) {
    //         if (s == "OK\n" || s == "OK") {
    //             std::cout << "thread received OK" << std::endl;
    //             break;
    //         }
    //     }
    //     else {
    //         std::cout << "received failed: " << error.message() << std::endl;
    //         return (84);
    //     }
    //     //mtx.unlock();
    // }
    return (0);
}

int Client::set_name()
{
    std::string tmp;
    std::cin  >> tmp;
    if (tmp.size() > 64)
        std::cout << "64 character maximum." << std::endl;
    name = tmp;
    boost::system::error_code error;
    const std::string msg = "nameIs:" + name + "\n";
    boost::asio::write( *s_socket, boost::asio::buffer(msg), error );
    if (!error)
        std::cout << "Client send new name!" << std::endl;
    else {
        std::cout << "send failed: " << error.message() << std::endl;
        return (84);
    }
    while (1 == 1) {
        //mtx.lock();
        char data[1024];
        std::string tmp;
        s_socket->read_some(boost::asio::buffer(data, 1024), error);
        std::string s = data;
        s = s.substr(0, 2);
        std::cout << s << '\n';
        if (!error) {
            if (s == "OK\n" || s == "OK") {
                std::cout << "Name changed on server" << std::endl;
                break;
            }
        }
        else {
            std::cout << "received failed: " << error.message() << std::endl;
            return (84);
        }
        //mtx.unlock();
    }
    return (0);
}

int Client::parse_contact_l(std::string contact_l)
{
    std::vector<std::string> contact_str;
    size_t pos = 0;
    std::string delim = "\t";
    std::string ip;
    std::string name;

    while ((pos = contact_l.find(delim)) != std::string::npos) {
        contact_str.push_back(contact_l.substr(0, pos));
        contact_l.erase(0, pos + delim.length());
    }
    contact_str.push_back(contact_l.substr(0,contact_l.size() - 1));
    delim = "|";
    for (int j = 0; j < (int)contact_str.size(); j++) {
        std::vector<std::string> tmp;
        while ((pos = contact_str[j].find(delim)) != std::string::npos) {
            tmp.push_back(contact_str[j].substr(0, pos));
            contact_str[j].erase(0, pos + delim.length());
        }
        tmp.push_back(contact_str[j]);
        contact_tab.push_back(Contact_info(stoi(tmp[0]), tmp[1], tmp[2], stoi(tmp[3])));
    }

    for (size_t i = 0; i < contact_tab.size(); i++) {
         std::cout << "Name: " << contact_tab[i].get_name() << "  ID :" << contact_tab[i].get_id()<< "  IP: "<< contact_tab[i].get_ip() << "  Online: "<< contact_tab[i].get_online()<<'\n';
    }
    return 0;
}

int Client::get_contact_list()
{
    boost::system::error_code error;
    const std::string msg = "refreshC\n";
    boost::asio::write( *s_socket, boost::asio::buffer(msg), error );
    if (!error)
        std::cout << "Client ask contact list!" << std::endl;
    else {
        std::cout << "send failed: " << error.message() << std::endl;
        return (84);
    }
    //mtx.lock();
    char data[1024];
    s_socket->read_some(boost::asio::buffer(data, 1024), error);
    std::string contact_l = data;
    if (!error)
        std::cout << "Contact list received!" << std::endl;
    else {
        std::cout << "received failed: " << error.message() << std::endl;
        return (84);
    }
    //mtx.unlock();
    parse_contact_l(contact_l);
    return (0);
}


int Client::write_offline() {
    boost::system::error_code error;
    const std::string msg = "amOffline\n";
    boost::asio::write( *s_socket, boost::asio::buffer(msg), error );
    if (!error)
        std::cout << "Client sent quit message!" << std::endl;
    else {
        std::cout << "send failed: " << error.message() << std::endl;
        return (84);
    }
    return (0);
}


int Client::write_online() {
    boost::system::error_code error;
    const std::string msg = "amOnline\n";
    boost::asio::write( *s_socket, boost::asio::buffer(msg), error );
    if (!error)
        std::cout << "Client sent hello message!" << std::endl;
    else {
        std::cout << "send failed: " << error.message() << std::endl;
        return (84);
    }
    std::cout << "Waiting for OK" << std::endl;

    while (1 == 1) {
        char data[1024];
        std::string tmp;
        s_socket->read_some(boost::asio::buffer(data, 1024), error);
        std::string s = data;
        s = s.substr(0, 2);
        std::cout << s << '\n';
        if (!error) {
            if (s == "OK\n" || s == "OK") {
                std::cout << "next" << '\n';
                break;
            }
        }
        else {
            std::cout << "received failed: " << error.message() << std::endl;
            return (84);
        }
    }
    return (0);
}

int Client::connectToServer() {
    boost::system::error_code ec;
    //time_t start_time = time(NULL);

    //std::cout << "Connecting to Babel server ...\n";

    do {
        s_socket->connect( boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 12345), ec);
        //if (time(NULL) - start_time > 10.0f) {
        //    std::cerr << "Error : Server not found" << '\n';
        //    return (84);
        //}
    } while (ec);


    return (1);
}
