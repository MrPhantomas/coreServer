#include "server.h"
#include <QByteArray>
#include <QString>
Server::Server(QObject *parent) :
    QObject(parent),
    firstSocket(NULL)
{
    server = new QTcpServer(this);
    qDebug() << "server listen = " << server->listen(QHostAddress::Any, 6666);
    connect(server, SIGNAL(newConnection()), this, SLOT(incommingConnection()));
}

void Server::incommingConnection() // обработчик подключений
{
    QTcpSocket * socket = server->nextPendingConnection();
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
    if (!firstSocket) {
        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
        socket->write("server");
        firstSocket = socket;
        qDebug() << "this one is server";
    }
    else if(sockets.size()<2){
        QString str("client count ");
        QByteArray arr;
        arr.append(str);
        arr.append(QString::number(sockets.size()+1));
        socket->write(arr, qstrlen(arr));
        sockets << socket;
    }
    else
    {
        socket->write("cant connect! server is full");
        socket->close();
    }
}

void Server::readyRead() // обработчик входящих сообщений от "вещающего"
{
    qDebug() << "[1]";
    QByteArray arr =  firstSocket->readAll();
    foreach(QTcpSocket *socket, sockets) {
        if (socket->state() == QTcpSocket::ConnectedState)
            socket->write(arr);
    }
}

void Server::stateChanged(QAbstractSocket::SocketState state)
{
    QObject * object = QObject::sender();
    if (!object)
        return;
    QTcpSocket * socket = static_cast<QTcpSocket *>(object);
    qDebug() << state;
    if (socket == firstSocket && state == QAbstractSocket::UnconnectedState)
        firstSocket = NULL;
}
