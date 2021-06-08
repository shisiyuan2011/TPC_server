#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->online_statue_1->setText("not online");
    ui->online_statue_2->setText("not online");
    ui->online_statue_3->setText("not online");
    ui->online_statue_4->setText("not online");

    //数据库部分
    db1 = new database("No1");
    db2 = new database("No2");
    db3 = new database("No3");
    db4 = new database("No4");
    QThread *t1 = new QThread;
    QThread *t2 = new QThread;
    QThread *t3 = new QThread;
    QThread *t4 = new QThread;

    db1->moveToThread(t1);
    db2->moveToThread(t2);
    db3->moveToThread(t3);
    db4->moveToThread(t4);

    t1->start();
    t2->start();
    t3->start();
    t4->start();

    connect(this,&Widget::datacome1,db1,&database::savedata);
    connect(this,&Widget::datacome2,db2,&database::savedata);
    connect(this,&Widget::datacome3,db3,&database::savedata);
    connect(this,&Widget::datacome4,db4,&database::savedata);
    //end

    initialplot();

    m = new myserver(this);
    m->listen(QHostAddress::Any,8888);
    connect(m,SIGNAL(MSEreadyread(int, int, const QByteArray &)),
            this, SLOT(handledata(int, int, const QByteArray &)));
    connect(m,SIGNAL(disconnectserver(int)),
            this,SLOT(disconnectwidget(int)));
    connect(m,SIGNAL(connectserver(int)),
            this,SLOT(connectwidget(int)));
}

