/*
** EPITECH PROJECT, 2019
** NtekS
** File description:
** IComponent.hpp
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <boost/asio.hpp>
#include <math.h>
#include <portaudio.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include "Audio.hpp"
#include "CSocket.hpp"
#include "client.hpp"
#include <QMainWindow>
#include <QMessageBox>
#include <QLabel>
#include <QMovie>
#include <QApplication>
#include <QPushButton>
#include <QThread>
#include <QList>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // GRAPHICAL
    void setStatus(int status) {this->status = status;};
    void babel_client();

    // NETWORK
    void initClient(boost::asio::io_service & io_service) {
        s_socket = new boost::asio::ip::tcp::socket(io_service);
    };
    int connectToServer();
    int writeOnline();
    int getContactList();
    int parseContactL(std::string);
    int ask_for_call(int);
    int begin_call(int);
    std::string get_ip_from_id(int);
    void waiting_window();
    static int call_update(int call) {
        static int call1 = 0;

        if (call == 2)
            return (call1);
        call1 = call;
        return (call1);
    };
    Audio *audio_stream;
    Ui::MainWindow *ui;
private slots:
    void on_next1_clicked();

    void on_callbutton_clicked();

    void on_hangbutton_clicked();

    void on_joincall_clicked();

private:
    int status = 0;
    int call_type;
    //CLIENT VAR
    int calling;
    boost::asio::ip::tcp::socket *s_socket;
    std::vector<Contact_info> contact_tab;
    Audio *audio2;
};
#endif // MAINWINDOW_H
