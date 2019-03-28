#include "lightdlg.h"
#include "widgets/newmessagebox.h"
#include "global.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif

LightDlg::LightDlg(QWidget*parent,QWidget *tablewidget):
    m_timer(new QTimer())
{
    m_widget = new MainWidget(0,tablewidget);

    connect(m_widget->m_line,SIGNAL(textChanged(QString)),this,SLOT(listenline()));
    connect(m_timer,SIGNAL(timeout()),this,SLOT(listenline()));
    connect(m_widget->m_socket,SIGNAL(receivedmsg(QString)),this,SLOT(recevfromser(QString)),Qt::QueuedConnection);
    connect(m_widget->m_mesmansger,SIGNAL(mesdata(QString)),this,SLOT(recevfromser(QString)),Qt::QueuedConnection);
    connect(m_widget,SIGNAL(sendmsg(QString)),this,SLOT(recevfromser(QString)),Qt::QueuedConnection);
}

LightDlg::~LightDlg()
{

}

QString LightDlg::m_linetxt = "";

void LightDlg::listenline()
{
    QString stringtxt = m_widget->m_line->text();
    m_timer->start(2000);
    if(QString::compare(m_linetxt,stringtxt) == 0&&m_linetxt.length() == 16)
    {
        m_timer->stop();
        m_widget->sweepsucess("","TEST_IR_TEST",m_linetxt);
#if MES
#else
        NewMessageBox *box = new NewMessageBox(0,"补光灯板测试结果","补光灯板测试是否通过",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        if(box->exec() == QMessageBox::Yes)
        {
            m_widget->testsuccess("补光灯板","TEST_IR_TEST",m_linetxt);
        }
        else
        {
            m_widget->testfaile("补光灯板","TEST_IR_TEST",m_linetxt,"light");
        }
#endif
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


void LightDlg::recevfromser(QString string)
{
    m_widget->MEScheck(string,"light"+m_linetxt);
    if(QString::compare(string,"REQUEST_OK_STEP0")==0)
    {
        m_widget->init();
    }
    if(QString::compare(string,"light::begin")==0)
    {
        NewMessageBox *box = new NewMessageBox(0,"补光灯板测试结果","补光灯板测试是否通过",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        if(box->exec() == QMessageBox::Yes)
        {
            m_widget->testsuccess("补光灯板","TEST_IR_TEST",m_linetxt);
        }
        else
        {
            m_widget->testfaile("补光灯板","TEST_IR_TEST",m_linetxt,"light");
        }
    }
#if 0
    if(QString::compare(string,"0:begin")==0)
    {
        m_widget->m_listwidget->insertItem(++(m_widget->item),QString("MES系统验证成功"));
        m_widget->m_mesmansger->stopcheckmes();
        NewMessageBox *box = new NewMessageBox(0,"补光灯板测试结果","补光灯板测试是否通过",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        if(box->exec() == QMessageBox::Yes)
        {
            m_widget->testsuccess("补光灯板","TEST_IR_TEST",m_linetxt);
        }
        else
        {
            m_widget->g_erronumber++;
            m_widget->testfaile("补光灯板","TEST_IR_TEST",m_linetxt,"light");
        }
    }
    if(QString::compare(string,"0:end")==0)
    {
        qDebug()<<"light";
         m_widget->m_mesmansger->stopcheckmes();
        m_widget->m_listwidget->insertItem(++(m_widget->item),QString("MES系统验证成功"));
        m_widget->m_listwidget->insertItem(++(m_widget->item),QString("本次测试已完成,请断电连接下一台设备"));
    }
    if(string.contains("1:begin")==true)
    {
        ++(m_widget->item);
        m_widget->print_ero((m_widget->item),QString("MES系统验证失败,")+string.remove(6));
        m_widget->m_mesmansger->stopcheckmes();

    }
    if(string.contains("1:end")==true)
    {
        ++(m_widget->item);
        m_widget->print_ero((m_widget->item),QString("MES系统验证失败,")+string.remove(4));
        m_widget->m_mesmansger->stopcheckmes();
    }
    if(QString::compare(string,"MESEro")==0)
    {
        ++(m_widget->item);
        m_widget->print_ero((m_widget->item),"MES系统无响应，请联系管理员,断电重启设备");
        m_widget->m_mesmansger->stopcheckmes();
        m_widget->m_line->clear();
    }
#endif
    if(QString::compare(string,"SocketEro")==0)
    {
        m_widget->socketero();
    }

}
