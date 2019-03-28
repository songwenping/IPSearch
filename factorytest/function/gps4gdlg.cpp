#include "gps4gdlg.h"
#include "widgets/newmessagebox.h"
#include "global.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif

Gps4GDlg::Gps4GDlg(QWidget *parent, QWidget *tablewidget):
    m_timer(new QTimer())
{
    m_widget = new MainWidget(0,tablewidget);

    connect(m_widget->m_line,SIGNAL(textChanged(QString)),this,SLOT(listenline()));
    connect(m_timer,SIGNAL(timeout()),this,SLOT(listenline()));
    connect(m_widget->m_socket,SIGNAL(receivedmsg(QString)),this,SLOT(recevfromser(QString)),Qt::QueuedConnection);
    connect(m_widget->m_mesmansger,SIGNAL(mesdata(QString)),this,SLOT(recevfromser(QString)),Qt::QueuedConnection);
}

Gps4GDlg::~Gps4GDlg()
{

}

QString Gps4GDlg::m_linetxt = "";
int Gps4GDlg::g_testtimes = 0;

void Gps4GDlg::listenline()
{
    QString stringtxt = m_widget->m_line->text();
    m_timer->start(2000);
    if(g_testtimes == 1)
    {
        if(QString::compare(m_linetxt,stringtxt) == 0&&m_linetxt.length() == 16)
        {
            m_timer->stop();
            m_widget->sweepsucess("MSG::20","TEST_4G_SIM_TEST1",m_linetxt);
        }
        else if(QString::compare(m_linetxt,stringtxt) == 0&&m_linetxt.length()>0)
        {
            m_widget->sweepero();
        }
        else
        {
            m_linetxt = stringtxt;
        }
    }
    else
    {
        if(QString::compare(m_linetxt,stringtxt) == 0&&m_linetxt.length() == 16)
        {
            m_timer->stop();
            if(g_testtimes == 2)
            {
                m_widget->sweepsucess("MSG::20","ASSY_4G_TEST2",m_linetxt);
            }
            if(g_testtimes == 3)
            {
                m_widget->sweepsucess("MSG::20","ASSY_4G_FINAL",m_linetxt);
            }
        }
        else if(QString::compare(m_linetxt,stringtxt) == 0&&m_linetxt.length()>0)
        {
            m_widget->sweepero();
        }
        else
        {
            m_linetxt = stringtxt;
        }
    }
}


