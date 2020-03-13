/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** main
*/

#include "mainwindow.hpp"

/*
#include <iostream>
#include <boost/asio.hpp>
#include <math.h>
#include <portaudio.h>
#include <stdio.h>
#include <stdlib.h>

#include "Audio.hpp"
#include "CSocket.hpp"

int main(int argc, char **argv)
{
    CSocket *s1;
    int nb_seconds = 100;
    int c = 0;

    if (strcmp(argv[1], "1") == 0) {
        s1 = new CSocket(CSocket::role_enum::MASTER, 2422, 2432, argv[2]);
    } else {
        s1 = new CSocket(CSocket::role_enum::SLAVE, 2432, 2422, argv[2]);
    }

    Audio audio(s1);
    audio.startStream();
    while (c != nb_seconds) {
        if (strcmp(argv[3], "--withoutEncoding") == 0) {
            audio.readStreamWithoutEncoding();
        } else {
            audio.readStream();
        }
        c++;
    }
    audio.stopStream();
}
*/

int main(int argc, char **argv) {
///////////////initialisation client////////////
    QApplication app(argc, argv);
    MainWindow w;
    boost::asio::io_service io_service;
    w.initClient(io_service);
    w.show();
    if (w.connectToServer() != 1)
        exit (84);
    if (w.writeOnline() == 84)
        exit (84);
    w.getContactList();
    w.babel_client();
    //w.setWindowTitle("Babel Client");
    //w.show();
    //std::thread fred(sometst, &w);
    //w.ask_for_call(2);
    //std::thread t1(connectthread, &w);
    //fred.join();
    app.exec();
    //app.exec();
    //fred.join();
    //fred.join();
    //wt1.join();
    return (0);
///////////////Connection au serveur/////////////

//////////////Read asynchrone: attente d'appel/////
    /*std::thread receive_call_thread = c_self.member1Thread();
    while (1)
        if (c_self.receive_call() == 1) {

        } else {

        }
    receive_call_thread.join();*/



/////////////QT: bouton pour appeller, changer de nom, refresh la liste des contact//////////

    /*c_self.ask_for_call(2);
    c_self.mtx.lock();
    while (c_self.calling == 1) {
        c_self.audio_stream->readStreamWithoutEncoding();
    }
    c_self.mtx.unlock();*/

///////////////Deconnection du client//////////////
    /*if (c_self.write_offline() == 84)
        return 84;
    return 0;*/
}
