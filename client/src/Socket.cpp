/*
** EPITECH PROJECT, 2019
** NtekS
** File description:
** input.hpp
*/

#include "CSocket.hpp"

CSocket::CSocket(role_enum role, int portRead, int portWrite, std::string ipAdress):
    _role(role),
    _portRead(portRead),
    _portWrite(portWrite)
{
    boost::asio::io_service io_service1;
    boost::asio::io_service io_service2;

    if (role == role_enum::MASTER) {
        tcp::acceptor acceptor1(io_service1, tcp::endpoint(tcp::v4(), portRead));
        tcp::acceptor acceptor2(io_service2, tcp::endpoint(tcp::v4(), portWrite));
        _socketRead = new tcp::socket(io_service1);
        _socketWrite = new tcp::socket(io_service2);
        acceptor1.accept(*_socketRead);
        acceptor2.accept(*_socketWrite);
    } else {
        _socketRead = new tcp::socket(io_service1);
        _socketWrite = new tcp::socket(io_service2);
        (_socketRead)->connect(tcp::endpoint(boost::asio::ip::address::from_string(ipAdress), portRead));
        (_socketWrite)->connect(tcp::endpoint(boost::asio::ip::address::from_string(ipAdress), portWrite));
    }
}

int CSocket::hello()
{
    const string msg = "Hello from Client!\n";

    boost::system::error_code error;
    boost::asio::write(*_socketWrite, boost::asio::buffer(msg), error );
    boost::asio::streambuf receive_buffer;
    boost::asio::read_until(*_socketRead, receive_buffer, "\n" );

    if ( error && error != boost::asio::error::eof ) {
        cout << "receive failed: " << error.message() << endl;
        return (0);
    } else {
        const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
        cout << data << endl;
    }
    return (1);
}

CSocket::~CSocket()
{
}

int CSocket::sendData(float *data, int numBytes)
{
    boost::system::error_code error;

    if (data == NULL) {
        boost::asio::write(*_socketWrite, boost::asio::buffer("\n\n"), error );
        boost::asio::write(*_socketWrite, boost::asio::buffer("\t\t\t\t"), error );
        return (0);
    }
    void *e = (void *)data;
    std::cout << (unsigned char *)data << "\n";
    boost::asio::write(*_socketWrite, boost::asio::buffer(e, numBytes), error );
    boost::asio::write(*_socketWrite, boost::asio::buffer("\t\t\t\t"), error );
    return (0);
}

int CSocket::getNbBytes()
{
    return (nb_bytes);
}

float *CSocket::readData()
{
    boost::asio::streambuf receive_buffer;
    float *data;

    boost::asio::read_until(*_socketRead, receive_buffer, "\t\t\t\t" );
    nb_bytes = receive_buffer.size();
    data = (float *)boost::asio::buffer_cast<const float*>(receive_buffer.data());
    std::cout << (unsigned char *)data << "\n";
    if (data[0] == '\n') {
        return (NULL);
    }
    return (data);
}
