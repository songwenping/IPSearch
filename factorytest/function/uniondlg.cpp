#include "uniondlg.h"
#include "widgets/newmessagebox.h"
#include "global.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif
UnionDlg::UnionDlg(QWidget *parent,QWidget *tablewidget):
    m_timer(new QTimer())
{
    m_widget = new MainWidget(0,tablewidget);
    connect(m_widget->m_line,SIGNAL(textChanged(QString)),this,SLOT(listenline()));
    connect(m_timer,SIGNAL(timeout()),this,SLOT(listenline()));
    connect(m_widget->m_socket,SIGNAL(receivedmsg(QString)),this,SLOT(recevfromser(QString)),Qt::QueuedConnection);
    connect(m_widget->m_mesmansger,SIGNAL(mesdata(QString)),this,SLOT(recevfromser(QString)),Qt::QueuedConnection);
}

UnionDlg::~UnionDlg()
{

}

QString UnionDlg::m_linetxt = "";
int UnionDlg::g_testtimes = 0;
QString UnionDlg::g_erro = "";


void UnionDlg::listenline()
{
    QString stringtxt = m_widget->m_line->text();
    m_timer->start(2000);
    if(g_testtimes == 1)
    {
        if(QString::compare(m_linetxt,stringtxt) == 0&&m_linetxt.length() == 16)
        {
            m_timer->stop();
            m_widget->sweepsucess("MSG::53","TEST_TF_SIM_TEST2",m_linetxt);
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
                m_widget->sweepsucess("MSG::53","ASSY_ETH_TF_TEST",m_linetxt);
            }
            if(g_testtimes == 3)
            {
                m_widget->sweepsucess("MSG::53","ASSY_ETH_TF_FINAL",m_linetxt);
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

void UnionDlg::recevfromser(QString string)
{
    m_widget->MEScheck(string,"MSG::53");
    if(QString::compare(string,"REQUEST_OK_STEP0")==0)
    {
        m_widget->init();
        g_testtimes = 1;
    }
    if(QString::compare(string,"REQUEST_OK_STEP1")==0)
    {
        m_widget->init();
        g_testtimes = 2;
    }
    if(QString::compare(string,"REQUEST_OK_STEP2")==0)
    {
        m_widget->init();
        g_testtimes = 3;
    }

    if(QString::compare(string,"TF_SIM_OK")==0)
    {
        m_widget->m_socket->soccheckstop();
        m_widget->testsuccess("SD","","");
        m_widget->m_socket->sendmsgtoser(QString("MSG::36"));
    }
    if(QString::compare(string,"TF_SIM_ERO")==0)
    {
        m_widget->m_socket->soccheckstop();
        g_erro = "SD,";
         m_widget->testfaile("SD","","","SD,");
        m_widget->m_socket->sendmsgtoser(QString("MSG::36"));
    }
    if(QString::compare(string,"KEY_LED_EXEC")==0)
    {
        m_widget->m_socket->soccheckstop();
        qDebug() <<"g_testtimes" <<g_testtimes;
        if(g_testtimes == 2 || g_testtimes ==3)
        {
            NewMessageBox *box = new NewMessageBox(0,"按钮LED灯测试结果","按钮LED灯测试是否通过",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
            if(box->exec() == QMessageBox::Yes)
            {
                m_widget->testsuccess("按钮LED灯","","");

            }
            else
            {
                m_widget->testfaile("按钮LED灯","","","SD");
                g_erro = g_erro+QString("button,");

            }
            m_widget->m_vlc = new VlcManager();
            m_widget->m_vlc->show();
            connect(m_widget->m_vlc,SIGNAL(vlcmsg(QString)),this,SLOT(recevfromser(QString)));
        }
        else
        {
            NewMessageBox *box = new NewMessageBox(0,"按钮LED灯测试结果","按钮LED灯测试是否通过",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
            if(box->exec() == QMessageBox::Yes)
            {
                if(QString::compare(g_erro,"")==0)
                {
                    m_widget->testsuccess("按钮LED灯","TEST_TF_SIM_TEST2",m_linetxt);
                    m_widget->m_socket->sendmsgtoser("MSG::48");
                }
                else
                {
                    m_widget->testsuccess("按钮LED灯","erroTEST_TF_SIM_TEST2",m_linetxt);
                    g_erro = "";
                }
            }
            else
            {
                m_widget->testfaile("按钮LED灯","TEST_TF_SIM_TEST2",m_linetxt,g_erro+"button,");
                g_erro="";

            }
        }

    }

    if(QString::compare(string,"vlcclose")==0)
    {
        NewMessageBox *box = new NewMessageBox(0,"ADC红外灯测试结果","ADC红外灯测试是否通过",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        if(box->exec() == QMessageBox::Yes)
        {
            if(QString::compare(g_erro,"")==0)
            {
                if(g_testtimes == 2)
                {
                    m_widget->testsuccess("ADC红外灯","ASSY_ETH_TF_TEST",m_linetxt);
                    m_widget->m_socket->sendmsgtoser("MSG::39");
                }
                if(g_testtimes == 3)
                {
                    m_widget->testsuccess("ADC红外灯","ASSY_ETH_TF_FINAL",m_linetxt);
                    m_widget->m_socket->sendmsgtoser("MSG::42");
                }
            }
            else
            {
                if(g_testtimes == 2)
                {
                    m_widget->testsuccess("ADC红外灯","erroASSY_ETH_TF_TEST",m_linetxt);
                }
                if(g_testtimes == 3)
                {
                    m_widget->testsuccess("ADC红外灯","erroASSY_ETH_TF_FINAL",m_linetxt);
                }
                g_erro = "";
            }

        }
        else
        {
            if(g_testtimes == 2)
            {
                m_widget->testfaile("ADC红外灯","ASSY_ETH_TF_TEST",m_linetxt,g_erro+"ADC");
                g_erro="";
            }
            if(g_testtimes == 3)
            {
                m_widget->testfaile("ADC红外灯","ASSY_ETH_TF_FINAL",m_linetxt,g_erro+"ADC");
                g_erro="";
            }

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
        m_widget->print_ero(m_widget->item,"设备保存测试结果失败，请处理，否则将会影响后面的测试");
    }

    if(QString::compare(string,"SocketEro")==0)
    {
        m_widget->socketero();
    }

}