Widget::~Widget()
{
    delete ui;
}
void Widget::handledata(int threadid, int sockdesc, const QByteArray &a){
    //qDebug()<<"emit recieved signal recieved"<<a;
    //通过协议对数据进行拆解，处理
    QString rawdata = a;
    QStringList datalist = rawdata.split(" ");
    struct data getdata = fliter(datalist);
    if(sockdesc == u1sockdesc){
        emit datacome1(a);
        ui->work_statue_1->setText(getdata.statue);
        if(getdata.statue == "working"){
            //力添加曲线
            ui->No1_plot->xAxis->setRange(0,1.5*plot1);
            ui->No1_plot->graph(0)->addData(plot1,getdata.curforce);
            ui->No1_plot->replot();
            ++plot1;
        }else{
            ui->No1_plot->graph(0)->data().data()->clear();
            ui->No1_plot->replot();
        }
        //qDebug()<<getdata.curforce;
        ui->lcd_speed_1->display(QString::number(getdata.curspeed));
        ui->lcdcir_1->display(QString::number(getdata.curcir));
    }else if(sockdesc == u2sockdesc){
        emit datacome2(a);
        ui->work_statue_2->setText(getdata.statue);
        if(getdata.statue == "working"){
            //力添加曲线
            ui->No2_plot->xAxis->setRange(0,1.5*plot2);
            ui->No2_plot->graph(0)->addData(plot2,getdata.curforce);
            ui->No2_plot->replot();
            ++plot2;
        }else{
            ui->No2_plot->graph(0)->data().data()->clear();
            ui->No2_plot->replot();
        }
        ui->lcd_speed_2->display(QString::number(getdata.curspeed));
        ui->lcdcir_2->display(QString::number(getdata.curcir));
    }else if(sockdesc == u3sockdesc){
        emit datacome3(a);
        ui->work_statue_3->setText(getdata.statue);
        if(getdata.statue == "working"){
            //力添加曲线
            ui->No3_plot->xAxis->setRange(0,1.5*plot3);
            ui->No3_plot->graph(0)->addData(plot3,getdata.curforce);
            ui->No3_plot->replot();
            ++plot3;
        }else{
            ui->No3_plot->graph(0)->data().data()->clear();
            ui->No3_plot->replot();
        }
        ui->lcd_speed_3->display(QString::number(getdata.curspeed));
        ui->lcdcir_3->display(QString::number(getdata.curcir));
    }else if(sockdesc == u4sockdesc){
        emit datacome4(a);
        ui->work_statue_4->setText(getdata.statue);
        if(getdata.statue == "working"){
            //力添加曲线
            ui->No4_plot->xAxis->setRange(0,1.5*plot4);
            ui->No4_plot->graph(0)->addData(plot4,getdata.curforce);
            ui->No4_plot->replot();
            ++plot4;
        }else{
            ui->No4_plot->graph(0)->data().data()->clear();
            ui->No4_plot->replot();
        }
        ui->lcd_speed_4->display(QString::number(getdata.curspeed));
        ui->lcdcir_4->display(QString::number(getdata.curcir));
    }
}
void Widget::connectwidget(int sockdesc){
    //检查ui状态，给空的ui分配myserverthread线程
    if(u1statue==false){
        u1sockdesc = sockdesc;
        ui->online_statue_1->setText("online");
        u1statue = true;
    }else if(u2statue==false){
        u2sockdesc = sockdesc;
        ui->online_statue_2->setText("online");
        u2statue = true;
    }else if(u3statue==false){
        u3sockdesc = sockdesc;
        ui->online_statue_3->setText("online");
        u3statue = true;
    }else if(u4statue==false){
        u4sockdesc = sockdesc;
        ui->online_statue_4->setText("online");
        u4statue = true;
    }else{
        QMessageBox::warning(this,"out of range","can only connect 4 client");
    }
}
void Widget::disconnectwidget(int sockdesc){
    //把sockdesc对应的ui给清空，并重置ui状态
    if(sockdesc == u1sockdesc){
        ui->online_statue_1->setText("not online");
        ui->work_statue_1->setText("");
        ui->lcdcir_1->display("");
        ui->lcd_speed_1->display("");
        ui->No1_plot->graph(0)->data().data()->clear();
        ui->No1_plot->replot();
        plot1 = 1;
        u1sockdesc = 0;
        u1statue = false;
    }else if(sockdesc == u2sockdesc){
        ui->online_statue_2->setText("not online");
        ui->work_statue_2->setText("");
        ui->lcdcir_2->display("");
        ui->lcd_speed_2->display("");
        ui->No2_plot->graph(0)->data().data()->clear();
        ui->No2_plot->replot();
        plot2 = 1;
        u2sockdesc = 0;
        u2statue = false;
    }else if(sockdesc == u3sockdesc){
        ui->online_statue_3->setText("not online");
        ui->work_statue_3->setText("");
        ui->lcdcir_3->display("");
        ui->lcd_speed_3->display("");
        ui->No3_plot->graph(0)->data().data()->clear();
        ui->No3_plot->replot();
        plot3 = 1;
        u3sockdesc = 0;
        u3statue = false;
    }else if(sockdesc == u4sockdesc){
        ui->online_statue_4->setText("not online");
        ui->work_statue_4->setText("");
        ui->lcdcir_4->display("");
        ui->lcd_speed_4->display("");
        ui->No4_plot->graph(0)->data().data()->clear();
        ui->No4_plot->replot();
        plot4 = 1;
        u4sockdesc = 0;
        u4statue = false;
    }else{
        QMessageBox::warning(this,"out of range","no link to the server");
    }
}
void Widget::test(){
    qDebug()<<"test";
    ui->online_statue_1->setText("test");
}

struct data Widget::fliter(QStringList rawdata){
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
    return result;
}
void Widget::initialplot(){
    ui->No1_plot->addGraph();
    ui->No2_plot->addGraph();
    ui->No3_plot->addGraph();
    ui->No4_plot->addGraph();

    ui->No1_plot->yAxis->setRange(0,200);
    ui->No2_plot->yAxis->setRange(0,200);
    ui->No3_plot->yAxis->setRange(0,200);
    ui->No4_plot->yAxis->setRange(0,200);

    ui->No1_plot->setInteractions(QCP::iRangeDrag |
                                  QCP::iRangeZoom |
                                  QCP::iSelectPlottables);
    ui->No2_plot->setInteractions(QCP::iRangeDrag |
                                  QCP::iRangeZoom |
                                  QCP::iSelectPlottables);
    ui->No3_plot->setInteractions(QCP::iRangeDrag |
                                  QCP::iRangeZoom |
                                  QCP::iSelectPlottables);
    ui->No4_plot->setInteractions(QCP::iRangeDrag |
                                  QCP::iRangeZoom |
                                  QCP::iSelectPlottables);
}
