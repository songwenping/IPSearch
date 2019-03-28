#include "sensordlg.h"
#include "global.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif

SensorDlg::SensorDlg(QWidget *parent,QWidget *tablewidget):
    m_timer(new QTimer()),
    m_mesmansger(new MesManager())
{
    m_widget = new MainWidget(0,tablewidget);

    connect(m_widget->m_line,SIGNAL(textChanged(QString)),this,SLOT(listenline()));
    connect(m_timer,SIGNAL(timeout()),this,SLOT(listenline()));
    connect(m_widget->m_socket,SIGNAL(receivedmsg(QString)),this,SLOT(recevfromser(QString)));
    connect(m_widget->m_mesmansger,SIGNAL(mesdata(QString)),this,SLOT(recevfromser(QString)));
    connect(m_widget,SIGNAL(sendmsg(QString)),this,SLOT(recevfromser(QString)));
}

SensorDlg::~SensorDlg()
{

}

QString SensorDlg::m_linetxt = "";
//int SensorDlg::item = 0;
int SensorDlg::visittime = 0;

void SensorDlg::listenline()
{
    QString stringtxt = m_widget->m_line->text();
    m_timer->start(2000);
    if(QString::compare(m_linetxt,stringtxt) == 0&&m_linetxt.length() == 16)
    {
        m_timer->stop();
        ++visittime;
        m_widget->sweepsucess("MSG::50","TEST_SENSOR_TEST1",m_linetxt);
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



void SensorDlg::recevfromser(QString string)
{
    m_widget->MEScheck(string,"MSG::50");

    if(QString::compare(string,"REQUEST_OK_STEP0")==0)
    {
        m_widget->init();
    }

    if(QString::compare(string,"SENSIOR_OK")==0)
    {
        m_widget->testsuccess("sensor板","TEST_SENSOR_TEST1",m_linetxt);
        m_widget->m_socket->soccheckstop();

    }

    if(QString::compare(string,"SENSIOR_ERO")==0)
    {
         m_widget->testfaile("sensor板","TEST_SENSOR_TEST1",m_linetxt,"sensor");
         m_widget->m_socket->soccheckstop();
    }

    if(QString::compare(string,"SocketEro")==0)
    {
        m_widget->socketero();
    }

}

