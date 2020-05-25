#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QByteArray>
#include <QDataStream>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
signals:

public slots:

    void incommingConnection(); // обработчик входящего подключения
    void readyRead(); // обработчик входящих данных
    void stateChanged(QAbstractSocket::SocketState stat);

private:
    QTcpServer *server; // указатель на сервер
    QList<QTcpSocket *> sockets; // получатели данных
    QTcpSocket *firstSocket; // вещатель
signals:

};

#endif // SERVER_H
