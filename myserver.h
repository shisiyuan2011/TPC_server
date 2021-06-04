#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QTcpServer>
#include "myserverthread.h"
#include "widget.h"
class Widget;
class myserver : public QTcpServer
{
    Q_OBJECT
public:
    explicit myserver(QObject *parent = nullptr);
    void test();//用于测试
public slots:
    void recieveddata(int threadid, int sockDesc, const QByteArray &array);
    void disconnect(int threadid);

signals:
    void MSEreadyread(int threadid, int sockDesc, const QByteArray &array);
    void disconnectserver(int threadID);
    void connectserver(int threadID);

private:
    void incomingConnection(qintptr sockDesc);
    Widget *m_dialog;
    int alllink;//传入myserverthread的threadid是从1开始

};

#endif // MYSERVER_H
