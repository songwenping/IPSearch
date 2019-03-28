#include "snprinterdlg.h"
#include "global.h"
#include <QSettings>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif

QString SnprinterDlg::m_linetxt = "";
int SnprinterDlg::item = 0;
int SnprinterDlg::visittime = 0;
int SnprinterDlg::g_testtimes = 0;
int SnprinterDlg::g_erro = 0;
QString SnprinterDlg::g_lineimei = "";

SnprinterDlg::SnprinterDlg(QWidget *parent,QWidget *tablewidget):
    m_sockettimer(new QTimer()),
    m_timer(new QTimer()),
    m_imeitimer(new QTimer())
{
    m_widget = new MainWidget(0,tablewidget);
    QSettings settings("testtool.ini", QSettings::IniFormat);
    m_checked  = settings.value("MesIP/checked").toInt();

    m_sockettimer->start(2000);
    //    connect(m_widget->m_line,SIGNAL(textChanged(QString)),this,SLOT(listenline()));
    //    connect(m_widget->m_imeiline,SIGNAL(textChanged(QString)),this,SLOT(listenimeiline()));
    //    connect(m_timer,SIGNAL(timeout()),this,SLOT(listenline()));
    //    connect(m_imeitimer,SIGNAL(timeout()),this,SLOT(listenimeiline()));
    //    connect(m_sockettimer,SIGNAL(timeout()),this,SLOT(sendtoserver()));
    //    connect(socket,SIGNAL(receivedmsg(QString)),this,SLOT(recevfromser(QString)));
    //    connect(m_mesmansger,SIGNAL(mesdata(QString)),this,SLOT(recevfromser(QString)));
    //    connect(m_widget->m_button,SIGNAL(clicked()),this,SLOT(turntomain()));
    connect(m_widget->m_line,SIGNAL(textChanged(QString)),this,SLOT(listenline()));
    connect(m_timer,SIGNAL(timeout()),this,SLOT(listenline()));
    connect(m_widget->m_socket,SIGNAL(receivedmsg(QString)),this,SLOT(recevfromser(QString)));
    connect(m_widget->m_mesmansger,SIGNAL(mesdata(QString)),this,SLOT(recevfromser(QString)));
}

