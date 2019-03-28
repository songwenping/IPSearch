#ifndef SNPRINTERDLG_H
#define SNPRINTERDLG_H

#include <QObject>
#include <QLayout>
#include <QTableWidget>
#include <QTimer>
#include "qudpmanager.h"
#include "mesmanager.h"
#include "widgets/snprinterwidget.h"
#include "factorytest.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif

class SnprinterDlg:public QObject
{
    Q_OBJECT
public:
    SnprinterDlg(QWidget*parent=0,QWidget *tablewidget=0);
    ~SnprinterDlg();

private:
    static QString  m_linetxt;
    static QString  g_lineimei;
    static int item;
    static int visittime;
    static int g_testtimes;
    static int g_erro;
    QTimer *m_timer;
    QTimer *m_sockettimer;
    QTimer *m_imeitimer;
    MainWidget *m_widget;
    int m_checked;

private slots:
    void listenline();
    void listenimeiline();
    void sendtoserver();
    void recevfromser(QString string);
    void turntomain();

Q_SIGNALS:
    void receivedmsg(QString data);
    void mesdata(QString string);
};

#endif // SNPRINTERDLG_H