void Gps4GDlg::recevfromser(QString string)
{
    m_widget->MEScheck(string,"MSG::20");
    if(QString::compare(string,"REQUEST_OK_STEP0")==0)
    {
        g_testtimes = 1;
        m_widget->init();
    }
    if(QString::compare(string,"REQUEST_OK_STEP1")==0)
    {
        g_testtimes = 2;
        m_widget->init();
    }
    if(QString::compare(string,"REQUEST_OK_STEP2")==0)
    {
        g_testtimes = 3;
        m_widget->init();
    }
    if(QString::compare(string,"_4G_OK")==0)
    {
        m_widget->m_socket->soccheckstop();
        qDebug() <<"51";
        NewMessageBox *box = new NewMessageBox(0,"4G测试","4G测试是否通过",QMessageBox::Yes|QMessageBox::No,QMessageBox::Ok);
        if(box->exec() == QMessageBox::Yes)
        {
            if(g_testtimes == 1)
            {
                m_widget->testsuccess("4G","TEST_4G_SIM_TEST1",m_linetxt);
                m_widget->m_socket->sendmsgtoser("MSG::49");
            }
            if(g_testtimes == 2)
            {
                m_widget->testsuccess("4G","ASSY_4G_TEST2",m_linetxt);
                m_widget->m_socket->sendmsgtoser("MSG::40");
            }
            if(g_testtimes == 3)
            {
                m_widget->testsuccess("4G","ASSY_4G_FINAL",m_linetxt);
                m_widget->m_socket->sendmsgtoser("MSG::43");
            }
        }
        else
        {
            if(g_testtimes == 1)
            {
                m_widget->testfaile("4G","TEST_4G_SIM_TEST1",m_linetxt,"4G");
            }
            if(g_testtimes == 2)
            {
                m_widget->testfaile("4G","ASSY_4G_TEST2",m_linetxt,"4G");
            }
            if(g_testtimes == 3)
            {
                m_widget->testfaile("4G","ASSY_4G_FINAL",m_linetxt,"4G");
            }
        }
#if 0
        m_4gwidget = new QWidget();
        QHBoxLayout *lay = new QHBoxLayout(m_widget);
        QLabel *label = new QLabel("请输入综测仪上的数字：");
        line  = new QLineEdit();
        QPushButton *button  =new QPushButton("确认保存");
        lay->addWidget(label);
        lay->addWidget(line);
        lay->addWidget(button);
        m_4gwidget->setLayout(lay);
        m_4gwidget->setWindowFlags(Qt::FramelessWindowHint);
        m_4gwidget->resize(500,120);
        m_4gwidget->setStyleSheet("background-color:turquoise;");
        line->setStyleSheet("background-color:white;");
        m_4gwidget->setWindowModality(Qt::ApplicationModal);
        m_4gwidget->show();
        connect(button,SIGNAL(clicked()),this,SLOT(sendgps()));
#endif
    }
    if(QString::compare(string,"_4G_ERO")==0)
    {
        m_widget->m_socket->soccheckstop();
        if(g_testtimes == 1)
        {
            qDebug()<<"4gggggggggg1";
            m_widget->testfaile("4G","TEST_4G_SIM_TEST1",m_linetxt,"4G");
        }
        if(g_testtimes == 2)
        {
            m_widget->testfaile("4G","ASSY_4G_TEST2",m_linetxt,"4G");
        }
        if(g_testtimes == 3)
        {
            m_widget->testfaile("4G","ASSY_4G_FINAL",m_linetxt,"4G");
        }
        ++(m_widget->item);
        m_widget->print_ero(m_widget->item,"4G模块启动失败,请重启设备或测试下一台设备,4G测试结果：不通过");
    }
    if(QString::compare(string,"GPS_OK")==0)
    {
        if(g_testtimes == 1)
        {
            m_widget->testsuccess("GPS","TEST_4G_SIM_TEST1",m_linetxt);
            m_widget->m_socket->sendmsgtoser("MSG::49");
        }
        if(g_testtimes == 2)
        {
            m_widget->testsuccess("GPS","ASSY_4G_TEST2",m_linetxt);
            m_widget->m_socket->sendmsgtoser("MSG::40");
        }
        if(g_testtimes == 3)
        {
            m_widget->testsuccess("GPS","ASSY_4G_FINAL",m_linetxt);
            m_widget->m_socket->sendmsgtoser("MSG::43");
        }
    }
    if(QString::compare(string,"GPS_ERO")==0)
    {
        if(g_testtimes == 1)
        {
            m_widget->testfaile("GPS","TEST_4G_SIM_TEST1",m_linetxt,"GPS");
        }
        if(g_testtimes == 2)
        {
            m_widget->testfaile("GPS","ASSY_4G_TEST2",m_linetxt,"GPS");
        }
        if(g_testtimes == 3)
        {
            m_widget->testfaile("GPS","ASSY_4G_FINAL",m_linetxt,"GPS");
        }
    }

    if(QString::compare(string,"SAVE_STATUS_OK")==0)
    {
        m_widget->m_socket->soccheckstop();
        m_widget->m_listwidget->insertItem(++m_widget->item,QString("设备保存测试结果成功"));
    }
    if(QString::compare(string,"SAVE_STATUS_ERO")==0)
    {
        m_widget->m_socket->soccheckstop();
        ++m_widget->item;
        m_widget->print_ero(m_widget->item,"设备保存");
    }

    if(QString::compare(string,"SocketEro")==0)
    {
        m_widget->socketero();
    }


}

void Gps4GDlg::sendgps()
{
    QString string  =line->text();
    m_4gwidget->close();
    if(g_testtimes == 1)
    {
        m_widget->m_mesmansger->startcheckmes("TEST_4G_SIM_TEST1",m_linetxt,3,string,"");
    }
    if(g_testtimes == 2)
    {
        m_widget->m_mesmansger->startcheckmes("ASSY_4G_TEST2",m_linetxt,3,string,"");
    }
    if(g_testtimes == 3)
    {
        m_widget->m_mesmansger->startcheckmes("ASSY_4G_FINAL",m_linetxt,3,string,"");
    }
}
