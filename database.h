#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include "QSqlDatabase"
#include "QSqlError"
#include "QSqlQuery"
#include <QDebug>
#include <QThread>
#include <QTime>
#include <QDir>

struct data{
    QString statue;
    int curspeed = 0;
    double curforce = 0;
    long int curcir = 0;
    QString dbname;
};

class database : public QObject
{
    Q_OBJECT
public:
    explicit database(QString, QObject *parent = nullptr);
    ~database();

signals:
public slots:
    void savedata(const QByteArray &a);//一定要在需要写入的时候进行调用
private:
    struct data fliter(const QByteArray &);
private:
    QSqlDatabase *db1;
    QTime mytime;
    QString fomername = "";
    QString coName;
    QDir *folder;
    QString path;

    int querycounter = 0;
    bool firstsave = true;
    bool fomerwrite = false;
};

#endif // DATABASE_H
