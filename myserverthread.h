#ifndef MYSERVERTHREAD_H
#define MYSERVERTHREAD_H

#include <QObject>
#include <QThread>
#include <QtNetwork>
#include <QDebug>

#include "mysocket.h"

class myserverthread : public QThread
{
    Q_OBJECT
public:
    explicit myserverthread(int threadid,int socket, QObject *parent = nullptr);
    void run();

signals:
    void MSTreadyread(int threadid, int sockDesc, const QByteArray &array);
    void disconnectthread(int threadid);
public slots:
    void recieveddata(int sockDesc, const QByteArray &array);
    void disconnect();
private:
    int sockDesc;
    mysocket *socket;
    int threadID;//用来帮助ui知道更新那一部分数据的

};

#endif // MYSERVERTHREAD_H
