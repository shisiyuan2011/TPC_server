#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QObject>
#include <iostream>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QThread>

class mysocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit mysocket(int sockDesc, QObject *parent = nullptr);
    ~mysocket();
    void sentdata(int sockDesc, const QByteArray &array);
public slots:
    void recieveddata();

signals:
    void MSreadyread(int sockDesc, const QByteArray &array);
private:
    int sockDesc_socket;
};

#endif // MYSOCKET_H
