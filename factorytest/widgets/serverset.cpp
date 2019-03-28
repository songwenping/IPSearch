#include "serverset.h"
#include "ui_serverset.h"
#include "factorytest.h"
#include <QSettings>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif

ServerSet::ServerSet(QWidget *parent,QWidget *tablewidget) :
    QMainWindow(parent),
    m_iplabel(new QLabel("IP配置")),
    m_portlabel(new QLabel("端口配置")),
    m_ipline(new QLineEdit()),
    m_portline(new QLineEdit()),
    m_button(new QPushButton("保存")),
    ui(new Ui::ServerSet)
{
    ui->setupUi(this);
    m_iplabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    m_portlabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    m_checkbox = new QCheckBox(QString("开启无MES模式"));
    m_checkbox->setStyleSheet("font:bold 16px;font-family:微软雅黑;");
//    if(Qt::Checked == m_checkbox->checkState())
//    {
//        m_ipline->setEnabled(false);
//        m_portline->setEnabled(false);
//    }
//    if(Qt::Unchecked == m_checkbox->checkState())
//    {
//        m_ipline->setEnabled(true);
//        m_portline->setEnabled(true);
//    }
    m_layout = new QGridLayout(tablewidget);
    m_layout->addWidget(m_checkbox,1,1,2,1);
    m_layout->addWidget(m_iplabel,2,0,2,1);
    m_layout->addWidget(m_ipline, 2, 1,2,2);
    m_layout->addWidget(m_portlabel, 3,0,2,1);
    m_layout->addWidget(m_portline,3,1,2,2);
    m_layout->addWidget(m_button, 4, 1,2,1);
    //    pLayout->setSpacing(10);
    //    pLayout->setMargin(10);
    tablewidget->setLayout(m_layout);

    QSettings settings("testtool.ini", QSettings::IniFormat);

    QString gpsBaud = settings.value("MesIP/IP").toString();
    m_ipline->clear();
    m_ipline->insert(gpsBaud);

    QString gpsPort = settings.value("MesIP/port").toString();
    m_portline->clear();
    m_portline->insert(gpsPort);

    int checked  = settings.value("MesIP/checked").toInt();
    if(0 == checked)
    {
        m_ipline->setEnabled(false);
        m_portline->setEnabled(false);
        m_checkbox->setChecked(true);

    }
    else
    {
        m_ipline->setEnabled(true);
        m_portline->setEnabled(true);
        m_checkbox->setChecked(false);
    }
    connect(m_button,SIGNAL(clicked()),this,SLOT(save_ipport()));
    connect(m_checkbox,SIGNAL(stateChanged(int)),this,SLOT(statechange()));
}

ServerSet::~ServerSet()
{
    delete ui;
}

void ServerSet::save_ipport()
{
    QString set_ip = m_ipline->text();
    QString set_port = m_portline->text();
    if(QString::compare(set_ip,"")== 0||QString::compare(set_port,"")== 0)
    {

    }
    else
    {
        QSettings settings("testtool.ini", QSettings::IniFormat); // 当前目录的INI文件

        settings.beginGroup("MesIP");
        settings.setValue("IP", set_ip);
        settings.setValue("port", set_port);
        settings.endGroup();
    }

    foreach (QWidget *w, qApp->topLevelWidgets())
        if (QMainWindow* mainWin = qobject_cast<QMainWindow*>(w))
            mainWin->close();


    FactoryTest *w =new FactoryTest();
    w->show();
}

void ServerSet::statechange()
{
    if(Qt::Checked == m_checkbox->checkState())
    {
        m_ipline->setEnabled(false);
        m_portline->setEnabled(false);
         QSettings settings("testtool.ini", QSettings::IniFormat);
         settings.beginGroup("MesIP");
             settings.setValue("checked",QString("0"));
         settings.endGroup();
    }
    if(Qt::Unchecked == m_checkbox->checkState())
    {
        m_ipline->setEnabled(true);
        m_portline->setEnabled(true);
        QSettings settings("testtool.ini", QSettings::IniFormat);
        settings.beginGroup("MesIP");
            settings.setValue("checked",QString("1"));
        settings.endGroup();
    }
}
