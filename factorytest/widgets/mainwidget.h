#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>
#include <QPushButton>
#include <QLayout>
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QTimer>
#include "qudpmanager.h"
#include "mesmanager.h"
#include "keypushbutton.h"
#include "vlcmanager.h"
#include "bannerwidget.h"
#include <QDialog>
#include <QSplitter>
namespace Ui {
class MainWidget;
}
class MainWidget : public QMainWindow
{
    Q_OBJECT

public:
    friend class SensorDlg;
    friend class TfcardDlg;
    friend class FocuslengthDlg;
    friend class Gps4GDlg;
    friend class LightDlg;
    friend class SensorfocusDlg;
    friend class SnprinterDlg;
    friend class UnionDlg;
    friend class HdimageDlg;
    friend class HdimageDlg;

    explicit MainWidget(QWidget *parent = 0,QWidget *tablewidget=0);
    ~MainWidget();

    void closeui();
    bool  eventFilter(QObject *target, QEvent *event);

private:
    void clearitem();
    void setrefont();
    void print_ero(int row, QString string);
    void print_success(int row, QString string);
    void deletelayout();
    int checkreply();
    void init();
    void sweepsucess(QString socketmsg,QString mesmsg,QString sn);//扫码成功
    void sweepero();//扫码失败
    void testsuccess(QString teststring,QString mesmsg,QString sn);//测试成功
    void testfaile(QString teststring,QString mesmsg,QString sn,QString eromsg);//测试失败
    void testcomplete();
    void MEScheck(QString name,QString socketmsg);//MES检查
    void socketero();

private:
    Ui::MainWidget *ui;
    QLabel *m_label;
    QLineEdit * m_line;
    QListWidget *m_listwidget;
    QListWidgetItem *itemer;
    Button *m_button;
    static QStringList g_itemlist;
    static int g_replytimes;
    static int item;
    static int g_vlc;
    static int g_testnumber;
    static int g_erronumber;
    static int g_erroflags;
    QWidget *tablewidget1;
    QUdpManager *m_socket;
    MesManager *m_mesmansger;
    QTimer *m_sockettimer;
    VlcManager *m_vlc;
    int m_checked;

    QSplitter  *m_spliter;
    QSplitter  *m_secondspliter;
    QTableWidget *top_widget;
    QTableWidget *down_widget;
    QTableWidget *left_widget;
    QTableWidget *right_widget;
    QLabel *stateslabel;
    QLabel *historylabel;
    QLabel *nowlabel;
    QLabel *suclabel;
    QLabel *sucbililabel;
    QLineEdit *historyline;
    QLineEdit *nowline;
    QLineEdit *sucline;
    QLineEdit *sucbililine;

    BannerWidget *bannerwidget;
    int historynum;


private slots:
    void sendtoserver();
    void turntomain();
    void recevfromvlc();
Q_SIGNALS:
    void sendmsg(QString string);


};

#endif // MAINWIDGET_H
