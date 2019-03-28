#ifndef MESMANAGER_H
#define MESMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QTimer>

class MesManager:public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit MesManager();
    ~MesManager();

    void sendhttpmsg(QString station,QString SN,int flag,QString status,QString ErrMsg);
    void startcheckmes(QString station,QString SN,int flag,QString status,QString ErrMsg);
    void stopcheckmes();

private:
    QNetworkRequest network;
    QNetworkReply *reply;
    QNetworkAccessManager *manager;
    QTimer *m_mestimer;
    static int g_visitetime;
    static QString g_station;
    static QString g_sn;
    static int g_flag;
    static QString g_status;
    static QString g_errmsg;

private slots:
    void replyFinished(QNetworkReply*);
    void checkmes();
Q_SIGNALS:
    void mesdata(QString string);

};

#endif // MESMANAGER_H
