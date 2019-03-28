#ifndef UNIONDLG_H
#define UNIONDLG_H

#include <QObject>
#include <QLayout>
#include <QTableWidget>
#include <QTimer>
#include "widgets/mainwidget.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif

class UnionDlg:public QObject
{
    Q_OBJECT
public:
    UnionDlg(QWidget*parent=0,QWidget *tablewidget=0);
    ~UnionDlg();

private:
    static QString  m_linetxt;
    static int g_testtimes;
    static QString g_erro;
    QTimer *m_timer;
    MainWidget* m_widget;

private slots:
    void listenline();
    void recevfromser(QString string);

Q_SIGNALS:
    void receivedmsg(QString data);
    void mesdata(QString string);
};

#endif // UNIONDLG_H
