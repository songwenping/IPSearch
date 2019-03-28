#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "factorytest.h"
#include "newmessagebox.h"
#include <QProcess>
#include <QDir>
#include <QDialog>
#include <QSettings>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif
#define MES 1

MainWidget::MainWidget(QWidget *parent,QWidget *tablewidget) :
    QMainWindow(parent),
    itemer(new QListWidgetItem()),
    m_socket(new QUdpManager),
    m_mesmansger(new MesManager),
    m_sockettimer(new QTimer),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    m_spliter = new QSplitter(Qt::Vertical,tablewidget);
    top_widget = new QTableWidget(m_spliter);
    //    m_list = new QListWidget( m_spliter );
    down_widget = new QTableWidget(m_spliter);

    m_spliter->addWidget(top_widget);
    m_spliter->addWidget( down_widget );
    m_spliter->setStretchFactor(0,3);//右面窗口上下窗口高度比为1：4
    m_spliter->setStretchFactor(1,1);

    QGridLayout* splayout = new QGridLayout(tablewidget);
    splayout->addWidget(m_spliter);
    tablewidget->setLayout(splayout);

    m_secondspliter = new QSplitter(Qt::Horizontal,down_widget);

    left_widget = new QTableWidget(m_secondspliter);
    //    m_list = new QListWidget( m_spliter );
    right_widget = new QTableWidget(m_secondspliter);

    m_secondspliter->addWidget(left_widget);
    m_secondspliter->addWidget( right_widget );

    m_secondspliter->setStretchFactor(0,1);//右面窗口上下窗口高度比为1：4
    m_secondspliter->setStretchFactor(1,4);

    QGridLayout* ssplayout = new QGridLayout(down_widget);
    ssplayout->addWidget(m_secondspliter);
    down_widget->setLayout(ssplayout);


    tablewidget1 = tablewidget;
    m_label = new QLabel("请扫描二维码");
    m_line = new QLineEdit();
    m_listwidget = new QListWidget();
    m_button=new Button(":/png/image/return.png", 1);

    itemer->setTextColor(QColor(178,34,34));
    m_listwidget->setObjectName("list");
    m_button->setToolTip("返回菜单");

    m_label->setAlignment(Qt::AlignHCenter);
    top_widget->setStyleSheet("QWidget{background: #ADD8E6;}"
                              "QListWidget{background: white;}"
                              "QLineEdit:!enabled {background-color: silver;border-style: outset;"
                              "border-width: 1px;border-radius: 5px;font: 16px;min-width:2em;"
                              "color:black;font-family:微软雅黑;padding: 5px;}"
                              "QLineEdit:enabled{background-color: white;}");
//    down_widget->setStyleSheet("QWidget{background: #ADD8E6;}"
//                               "QLineEdit:!enabled {background-color: silver;border-style: outset;"
//                               "border-width: 1px;border-radius: 5px;font: 16px;min-width:2em;"
//                               "color:black;font-family:微软雅黑;padding: 5px;}");

    QGridLayout* layout = new QGridLayout(top_widget);
    layout->addWidget(m_label,0,0,1,1);
    layout->addWidget(m_line,0,1,1,3);
    layout->addWidget(m_button,0,4,1,1);
    layout->addWidget(m_listwidget,1,0,layout->rowCount(),layout->columnCount());
    top_widget->setLayout(layout);


    //    stateslabel = new QLabel("Waite");
    //    stateslabel->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    bannerwidget = new BannerWidget();
    bannerwidget->setText("等待测试");
    QGridLayout *leftlayout = new QGridLayout(left_widget);
    leftlayout->addWidget(bannerwidget,0,0,leftlayout->rowCount(),leftlayout->columnCount());
    leftlayout->setMargin(0);

    historylabel = new QLabel("历史测试总数");
    nowlabel = new QLabel("本次测试数");
    suclabel = new QLabel("本次通过设备数");
    sucbililabel = new QLabel("本次测试直通率");
    historyline = new QLineEdit();
    nowline = new QLineEdit();
    sucline = new QLineEdit();
    sucbililine = new QLineEdit();
    historyline->setFocusPolicy(Qt::NoFocus);
    nowline->setFocusPolicy(Qt::NoFocus);
    sucline->setFocusPolicy(Qt::NoFocus);
    sucbililine->setFocusPolicy(Qt::NoFocus);
    QSettings settings1("testtool.ini", QSettings::IniFormat);

    historynum = settings1.value("HistoryNum/num").toInt();
    nowline->setText("0");
    sucline->setText("0");
    sucbililine->setText("0%");
    historyline->setText(QString("%1").arg(historynum));
    QGridLayout* downlayout = new QGridLayout(right_widget);
    //    downlayout->addWidget(stateslabel,0,0,downlayout->rowCount(),3);
    downlayout->addWidget(historylabel,0,0,1,1);
    downlayout->addWidget(historyline,0,1,1,1);
    downlayout->addWidget(suclabel,0,2,1,1);
    downlayout->addWidget(sucline,0,3,1,1);
    downlayout->addWidget(nowlabel,1,0,1,1);
    downlayout->addWidget(nowline,1,1,1,1);
    downlayout->addWidget(sucbililabel,1,2,1,1);
    downlayout->addWidget(sucbililine,1,3,1,1);
    right_widget->setLayout(downlayout);
