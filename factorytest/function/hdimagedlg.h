#ifndef HDIMAGEDLG_H
#define HDIMAGEDLG_H

#include <QObject>
#include <QLayout>
#include <QTableWidget>
#include <QTimer>
#include "widgets/mainwidget.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif

class HdimageDlg:public QObject
{
    Q_OBJECT
public:
     friend class MainWidget;
    HdimageDlg(QWidget *parent =0, QWidget *tablewidget=0);
    ~HdimageDlg();

private:
    static QString  m_linetxt;
    static int g_testtimes ;
    QTimer *m_timer;
    MainWidget *m_widget;

private slots:
    void listenline();
    void recevfromser(QString string);
Q_SIGNALS:
    void receivedmsg(QString data);
    void mesdata(QString string);
    void vlcmsg(QString);
};

#endif // HDIMAGEDLG_H
