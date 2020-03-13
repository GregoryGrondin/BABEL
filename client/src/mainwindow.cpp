/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** main
*/

#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , calling(0)
{
    ui->setupUi(this);
    QMessageBox::about(this, "WELCOME", "Hello, and welcome on our Babel client. The project is still in development. "
                                        "For now, you can call your friends and speak with them for hours.");
    waiting_window();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::waiting_window()
{
    QMovie *movie = new QMovie("../../client/media/loading.gif");

    ui->contactlist->hide();
    ui->callbutton->hide();
    ui->callingtext->hide();
    ui->hangbutton->hide();
    ui->waitinglab_gif->setMovie(movie);
    movie->start();
}

void MainWindow::babel_client()
{
    boost::asio::io_service io_service;
    ui->waitinglab_gif->clear();
    ui->waitinlab_txt->setText("Connection successful !");
    ui->waitinlab_txt->clear();
    ui->contactlist->show();
    ui->joincall->show();
    ui->callbutton->setIcon(QIcon("../../client/media/phocall.ico"));
    ui->callbutton->setIconSize(QSize(32, 32));
    ui->callbutton->show();
    std::vector<QListWidgetItem *> item_vect;

    for (auto iter : contact_tab) {
        std::string ct = iter.get_name();
        std::string tp = iter.get_ip();
        std::string res = ct + "\t" + tp;
        QListWidgetItem *tmp = new QListWidgetItem(QIcon("../../client/media/user.ico"), QString::fromUtf8(res.c_str()));
        item_vect.push_back(tmp);
        ui->contactlist->addItem(tmp);
    }
}

void MainWindow::on_next1_clicked()
{
}

void MainWindow::on_callbutton_clicked()
{
    ui->callingtext->setText("Calling . . .");
    ui->hangbutton->setIcon(QIcon("../../client/media/phone.ico"));
    ui->hangbutton->setIconSize(QSize(65, 65));
    ui->callingtext->show();
    ui->callbutton->hide();
    ui->hangbutton->show();

    call_type = 1;
    QApplication::processEvents();
    ask_for_call(2);
    call_update(1);
    while (call_update(2) == 1) {
        QApplication::processEvents();
        audio_stream->readStreamWithoutEncoding(0);
    }
    //audio_stream->stopStream();
    //exit (0);
}

void MainWindow::on_joincall_clicked()
 {
    ui->callingtext->setText("Calling . . .");
    ui->hangbutton->setIcon(QIcon("../../client/media/phone.ico"));
    ui->hangbutton->setIconSize(QSize(65, 65));
    ui->callingtext->show();
    ui->callbutton->hide();
    ui->hangbutton->show();

    call_type = 2;
    QApplication::processEvents();
    std::string current = ui->contactlist->currentItem()->text().toUtf8().constData();
    std::size_t pos = current.find("\t");
    std::string ip = current.substr(pos + 1);
    ui->callbutton->hide();
    //ui->hangbutton->show();
    CSocket *s1 = new CSocket(CSocket::role_enum::SLAVE, 2432, 2422, ip);
    audio2 = new Audio(s1);
    audio2->startStream();
    
    call_update(1);
    while (call_update(2) == 1) {
      QApplication::processEvents();
      audio2->readStreamWithoutEncoding(0);
    }
    audio2->stopStream();
 }

void MainWindow::on_hangbutton_clicked()
{
    ui->callingtext->hide();
    ui->callbutton->show();
    ui->hangbutton->hide();
    QMessageBox::about(this, "Thanks for using our service", "Thank you, give us credits please.");
    //audio2->readStreamWithoutEncoding(1);
    if (call_type == 1) {
        audio_stream->readStreamWithoutEncoding(1);
    } else {
        audio2->readStreamWithoutEncoding(1);
    }
    call_update(0);
}
