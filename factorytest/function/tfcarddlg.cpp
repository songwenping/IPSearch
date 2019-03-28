#include "tfcarddlg.h"
#include "global.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif

TfcardDlg::TfcardDlg(QWidget *parent,QWidget *tablewidget):
    m_timer(new QTimer())
{
    m_widget = new MainWidget(0,tablewidget);

    connect(m_widget->m_line,SIGNAL(textChanged(QString)),this,SLOT(listenline()));
    connect(m_timer,SIGNAL(timeout()),this,SLOT(listenline()));
    connect(m_widget->m_socket,SIGNAL(receivedmsg(QString)),this,SLOT(recevfromser(QString)));
    connect(m_widget->m_mesmansger,SIGNAL(mesdata(QString)),this,SLOT(recevfromser(QString)));
}

TfcardDlg::~TfcardDlg()
{

}


QString TfcardDlg::m_linetxt = "";

void TfcardDlg::listenline()
{
    QString stringtxt = m_widget->m_line->text();
    m_timer->start(2000);
    if(QString::compare(m_linetxt,stringtxt) == 0&&m_linetxt.length() == 16)
    {
        m_timer->stop();
        m_widget->sweepsucess("MSG::53","TEST_TF_SIM_TEST1",m_linetxt);
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

void TfcardDlg::recevfromser(QString string)
{
    m_widget->MEScheck(string,"MSG::53");
    if(QString::compare(string,"REQUEST_OK_STEP0")==0)
    {
        m_widget->init();
    }

    if(QString::compare(string,"TF_SIM_OK")==0)
    {
        m_widget->m_socket->soccheckstop();
         m_widget->testsuccess("tf板","TEST_TF_SIM_TEST1",m_linetxt);
    }
    if(QString::compare(string,"TF_SIM_ERO")==0)
    {
        m_widget->m_socket->soccheckstop();
        m_widget->testfaile("tf板","TEST_TF_SIM_TEST1",m_linetxt,"tf");
    }
    if(QString::compare(string,"SocketEro")==0)
    {
        m_widget->socketero();
    }

}