#if 1
    m_line->setEnabled(false);
    m_line->setPlaceholderText("");
    m_line->installEventFilter(this);
#endif
    QSettings settings("testtool.ini", QSettings::IniFormat);
    m_checked  = settings.value("MesIP/checked").toInt();

    m_sockettimer->start(5000);
    connect(m_sockettimer,SIGNAL(timeout()),this,SLOT(sendtoserver()));
    connect(m_button,SIGNAL(clicked()),this,SLOT(turntomain()));

}

MainWidget::~MainWidget()
{
    delete ui;

}

QStringList MainWidget::g_itemlist;
int MainWidget::g_replytimes = 0;
int MainWidget::item = 0;
int MainWidget::g_vlc = 0;
int MainWidget::g_testnumber = 0;
int MainWidget::g_erronumber = 0;
int MainWidget::g_erroflags = 0;

void MainWidget::closeui()
{
    clearitem();
    foreach (QWidget *w, qApp->allWidgets())
        if (QMainWindow* mainWin = qobject_cast<QMainWindow*>(w))
            mainWin->close();

}



bool MainWidget::eventFilter(QObject *target, QEvent *event)
{
    if (target == m_line)
    {
        if(event->type() == QEvent::EnabledChange)
        {
            m_line->setFocus();
        }
    }
    return QObject::eventFilter(target, event);

}

void MainWidget::clearitem()
{
    if(!(g_itemlist.isEmpty()))
    {
        for(int i=0;i<g_itemlist.size();++i)
        {
            QString str = g_itemlist.at(i);
            qDebug()<<"str"<<str;
            int j=str.toInt();
            QListWidgetItem *item = new QListWidgetItem;
            item = m_listwidget->item(j);
            m_listwidget->takeItem(j);
            delete item;
        }
        g_itemlist.clear();
    }
}

void MainWidget::setrefont()
{
    itemer->setTextColor(QColor(178,34,34));
}

void MainWidget::print_ero(int row, QString string)
{
    QListWidgetItem* itemer = new QListWidgetItem;
    itemer->setTextColor(QColor(178,34,34));
    itemer->setText(QString(string));
    m_listwidget->insertItem(row,itemer);
    g_itemlist.append(QString::number(row));
}

void MainWidget::print_success(int row, QString string)
{
    QListWidgetItem* itemer = new QListWidgetItem;
    itemer->setTextColor(QColor(0,128,0));
    itemer->setText(QString(string));
    m_listwidget->insertItem(row,itemer);
    g_itemlist.append(QString::number(row));
}

void MainWidget::deletelayout()
{
    QLayoutItem *item = NULL;
    QGridLayout * layout = (QGridLayout *)tablewidget1->layout();
    if(layout)
    {
        while(NULL!=(item = layout->takeAt(0)))
        {
            item->widget()->close();//关闭但不释放
            delete item;
            item = NULL;
        }
    }
    delete layout;
    layout = NULL;
}

