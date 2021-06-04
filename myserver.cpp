#include "myserver.h"

myserver::myserver(QObject *parent) : QTcpServer(parent)
{
    /* get current dialog object */
    m_dialog = dynamic_cast<Widget *>(parent);
    alllink = 0;
}
void myserver::incomingConnection(qintptr sockDesc){
    qDebug()<<"new connection"<<"sockDesc:"<<sockDesc<<"create threadID:"<<alllink+1;
    ++alllink;
    emit connectserver(sockDesc);
    myserverthread *thread = new myserverthread(alllink,sockDesc);
    thread->start();
    connect(thread,SIGNAL(MSTreadyread(int, int, const QByteArray &)),
            this,SLOT(recieveddata(int, int, const QByteArray &)));
    connect(thread,SIGNAL(disconnectthread(int)),
            this,SLOT(disconnect(int)));
}
void myserver::recieveddata(int threadid, int sockDesc, const QByteArray &array){
    emit MSEreadyread(threadid, sockDesc, array);
}
void myserver::disconnect(int sockdesc){
    alllink--;
    emit disconnectserver(sockdesc);
}
void myserver::test(){
}

