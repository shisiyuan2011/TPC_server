#include "myserverthread.h"

myserverthread::myserverthread(int threadid, int socket,QObject *parent) : QThread(parent)
{
    sockDesc = socket;
    threadID = threadid;
}
void myserverthread::run(){
    qDebug()<<"run()"<<sockDesc;
    socket = new mysocket(sockDesc);
    if (!socket->setSocketDescriptor(sockDesc)) {
        return ;
    }
    connect(socket,SIGNAL(MSreadyread(int, const QByteArray &)),
            this,SLOT(recieveddata(int, const QByteArray &)));
    connect(socket,&mysocket::disconnected,this,&myserverthread::disconnect);
    this->exec();
}
void myserverthread::recieveddata(int sockDesc, const QByteArray &array){
    emit MSTreadyread(threadID, sockDesc, array);
}
void myserverthread::disconnect(){
    socket->disconnectFromHost();
    emit disconnectthread(sockDesc);
    this->quit();
}
