#ifndef TFCARDDLG_H
#define TFCARDDLG_H

#include <QLayout>
#include <QTableWidget>
#include <QTimer>
#include "widgets/mainwidget.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif

class TfcardDlg:public QObject
{
    Q_OBJECT
public:
    TfcardDlg(QWidget*parent=0,QWidget *tablewidget=0);
    ~TfcardDlg();

private:
    static QString  m_linetxt;
    QTimer *m_timer;
    MainWidget *m_widget;

private slots:
    void listenline();
    void recevfromser(QString string);

Q_SIGNALS:
    void receivedmsg(QString data);
    void mesdata(QString string);
};
#endif // TFCARDDLG_H
