#include "database.h"

database::database(QString connectionname, QObject *parent) : QObject(parent)
{
    db1 = new QSqlDatabase;
    coName = connectionname;
    folder = new QDir;
    path = folder->absolutePath() + "/database";
    if(!folder->exists(path))
    {
        if(!folder->mkdir(path))
        {
            qDebug()<<"create folder fail";
        }
    }
}
database::~database(){
    delete  folder;
    db1->close();
    delete db1;
}
void database::savedata(const QByteArray &a){
    //qDebug()<<"last order untile now time espace: "<<mytime.elapsed();
    data getdata = fliter(a);
    if(getdata.statue == "working")
    {
        fomerwrite = true;
    }
    if(firstsave){
        *db1 = QSqlDatabase::addDatabase("QSQLITE",coName);
        QString name = path + "/" + getdata.dbname+".db";
        db1->setDatabaseName(name);
        if(!db1->open())
        {
            qDebug()<<"open db fail";
            return;
        }
        else
        {
            qDebug()<<"open success";
        }
        fomername = getdata.dbname;

        QString Qquery = "CREATE TABLE IF NOT EXISTS DATALOG(ID INTEGER PRIMARY KEY AUTOINCREMENT,"
"FORCE TEXT, SPEED TEXT, CURCIR TEXT)";
        QSqlQuery query(*db1);
        if(!query.exec(Qquery))
        {
            qDebug()<<"create table fail";
            return;
        }
        else
        {
            qDebug()<<"create table success";
        }
        firstsave = false;
        db1->transaction();//开启代理
    }
    else
    {
        //如果试验的名字改变的画，重新创建新的数据库
        if(fomername != getdata.dbname){
            db1->commit();//先把之前的全部提交
            db1->close();
            fomername = getdata.dbname;
            QString name = path + "/" + getdata.dbname+".db";
            db1->setDatabaseName(name);
            db1->open();
            QString Qquery = "CREATE TABLE IF NOT EXISTS DATALOG(ID INTEGER PRIMARY KEY AUTOINCREMENT,"
    "FORCE TEXT, SPEED TEXT, CURCIR TEXT)";
            QSqlQuery query(*db1);
            if(!query.exec(Qquery))
            {
                qDebug()<<"create table fail";
                return;
            }
            else
            {
                qDebug()<<"create table success";
            }

            db1->transaction();//再重开代理
        }
    }

    //存储数据
    QString Qquery;
    QSqlQuery query(*db1);
    if(querycounter < 10)//次数可以调整
    {
        if(fomerwrite == true && getdata.statue == "not working")
        {
            qDebug()<<"not working so save";
            Qquery = QString("INSERT INTO DATALOG (FORCE, SPEED, CURCIR) VALUES ('%1','%2','%3')").
                    arg(getdata.curforce).
                    arg(getdata.curspeed).
                    arg(getdata.curcir);
            query.exec(Qquery);
            db1->commit();
            db1->transaction();
            querycounter = 0;
            fomerwrite = false;
        }
        else
        {
            if(getdata.statue == "working")
            {
                Qquery = QString("INSERT INTO DATALOG (FORCE, SPEED, CURCIR) VALUES ('%1','%2','%3')").
                        arg(getdata.curforce).
                        arg(getdata.curspeed).
                        arg(getdata.curcir);
                query.exec(Qquery);
                ++querycounter;
            }
        }
    }
    else
    {
        db1->commit();//提交
        db1->transaction();//重新开启代理
        querycounter = 0;
    }
    mytime.restart();
}
struct data database::fliter(const QByteArray &a){
    QString raw = a;
    QStringList rawdata = raw.split(" ");
    struct data result;
    if(rawdata.at(2) == "0")
    {
        result.statue = "not working";
    }else
    {
        result.statue = "working";
    }
    result.curspeed = rawdata.at(3).toInt();
    result.curforce = rawdata.at(4).toDouble();
    result.curcir = rawdata.at(5).toLong();
    result.dbname = rawdata.at(1);
    return result;
}
