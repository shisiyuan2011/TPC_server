#include "mysocket.h"

mysocket::mysocket(int sockDesc, QObject *parent) : QTcpSocket(parent)
{
    sockDesc_socket = sockDesc;
    connect(this, SIGNAL(readyRead()),this, SLOT(recieveddata()));
}
mysocket::~mysocket(){

}
void mysocket:: recieveddata(){
    //QByteArray a = readAll();
    //emit MSreadyread(sockDesc_socket, a);

    //防止粘包处理,一边处理一边读取
    //定义表头是###，表尾是&&&，假定数据中没有#与&
    //数据处理部分
    QString n;
    QString temp;
    while(bytesAvailable()>0){
        temp = read(1);
        if(temp == "#"){
            n += temp;
        }
        if(n.contains("###",Qt::CaseSensitive)&&!n.contains("&&&",Qt::CaseSensitive)){
            //后面的都是数据，直到&&&的出现
            if(temp == "#"){
            }else{
               n += temp;
            }
        }
        if(n.contains("###",Qt::CaseSensitive)&&n.contains("&&&",Qt::CaseSensitive)){
            //发射信号
            emit MSreadyread(sockDesc_socket, n.toUtf8());
            QThread::msleep(10);
            n.clear();
        }
        if(n.size()>50) n.clear();
    }
}
void mysocket::sentdata(int sockDesc, const QByteArray &array){
    if(sockDesc == sockDesc_socket)
    {
        write(array);
        return;
    }
    else
    {
        qDebug()<<"ID is not right";
        return;
    }
}