SnprinterDlg::~SnprinterDlg()
{

}
void SnprinterDlg::listenline()
{
    QString stringtxt = m_widget->m_line->text();
    m_timer->start(2000);

    if(QString::compare(m_linetxt,stringtxt) == 0&&m_linetxt.length() == 16)
    {
        m_timer->stop();
        m_widget->sweepsucess("MSG::57","ASSY_IMEI_WRITE_SN",m_linetxt);


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

void SnprinterDlg::listenimeiline()
{
#if 0
    QString stringimei = m_widget->m_imeiline->text();
    m_imeitimer->start(2000);

    if(QString::compare(g_lineimei,stringimei) == 0&&g_lineimei.length() == 99)
    {
        qDebug()<<"g_lineimei"<<g_lineimei;
        m_imeitimer->stop();
        ++visittime;
        m_widget->m_imeiline->setPlaceholderText("");
        m_widget->m_imeiline->setEnabled(false);
        m_widget->m_listwidget->insertItem(++item,QString("主板IMEI号扫码完成"));
        QStringList list  = g_lineimei.split(";");
        QString Imei;
        for(int i=0;i<list.size();++i)
        {
            QString str = list.at(i);
            if(str.contains("IMEI"))
            {
                str.remove("IMEI:");
                str.remove(" ");
                Imei = str;
            }
        }
        qDebug()<<"Imei"<<Imei;
#if MES
        m_mesmansger->startcheckmes("ASSY_IMEI_WRITE_SN",m_linetxt,2,m_linetxt,Imei);

#else
        m_widget->m_listwidget->insertItem(++item,QString("标签打印完成"));
        m_widget->m_listwidget->insertItem(++item,QString("标签打印结果：成功"));
        socket->sendmsgtoser(QString("MSG::08::")+QString(m_linetxt));
#endif
    }
    else if(QString::compare(g_lineimei,stringimei) == 0&&g_lineimei.length()>0)
    {
        m_widget->m_listwidget->clear();
        m_widget->m_listwidget->insertItem(0,QString("扫码失败,请重新扫码"));
        m_widget->m_imeiline->clear();
    }
    else
    {
        g_lineimei = stringimei;
    }
#endif

}

void SnprinterDlg::sendtoserver()
{
    qDebug() <<"start";
    m_widget->m_socket->sendtoser();
}

void SnprinterDlg::recevfromser(QString string)
{
    m_widget->MEScheck(string,"MSG::57");
    if(QString::compare(string,"REQUEST_OK_STEP0")==0)
    {
        m_widget->clearitem();
        m_widget->m_line->clear();
        g_testtimes = 1;
        m_widget->m_line->setEnabled(true);
        m_widget->m_socket->sendmsgtoser(QString("MSG::56"));
        item = 0;
        m_widget->m_listwidget->clear();
        m_widget->m_line->setPlaceholderText("请扫描二维码");
    }
#if 0
    if(QString::compare(string,"0:begin")==0)
    {
        m_widget->m_listwidget->insertItem(++item,QString("MES系统验证成功"));
        m_widget->m_socket->sendmsgtoser(QString("MSG::57"));
        m_widget->m_mesmansger->stopcheckmes();
    }
    if(QString::compare(string,"0:end")==0)
    {
        m_widget->m_listwidget->insertItem(++item,QString("MES系统验证成功"));
        m_widget->m_mesmansger->stopcheckmes();
        m_widget->m_listwidget->insertItem(++item,QString("本次测试已完成,请断电连接下一台设备"));
        if(m_widget->g_testnumber == 0)
        {
            m_widget->m_listwidget->insertItem(++item,QString("已测试设备总数：%1, 不合格总数：%2, 不合格率：%3%4").arg(0).arg(0).arg(0).arg("%"));
        }
        else
        {
            m_widget->m_listwidget->insertItem(++item,QString("已测试设备总数：%1, 不合格总数：%2, 不合格率：%3%4").arg(m_widget->g_testnumber).arg(m_widget->g_erronumber).arg((m_widget->g_erronumber*100/m_widget->g_testnumber)).arg("%"));
        }
    }
    if(QString::compare(string,"0:printer")==0)
    {
        m_widget->m_listwidget->insertItem(++item,QString("MES系统存储成功"));
        m_widget->m_socket->sendmsgtoser(QString("MSG::08::")+QString(m_linetxt));
        m_widget->m_mesmansger->stopcheckmes();
    }
    if(string.contains("1:printer")==true)
    {
        ++item;
        m_widget->print_ero(item,QString("MES系统存储失败,")+string.remove(9));
        m_widget->m_mesmansger->stopcheckmes();
    }
    if(string.contains("1:begin")==true)
    {
        ++item;
        m_widget->print_ero(item,QString("MES系统验证失败,")+string.remove(6));
        m_widget->m_mesmansger->stopcheckmes();

    }
    if(string.contains("1:end")==true)
    {
        ++item;
        m_widget->print_ero(item,QString("MES系统验证失败,")+string.remove(4));
        m_widget->m_mesmansger->stopcheckmes();
    }
    if(QString::compare(string,"MESEro")==0)
    {
        ++item;
        m_widget->print_ero(item,"MES系统无响应，请联系管理员,断电重启设备");
        m_widget->m_mesmansger->stopcheckmes();
    }
#endif
    if(QString::compare(string,"SN_WRITE_OK")==0)
    {
        qDebug() <<"51";
        m_widget->m_listwidget->insertItem(++item,QString("SN烧录完成"));
        ++item;
        m_widget->print_success(item,"SN烧录结果：通过！");
        //        if(g_erro == 1)
        //        {
        //            m_widget->g_erronumber++;
        //            g_erro = 0;
        //            ++item;
        //            m_widget->print_success(item,"本次测试最终结果：不通过！");
        //            m_mesmansger->startcheckmes("TEST_TF_SIM_TEST1",m_linetxt,1,"1","print");
        //        }
        //        else
        {
            ++item;
            m_widget->print_success(item,"本次测试最终结果：通过！");
            if(1 == m_checked)
            {
                m_widget->m_mesmansger->startcheckmes("ASSY_IMEI_WRITE_SN",m_linetxt,1,"0","");
            }
        }
        if(0 == m_checked)
            m_widget->m_listwidget->insertItem(++item,QString("本次测试已完成,请断电连接下一台设备"));

    }
    if(QString::compare(string,"SN_WRITE_ERO")==0)
    {
        qDebug() <<"52";
        m_widget->m_listwidget->insertItem(++item,QString("SN烧录完成"));
        ++item;
        m_widget->print_ero(item,"SN烧录结果：失败！");
        ++item;
        m_widget->print_ero(item,"本次测试最终结果：不通过！");
        if(1 == m_checked)
            if(g_erro == 1)
            {
                m_widget->m_mesmansger->startcheckmes("ASSY_IMEI_WRITE_SN",m_linetxt,1,"1","print,write");
                g_erro=0;
            }
            else
            {
                m_widget->m_mesmansger->startcheckmes("ASSY_IMEI_WRITE_SN",m_linetxt,1,"1","write");
            }

        else
            m_widget->m_listwidget->insertItem(++item,QString("本次测试已完成,请断电连接下一台设备"));

    }
    if(string.contains("GET_IMEI_OK")==true)
    {
        m_widget->m_listwidget->insertItem(++item,QString("获取IMEI号完成"));
        ++item;
        m_widget->print_success(item,"获取IMEI号结果：通过！");
        QString Imei = string.remove("GET_IMEI_OK");
        qDebug()<<"Imei"<<Imei;
        m_widget->m_listwidget->insertItem(++item,QString("获取IMEI号：%1").arg(Imei));
        if(Imei.length() != 15)
        {
            ++item;
            m_widget->print_ero(item,"IMEI号有误，请重新断电重启设备进行重新获取");
        }
        else
        {
            if(1== m_checked)
            {
                m_widget->m_mesmansger->startcheckmes("ASSY_IMEI_WRITE_SN",m_linetxt,2,Imei,"");
            }
        }

        if(0== m_checked)
        {
            m_widget->m_listwidget->insertItem(++item,QString("标签打印完成"));
            m_widget->m_listwidget->insertItem(++item,QString("标签打印结果：成功"));
            m_widget->m_socket->sendmsgtoser(QString("MSG::08::")+QString(m_linetxt));
        }
    }
    if(QString::compare(string,"GET_IMEI_ERO")==0)
    {
        g_erro = 1;
        m_widget->m_listwidget->insertItem(++item,QString("获取IMEI号完成"));
        ++item;
        m_widget->print_ero(item,"获取IMEI号结果：失败！");
        if(m_checked == 1)
        {
            m_widget->m_mesmansger->startcheckmes("ASSY_IMEI_WRITE_SN",m_linetxt,1,"1","print");
        }
        else
        {
            m_widget->m_listwidget->insertItem(++item,QString("本次测试已完成,请断电连接下一台设备"));
        }
    }

}
void SnprinterDlg::turntomain()
{
    m_widget->closeui();
    FactoryTest *w = new FactoryTest();
    w->show();

}
