#include "hdimagedlg.h"
#include "widgets/newmessagebox.h"
#include "global.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif
HdimageDlg::HdimageDlg(QWidget *parent, QWidget *tablewidget):
    m_timer(new QTimer())
{
    m_widget = new MainWidget(0,tablewidget);

    connect(m_widget->m_line,SIGNAL(textChanged(QString)),this,SLOT(listenline()));
    connect(m_timer,SIGNAL(timeout()),this,SLOT(listenline()));
    connect(m_widget->m_socket,SIGNAL(receivedmsg(QString)),this,SLOT(recevfromser(QString)));
    connect(m_widget->m_mesmansger,SIGNAL(mesdata(QString)),this,SLOT(recevfromser(QString)));
    connect(m_widget,SIGNAL(sendmsg(QString)),this,SLOT(recevfromser(QString)));


}

HdimageDlg::~HdimageDlg()
{

}

QString HdimageDlg::m_linetxt = "";
int HdimageDlg::g_testtimes = 0;

void HdimageDlg::listenline()
{
    QString stringtxt = m_widget->m_line->text();
    m_timer->start(2000);
    if(g_testtimes == 1)
    {
        if(QString::compare(m_linetxt,stringtxt) == 0&&m_linetxt.length() == 16)
        {
            m_timer->stop();
            m_widget->sweepsucess("vlc","TEST_SENSOR_TEST2",m_linetxt);
#if MES

#else
            m_widget->m_vlc = new VlcManager();
            m_widget->m_vlc->show();
            connect(m_widget->m_vlc,SIGNAL(vlcmsg(QString)),this,SLOT(recevfromser(QString)));
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
    else
    {

        if(QString::compare(m_linetxt,stringtxt) == 0&&m_linetxt.length() == 16)
        {
            m_timer->stop();
            if(g_testtimes == 2)
            {
                m_widget->sweepsucess("vlc","ASSY_HD_TEST",m_linetxt);
#if MES
#else
            m_widget->m_vlc = new VlcManager();
            m_widget->m_vlc->show();
            connect(m_widget->m_vlc,SIGNAL(vlcmsg(QString)),this,SLOT(recevfromser(QString)));
#endif

            }
            if(g_testtimes == 3)
            {
                m_widget->sweepsucess("vlc","ASSY_HD_FINAL",m_linetxt);
#if MES
#else
            m_widget->m_vlc = new VlcManager();
            m_widget->m_vlc->show();
            connect(m_widget->m_vlc,SIGNAL(vlcmsg(QString)),this,SLOT(recevfromser(QString)));
#endif

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

void HdimageDlg::recevfromser(QString string)
{
    m_widget->MEScheck(string,"vlc");
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
    if(QString::compare(string,"vlcclose")==0)
    {
        NewMessageBox *box = new NewMessageBox(0,"图像高清测试结果","图像高清测试是否通过",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        if(box->exec() == QMessageBox::Yes)
        {
            if(g_testtimes == 1)
            {
                m_widget->m_socket->sendmsgtoser("MSG::47");
                m_widget->testsuccess("图像高清","TEST_SENSOR_TEST2",m_linetxt);
            }
            if(g_testtimes == 2)
            {
                m_widget->m_socket->sendmsgtoser("MSG::38");
                m_widget->testsuccess("图像高清","ASSY_HD_TEST",m_linetxt);
            }
            if(g_testtimes == 3)
            {
                m_widget->m_socket->sendmsgtoser("MSG::41");
                m_widget->testsuccess("图像高清","ASSY_HD_FINAL",m_linetxt);

            }
        }
        else
        {
            if(g_testtimes == 1)
            {
                m_widget->testfaile("图像高清","TEST_SENSOR_TEST2",m_linetxt,"image");
            }
            if(g_testtimes == 2)
            {
                m_widget->testfaile("图像高清","ASSY_HD_TEST",m_linetxt,"image");
            }
            if(g_testtimes == 3)
            {
                m_widget->testfaile("图像高清","ASSY_HD_FINAL",m_linetxt,"image");
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
