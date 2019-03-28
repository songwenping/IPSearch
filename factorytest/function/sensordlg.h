#ifndef SENSORDLG_H
#define SENSORDLG_H

#include <QObject>
#include <QLayout>
#include <QTableWidget>
#include <QTimer>
#include "mesmanager.h"
#include "widgets/mainwidget.h"
#include "factorytest.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif

class SensorDlg:public QObject
{
    Q_OBJECT
public:
    SensorDlg(QWidget*parent=0,QWidget *tablewidget=0);
    ~SensorDlg();

private:
    static QString  m_linetxt;
    static int visittime;
    QTimer *m_timer;
    MesManager *m_mesmansger;
    MainWidget *m_widget;


private slots:
    void listenline();
    void recevfromser(QString string);

Q_SIGNALS:
    void receivedmsg(QString data);
    void mesdata(QString string);
};

#endif // SENSORDLG_H