int MainWidget::checkreply()
{
    ++g_replytimes;
    if(g_replytimes == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void MainWidget::init()
{

    clearitem();
    m_line->clear();
    m_line->setEnabled(true);
    m_listwidget->clear();
    m_line->setPlaceholderText("请扫描二维码");
    item = 0;
    m_socket->sendmsgtoser(QString("MSG::56"));
}

void MainWidget::sweepsucess(QString socketmsg,QString mesmsg,QString sn)
{
    g_testnumber++;
    historynum++;
    m_line->setPlaceholderText("");
    m_line->setEnabled(false);

    m_listwidget->clear();
    m_listwidget->insertItem(0,QString("扫码完成"));
    bannerwidget->setText("测试中");
    if (1==m_checked)
    {
        m_mesmansger->startcheckmes(mesmsg,sn,0,"","");
    }
    else
    {
        if(QString::compare(socketmsg,"vlc") == 0)
        {
            m_vlc = new VlcManager();
            m_vlc->show();
            connect(m_vlc,SIGNAL(vlcmsg(QString)),this,SLOT(recevfromvlc()),Qt::QueuedConnection);
        }
        else
        {
            qDebug()<<"socketmsg"<<socketmsg;
            m_socket->soccheckstart(QString(socketmsg));
        }
    }
}

void MainWidget::sweepero()
{
    m_listwidget->clear();
    m_listwidget->insertItem(0,QString("扫码失败,请重新扫码"));
    m_line->clear();
    item = 0;
}

void MainWidget::testsuccess(QString teststring,QString mesmsg,QString sn )
{
    m_listwidget->insertItem(++item,QString(teststring)+QString("测试完成"));
    ++item;
    print_success(item,QString(teststring)+"测试结果：通过！");
    m_socket->soccheckstop();
    if(mesmsg.length()&&mesmsg.contains("erro")==false)
    {
        if (1==m_checked)
            m_mesmansger->startcheckmes(mesmsg,sn,1,"0","");
        else
            testcomplete();
    }
    if(mesmsg.contains("erro")==true)
    {
        mesmsg.remove(0,4);
        qDebug()<<"erromesmsg"<<mesmsg;
        if (1==m_checked)
            m_mesmansger->startcheckmes(mesmsg,sn,1,"1","");
        else
            testcomplete();
    }
}

void MainWidget::testfaile(QString teststring,QString mesmsg,QString sn,QString eromsg)
{
    g_erroflags = 1;
    m_listwidget->insertItem(++item,QString(teststring)+QString("测试完成"));
    ++item;
    print_ero(item,QString(teststring)+"测试结果：不通过！");
    if(mesmsg.length())
    {
        if (1==m_checked)
            m_mesmansger->startcheckmes(mesmsg,sn,1,"1",eromsg);
        else
            testcomplete();
    }
}

void MainWidget::testcomplete()
{
    m_listwidget->insertItem(++item,QString("本次测试已完成,请断电连接下一台设备"));
    if(g_erroflags == 0)
    {
        bannerwidget->setText("测试通过");
    }
    else
    {
        g_erroflags = 0;
        bannerwidget->setText("测试不通过");
        g_erronumber++;
    }
    qDebug()<<"g_testnumber"<<g_testnumber;
    if(g_testnumber == 0)
    {
        m_listwidget->insertItem(++item,QString("已测试设备总数：%1, 不合格总数：%2, 不合格率：%3%4").arg(0).arg(0).arg(0).arg("%"));
        nowline->clear();
        nowline->setText("0");
    }
    else
    {
        m_listwidget->insertItem(++item,QString("已测试设备总数：%1, 不合格总数：%2, 不合格率：%3%4").arg(g_testnumber).arg(g_erronumber).arg((g_erronumber*100/g_testnumber)).arg("%"));
        nowline->clear();
        sucline->clear();
        sucbililine->clear();
        historyline->clear();
        int n = g_testnumber-g_erronumber;
        nowline->setText(QString("%1").arg(g_testnumber));
        sucline->setText(QString("%1").arg(n));
        sucbililine->setText(QString("%1""%2").arg(n*100/g_testnumber).arg("%"));
        historyline->setText(QString("%1").arg(historynum));

        QSettings settings("testtool.ini", QSettings::IniFormat); // 当前目录的INI文件

        settings.beginGroup("HistoryNum");
        settings.setValue("num", QString("%1").arg(historynum));
        settings.endGroup();
    }
}

void MainWidget::MEScheck(QString name,QString socketmsg)
{
    if (1==m_checked)
    {
        if(QString::compare(name,"0:begin")==0)
        {
            m_listwidget->insertItem(++item,QString("MES系统验证成功"));
            m_mesmansger->stopcheckmes();
            if(QString::compare(socketmsg,"vlc") == 0)
            {
                m_vlc = new VlcManager();
                m_vlc->show();
                connect(m_vlc,SIGNAL(vlcmsg(QString)),this,SLOT(recevfromvlc()),Qt::QueuedConnection);
            }
            else if(socketmsg.contains("light") == true)
            {
                emit sendmsg("light::begin");
            }
            else if(QString::compare(socketmsg,"MSG::20") == 0)
            {
                m_socket->soccheckstart(QString(socketmsg));
                m_listwidget->insertItem(++item,"4G模组启动中，这可能花费30s-60s的时间，请等待......");
            }
            else
            {
                m_socket->soccheckstart(QString(socketmsg));
            }
        }
        if(QString::compare(name,"0:end")==0)
        {
            m_listwidget->insertItem(++item,QString("MES系统验证成功"));
            m_mesmansger->stopcheckmes();
            testcomplete();
        }
        if(QString::compare(name,"0:printer")==0)
        {
            QString linetxt = m_line->text();
            m_listwidget->insertItem(++item,QString("MES系统存储成功"));
            m_socket->sendmsgtoser(QString("MSG::08::")+QString(linetxt));
            m_mesmansger->stopcheckmes();
        }
        if(name.contains("1:printer")==true)
        {
            ++item;
            print_ero(item,QString("MES系统存储失败,")+name.remove(9));
            m_mesmansger->stopcheckmes();
        }
        if(name.contains("1:begin")==true)
        {
            bannerwidget->setText("测试不通过");
            ++item;
            print_ero(item,QString("MES系统验证失败,")+name.remove(0,7));
            m_mesmansger->stopcheckmes();

        }
        if(name.contains("1:end")==true)
        {
            bannerwidget->setText("测试不通过");
            ++item;
            print_ero(item,QString("MES系统验证失败,")+name.remove(0,5));
            m_mesmansger->stopcheckmes();
        }
        if(QString::compare(name,"MESEro")==0)
        {
            ++item;
            bannerwidget->setText("测试不通过");
            print_ero(item,"MES系统无响应，请联系管理员,断电重启设备");
            m_mesmansger->stopcheckmes();
        }
#if 0
        if(QString::compare(name,"0:save")==0)
        {
            m_listwidget->insertItem(++item,QString("4G值保存成功"));
            m_socket->sendmsgtoser("MSG::26");
            m_mesmansger->stopcheckmes();
        }
        if(QString::compare(name,"1:save")==0)
        {
            ++item;
            print_ero(item,QString("4G值保存失败,")+name.remove(0,5));
            m_mesmansger->stopcheckmes();
        }
#endif
    }
}

void MainWidget::socketero()
{
    bannerwidget->setText("测试不通过");
    m_socket->soccheckstop();
    ++item;
    print_ero(item,"访问设备超时,请检查网络是否正常");
}

void MainWidget::sendtoserver()
{
    qDebug()<<"start";
    m_socket->sendtoser();
}

void MainWidget::turntomain()
{
    closeui();
    FactoryTest *w = new FactoryTest();
    w->show();
    //    QString program = QApplication::applicationFilePath();
    //        QStringList arguments = QApplication::arguments();
    //        QString workingDirectory = QDir::currentPath();
    //        QProcess::startDetached(program, arguments, workingDirectory);
    //        QApplication::exit();
}

void MainWidget::recevfromvlc()
{
    qDebug()<<"11111111vlc";
    emit sendmsg("vlcclose");
}
