#ifndef GPS4GDLG_H
#define GPS4GDLG_H

#include <QObject>
#include <QLayout>
#include <QTableWidget>
#include <QTimer>
#include "widgets/mainwidget.h"

class Gps4GDlg:public QObject
{
    Q_OBJECT
public:
    Gps4GDlg(QWidget *parent =0, QWidget *tablewidget=0);
    ~Gps4GDlg();

private:
    static QString  m_linetxt;
    static int g_testtimes;
    QTimer *m_timer;
    MainWidget *m_widget;
    QWidget *m_4gwidget;
    QLineEdit * line;

private slots:
    void listenline();
    void recevfromser(QString string);
    void sendgps();

Q_SIGNALS:
    void receivedmsg(QString data);
    void mesdata(QString string);
};

#endif // GPS4GDLG_H
