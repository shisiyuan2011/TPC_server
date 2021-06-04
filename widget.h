#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "mysocket.h"
#include "myserver.h"
#include "myserverthread.h"
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui {class Widget;}
QT_END_NAMESPACE
/*
struct data{
    QString statue;
    int curspeed = 0;
    double curforce = 0;
    long int curcir = 0;
    QString dbname;
};*/

class myserver;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    void test();
    ~Widget();

public slots:
    void handledata(int threadid, int sockdesc, const QByteArray &a);
    void disconnectwidget(int);
    void connectwidget(int);

signals:
    void datacome1(const QByteArray &a);
    void datacome2(const QByteArray &a);
    void datacome3(const QByteArray &a);
    void datacome4(const QByteArray &a);

private:
    Ui::Widget *ui;
    friend class myserver;
    myserver *m;

private:
    int u1sockdesc=0,u2sockdesc=0,u3sockdesc=0,u4sockdesc=0;
    bool u1statue=false,u2statue=false,u3statue=false,u4statue=false;
    //false是空闲true是占用
    int k=0;
    long int plot1 = 1,plot2 = 1,plot3 = 1,plot4 = 1;

    database *db1;
    database *db2;
    database *db3;
    database *db4;

private:
    struct data fliter(QStringList);
    void initialplot();
};
#endif // WIDGET_H
