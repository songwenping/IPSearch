#include "qudpmanager.h"
#include <QSettings>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif

QUdpManager::QUdpManager():
    socket(new QUdpSocket()),
    m_sockettimer(new QTimer())
{

    connect(socket, SIGNAL(readyRead()),this, SLOT(readPendingDatagrams()));
    connect(m_sockettimer, SIGNAL(timeout()),this, SLOT(socketcheck()));
}

QUdpManager::~QUdpManager()
{

}

int QUdpManager::g_socketvisit = 0;
QString QUdpManager::g_socketstr = "";

void QUdpManager::sendtoser()
{
    QString str = "MSG::00";
    QByteArray ba1 = str.toLatin1();
    socket->writeDatagram(ba1.data(), ba1.size(),QHostAddress::Broadcast, 32323);
}

void QUdpManager::sendmsgtoser(QString string)
{
    QString str;
    QSettings settings("testtool.ini", QSettings::IniFormat);
    str= settings.value("ServerIP/IP").toString();
    qDebug()<<"str"<<str;
    QByteArray ba1 = string.toLatin1();
    socket->writeDatagram(ba1.data(), ba1.size(),QHostAddress(str), 32323);
}

void QUdpManager::soccheckstart(QString string)
{
    m_sockettimer->start(10000);
    g_socketstr = string;
    sendmsgtoser(string);
}

void QUdpManager::soccheckstop()
{
    m_sockettimer->stop();
    g_socketvisit = 0;
}

void QUdpManager::readPendingDatagrams()
{
    QHostAddress host;
    quint16 port;
    QByteArray ba;

    if(socket->state() == QAbstractSocket::UnconnectedState)
    {
        qDebug()<< "socket->state()";
    }
    else
    {
        while(socket->hasPendingDatagrams())
        {

            ba.resize(socket->pendingDatagramSize());
            socket->readDatagram(ba.data(), ba.size(),&host,&port);
            host.toIPv4Address();
            QString ip=host.toString();
            ip.remove("::ffff:");
            ip.remove(" ");
            qDebug()<< "ip" <<ip;


            QSettings settings("testtool.ini", QSettings::IniFormat); // 当前目录的INI文件

            settings.beginGroup("ServerIP");
            settings.setValue("IP", ip);
            settings.endGroup();

            QString string = QString(ba.data());

            string.remove(0,5);
            QString revstr = string.remove(2,100);
            QString IMEI = QString(ba.data()).remove(0,9);
            recived tmp=(recived) revstr.toInt();
//            qDebug()<<"tmp = " <<tmp;
            switch(tmp)
            {
            case 0://REQUEST
                break;
            case 1://REQUEST_OK_STEP1
            {
                emit receivedmsg(QString("REQUEST_OK_STEP1"));
                break;
            }

            case 2://REQUEST_OK_STEP2
            {
                emit receivedmsg(QString("REQUEST_OK_STEP2"));
                break;
            }


            case 3://REQUEST_OK_STEP3,
                break;
            case 4://REQUEST_ERO,
                break;
            case 5:// SN_READ,
                break;
            case 6://SN_READ_OK,
                break;
            case 7://SN_READ_ERO,
                break;
            case 8:// SN_WRITE,
                break;
            case 9://SN_WRITE_OK,
            {
                emit receivedmsg(QString("SN_WRITE_OK"));
                break;
            }

            case 10://SN_WRITE_ERO,
            {
                emit receivedmsg(QString("SN_WRITE_ERO"));
                break;
            }

            case 11://MAC_READ,
                break;
            case 12://MAC_READ_OK,
                break;
            case 13://MAC_READ_ERO,
                break;
            case 14://MAC_WRITE,
                break;
            case 15:// MAC_WRITE_OK,
            {
                emit receivedmsg(QString("MAC_WRITE_OK"));
                break;
            }
            case 16://MAC_WRITE_ERO,
            {
                emit receivedmsg(QString("MAC_WRITE_ERO"));
                break;
            }
            case 17://SD_
                break;
            case 18://SD_OK
            {
                emit receivedmsg(QString("SD_OK"));
                break;
            }

            case 19://SD_ERO
            {
                emit receivedmsg(QString("SD_ERO"));
                break;
            }
            case 20:// _4G,
                break;
            case 21://_4G_OK,
            {
                emit receivedmsg(QString("_4G_OK"));
                break;
            }
            case 22:// _4G_ERO,
            {
                emit receivedmsg(QString("_4G_ERO"));
                break;
            }
            case 23://WIFI_,
                break;
            case 24:// WIFI_OK,
                break;
            case 25://WIFI_ERO,
                break;
            case 26://GPS,
                break;
            case 27://GPS_OK,
            {
                emit receivedmsg(QString("GPS_OK"));
                break;
            }
            case 28:// GPS_ERO,
            {
                emit receivedmsg(QString("GPS_ERO"));
                break;
            }
            case 29://LED_ON,
                break;
            case 30://LED_ON_EXEC,
                break;
            case 31://LED_OFF,
                break;
            case 32:// LED_OFF_EXEC,
                break;
            case 33://KEY,
                break;
            case 34://KEY_OK,
                break;
            case 35://KEY_ERO,
                break;
            case 36://KEY_LED,
                break;
            case 37://KEY_LED_EXEC,
            {
                emit receivedmsg(QString("KEY_LED_EXEC"));
                break;
            }

            case 38://STEP1_01_OVER,
                break;
            case 39:// STEP1_02_OVER,
                break;
            case 40://STEP1_03_OVER,
                break;
            case 41://STEP2_01_OVER,
                break;
            case 42://STEP2_02_OVER,
                break;
            case 43://STEP2_03_OVER,
                break;
            case 44://SAVE_STATUS_OK,
            {
                emit receivedmsg(QString("SAVE_STATUS_OK"));
                break;
            }
            case 45://SAVE_STATUS_ERO
            {
                emit receivedmsg(QString("SAVE_STATUS_ERO"));
                break;
            }

            case 46://REQUEST_OK_STEP0
                emit receivedmsg(QString("REQUEST_OK_STEP0"));
                break;

            case 47://STEP0_01_OVER,
                break;
            case 48://STEP0_02_OVER,
                break;
            case 49://STEP0_03_OVER,
                break;
            case 50://SENSIOR_
                break;
            case 51://SENSIOR_OK
            {
                emit receivedmsg(QString("SENSIOR_OK"));
                break;
            }

            case 52://SENSIOR_ERO
            {
                emit receivedmsg(QString("SENSIOR_ERO"));
                break;
            }

            case 53://TF_SIM
                break;
            case 54://TF_SIM_OK
            {
                emit receivedmsg(QString("TF_SIM_OK"));
                break;
            }

            case 55://TF_SIM_ERO
            {
                emit receivedmsg(QString("TF_SIM_ERO"));
                break;
            }
            case 56://RECV_OK
                break;
            case 57://GET_IMEI
                break;
            case 58://GET_IMEI_OK
            {
                emit receivedmsg(QString("GET_IMEI_OK")+IMEI);
                break;
            }
            case 59://GET_IMEI_ERO
            {
                emit receivedmsg(QString("GET_IMEI_ERO"));
                break;
            }
            default:
                break;

            }


        }
    }
}

void QUdpManager::socketcheck()
{
    ++g_socketvisit;
    if(g_socketvisit == 12)
    {
        soccheckstop();
        emit receivedmsg(QString("SocketEro"));
    }

}
