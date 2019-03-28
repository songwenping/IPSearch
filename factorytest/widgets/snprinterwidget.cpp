#include "snprinterwidget.h"
#include "ui_snprinterwidget.h"
#include <QDebug>
#include <QSettings>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif

SnPrinterWidget::SnPrinterWidget(QWidget *parent,QWidget *tablewidget) :
    QMainWindow(parent),
    itemer(new QListWidgetItem),
    ui(new Ui::SnPrinterWidget)
{
    ui->setupUi(this);

    m_label = new QLabel("请扫描整机二维码");
    m_imeilabel = new QLabel("获取主板IMEI号");
    m_line = new QLineEdit();
    m_imeiline = new QLineEdit();
    m_listwidget = new QListWidget();
    m_button=new Button(":/png/image/return.png", 1);

    itemer->setTextColor(QColor(178,34,34));
    m_listwidget->setObjectName("list");


    QGridLayout* layout = new QGridLayout(tablewidget);
    layout->addWidget(m_label,0,0,1,1);
    layout->addWidget(m_line,0,1,1,layout->columnCount());
    layout->addWidget(m_button,0,layout->columnCount(),1,1);
    layout->addWidget(m_imeilabel,1,0,1,1);
    layout->addWidget(m_imeiline,1,1,1,layout->columnCount());
    layout->addWidget(m_listwidget,2,0,layout->rowCount(),layout->columnCount());
    tablewidget->setLayout(layout);
    m_line->setEnabled(false);
    m_line->setPlaceholderText("");
    m_imeiline->setEnabled(false);
    m_imeiline->setPlaceholderText("");
    m_line->installEventFilter(this);
}
QStringList SnPrinterWidget::g_itemlist;
int SnPrinterWidget::g_testnumber = 0;
int SnPrinterWidget::g_erronumber = 0;

SnPrinterWidget::~SnPrinterWidget()
{
    delete ui;
}

void SnPrinterWidget::closeui()
{
    clearitem();
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (QMainWindow* mainWin = qobject_cast<QMainWindow*>(w))
            mainWin->close();
}

bool SnPrinterWidget::eventFilter(QObject *target, QEvent *event)
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

void SnPrinterWidget::clearitem()
{
    g_itemlist.clear();
    if(!(g_itemlist.isEmpty()))
    {
        for(int i=0;i<g_itemlist.size();++i)
        {
            QString str = g_itemlist.at(i);
             qDebug()<<"str"<<str;
            int j=str.toInt();
            m_listwidget->takeItem(j);
        }
        g_itemlist.clear();
    }
}

void SnPrinterWidget::print_ero(int row, QString string)
{
    QListWidgetItem* itemer = new QListWidgetItem;
    itemer->setTextColor(QColor(178,34,34));
    itemer->setText(QString(string));
    m_listwidget->insertItem(row,itemer);
    g_itemlist.append(QString::number(row));
}

void SnPrinterWidget::print_success(int row, QString string)
{
    QListWidgetItem* itemer = new QListWidgetItem;
    itemer->setTextColor(QColor(0,128,0));
    itemer->setText(QString(string));
    m_listwidget->insertItem(row,itemer);
    g_itemlist.append(QString::number(row));
}
