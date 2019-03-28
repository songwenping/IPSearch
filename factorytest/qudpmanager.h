#ifndef QUDPMANAGER_H
#define QUDPMANAGER_H
#include <QTimer>
#include <QUdpSocket>
#include <QMutexLocker>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif
enum recived
{
    REQUEST,
    REQUEST_OK_STEP1,
    REQUEST_OK_STEP2,
    REQUEST_OK_STEP3,
    REQUEST_ERO,
    SN_READ,
    SN_READ_OK,
    SN_READ_ERO,
    SN_WRITE,
    SN_WRITE_OK,
    SN_WRITE_ERO,
    MAC_READ,
    MAC_READ_OK,
    MAC_READ_ERO,
    MAC_WRITE,
    MAC_WRITE_OK,
    MAC_WRITE_ERO,
    SD_,
    SD_OK,
    SD_ERO,
    _4G,
    _4G_OK,
    _4G_ERO,
    WIFI_,
    WIFI_OK,
    WIFI_ERO,
    GPS,
    GPS_OK,
    GPS_ERO,
    LED_ON,
    LED_ON_EXEC,
    LED_OFF,
    LED_OFF_EXEC,
    KEY,
    KEY_OK,
    KEY_ERO,
    KEY_LED,
    KEY_LED_EXEC,
    STEP1_01_OVER,
    STEP1_02_OVER,
    STEP1_03_OVER,
    STEP2_01_OVER,
    STEP2_02_OVER,
    STEP2_03_OVER,
    SAVE_STATUS_OK,
    SAVE_STATUS_ERO,
    REQUEST_OK_STEP0,
    STEP0_01_OVER,
    STEP0_02_OVER,
    STEP0_03_OVER,
    SENSIOR_,
    SENSIOR_OK,
    SENSIOR_ERO,
    TF_SIM,
    TF_SIM_OK,
    TF_SIM_ERO,
    RECV_OK,
};

class QUdpManager:public QUdpSocket
{
    Q_OBJECT
public:
    explicit QUdpManager();
    ~QUdpManager();

    void sendtoser();
    void sendmsgtoser(QString string);
    void soccheckstart(QString string);
    void soccheckstop();

public:
    QUdpSocket *socket;
    static int g_socketvisit;
    static QString g_socketstr;
    QTimer *m_sockettimer;

private slots:
    void readPendingDatagrams();
    void socketcheck();

Q_SIGNALS:
    void receivedmsg(QString data);

};

#endif // QUDPMANAGER_H
