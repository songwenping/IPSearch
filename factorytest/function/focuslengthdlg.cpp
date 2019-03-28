#include "focuslengthdlg.h"
#include "widgets/newmessagebox.h"
#include "global.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif


QString FocuslengthDlg::m_linetxt = "";

FocuslengthDlg::FocuslengthDlg(QWidget*parent,QWidget *tablewidget):
    m_timer(new QTimer())
{
    m_widget = new MainWidget(0,tablewidget);

    connect(m_widget->m_line,SIGNAL(textChanged(QString)),this,SLOT(listenline()));
    connect(m_timer,SIGNAL(timeout()),this,SLOT(listenline()));
    connect(m_widget->m_socket,SIGNAL(receivedmsg(QString)),this,SLOT(recevfromser(QString)),Qt::QueuedConnection);
    connect(m_widget->m_mesmansger,SIGNAL(mesdata(QString)),this,SLOT(recevfromser(QString)),Qt::QueuedConnection);
    connect(m_widget,SIGNAL(sendmsg(QString)),this,SLOT(recevfromser(QString)),Qt::QueuedConnection);

}

FocuslengthDlg::~FocuslengthDlg()
{

}
void FocuslengthDlg::listenline()
{
    QString stringtxt = m_widget->m_line->text();
    m_timer->start(2000);
    if(QString::compare(m_linetxt,stringtxt) == 0&&m_linetxt.length() == 16)
    {
        m_timer->stop();
        m_widget->sweepsucess("vlc","ASSY_FOCUS_TEST2",m_linetxt);

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


void FocuslengthDlg::recevfromser(QString string)
{
    m_widget->MEScheck(string,"vlc");
    if(QString::compare(string,"REQUEST_OK_STEP0")==0)
    {
        m_widget->init();
    }

    if(QString::compare(string,"vlcclose")==0)
    {
        NewMessageBox *box = new NewMessageBox(0,"sensor焦距check测试结果","sensor焦距check测试是否通过",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        if(box->exec() == QMessageBox::Yes)
        {
            m_widget->testsuccess("sensor焦距check","ASSY_FOCUS_TEST2",m_linetxt);
        }
        else
        {
            m_widget->testfaile("sensor焦距check","ASSY_FOCUS_TEST2",m_linetxt,"check");
        }
    }
    if(QString::compare(string,"SAVE_STATUS_OK")==0)
    {
        m_widget->m_listwidget->insertItem(++(m_widget->item),QString("设备保存测试结果成功"));
    }
    if(QString::compare(string,"SAVE_STATUS_ERO")==0)
    {
        ++(m_widget->item);
        m_widget->print_ero((m_widget->item),"设备保存测试结果失败，请处理，否则将会影响后面的测试");
    }
    if(QString::compare(string,"SocketEro")==0)
    {
        m_widget->socketero();
    }

}

