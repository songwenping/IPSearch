#include "mesmanager.h"
#include <QSettings>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonObject>
#include <iostream>
#include <QTextCodec>
#include <QtScript/QScriptEngine>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif



using namespace std;

MesManager::MesManager():
    m_mestimer(new QTimer())
{
    connect(m_mestimer, SIGNAL(timeout()), this, SLOT(checkmes()));
}

MesManager::~MesManager()
{

}

int MesManager::g_visitetime = 0;
QString MesManager::g_station = "";
QString MesManager::g_sn = "";
int MesManager::g_flag = 0;
QString MesManager::g_status = "";
QString MesManager::g_errmsg = "";

void MesManager::sendhttpmsg(QString station, QString SN, int flag, QString status, QString ErrMsg)
{
    QSettings settings("testtool.ini", QSettings::IniFormat);

    QString gpsBaud = settings.value("MesIP/IP").toString();
    QString gpsPort = settings.value("MesIP/Port").toString();

    QString baseUrl;
    baseUrl.clear();
    // URL
    if(g_flag == 0)
    {
        qDebug() <<"flag0" <<flag;
            baseUrl = QString("http://127.0.0.1:23413/TestUrl/sg/ajax?style=smtUtil&type=RouterCheck");
#if 0
        baseUrl = QString("http://%1:%2/sg/ajax?style=smtUtil&type=RouterCheck&allow_type=allow_type&station=%3&sn=%4")
                .arg(gpsBaud).arg(gpsPort).arg(station).arg(SN);
#endif
        qDebug()<<baseUrl;
    }
    else if(g_flag == 1)
    {
        qDebug() <<"flag1" <<flag;

        baseUrl = QString("http://127.0.0.1:23413/TestUrl");
#if 0
        baseUrl = QString("http://%1:%2/sg/ajax?style=smtUtil&type=StationPass&allow_type=allow_type&Station=%3&Sn=%4&Status=%5&ToolNo=""&UserName=UA&ErrMsg=%6")
                .arg(gpsBaud).arg(gpsPort).arg(station).arg(SN).arg(status).arg(ErrMsg);
#endif
    }
    else if(g_flag == 2)
    {
        baseUrl = QString("http://127.0.0.1:23413/TestUrl");
#if 0
        baseUrl = QString("http://%1:%2/sg/ajax?style=smtUtil&type=JDdata&allow_type=allow_type&sn=%3&Position=%4&data1=%5&data2=&data3=")
                .arg(gpsBaud).arg(gpsPort).arg(SN).arg(station).arg(status);
#endif
    }
    else if(g_flag == 4)
    {
        baseUrl = QString("http://%1:%2/sg/ajax?style=smtUtil&type=StationPass&allow_type=allow_type&Station=%3&Sn=%4&Status=%5&ToolNo=&UserName=89001618&ErrMsg=%6")
                .arg(gpsBaud).arg(gpsPort).arg(station).arg(SN).arg(status).arg(ErrMsg);
    }


    // 构造请求

    network.setUrl(QUrl(baseUrl));

    QUrl url(baseUrl);
    qDebug() << url.url();

    manager = new QNetworkAccessManager(this);
    // 发送请求
    reply = manager->get(network);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));


}

void MesManager::startcheckmes(QString station,QString SN,int flag,QString status,QString ErrMsg)
{
    m_mestimer->start(1000);
    g_station = station;
    g_sn = SN;
    g_flag  = flag;
    g_status = status;
    g_errmsg = ErrMsg;
    sendhttpmsg(g_station,g_sn,g_flag,g_status,g_errmsg);
}

void MesManager::stopcheckmes()
{
    g_visitetime = 0;
    m_mestimer->stop();
}

void MesManager::checkmes()
{
    //    sendhttpmsg(g_station,g_sn,g_flag,g_status,g_errmsg);
    //    m_mestimer->start(1000);
    ++g_visitetime;
    if(g_visitetime == 10)
    {
        g_visitetime = 0;
        m_mestimer->stop();
        emit mesdata(QString("MESEro"));
    }
}

void MesManager::replyFinished(QNetworkReply*)
{

    QTextCodec::ConverterState state;

    QByteArray bytes= reply->readAll();
    //    char data_test[1024];
    //    memset(data_test, 0, 1024);
    //    if (reply->read(data_test, 1024) <= 0)
    //    {
    //        qDebug("read data failed data len = %d\n");
    ////        return;
    //    }
    //    QString data_test_1(data_test);
    //    qDebug("test data is %s\n", data_test);
    //    qDebug()<<"test data is"<<data_test_1;
    //    QString strJson =  data_test_1;
    //    char* str = bytes.data();
    //    for(int i=0;i<bytes.length();i++)
    //    {
    //        qDebug("s:%x",str[i]);
    //    }
    QTextCodec *codec = QTextCodec::codecForName("utf8");
    qDebug()<<"codec" <<codec;
    QString strJson = codec->toUnicode(bytes.constData(),bytes.size(), &state);

    if (state.invalidChars > 0)
    {
        strJson = QTextCodec::codecForName("GBK")->toUnicode(bytes);
    }
    else
    {
        qDebug()<<"utf-8";
    }
    qDebug()<<"strJson"<<strJson;
    if(QString::compare(strJson,"") != 0)
    {
        QScriptEngine engine;
        QScriptValue sc = engine.evaluate("value=" + strJson);
        QString data = sc.property("code").toString();
        QString descstr = sc.property("desc").toString();
        qDebug()<<"data"<<data;
        qDebug()<<"desc"<<descstr;

        QSettings settings("testtool.ini", QSettings::IniFormat); // 当前目录的INI文件

        settings.beginGroup("A");
        settings.setValue("data", data);
        settings.endGroup();
        if(QString::compare(data,"0") == 0)
        {
            if(g_flag == 0)
            {
                emit mesdata(QString("0:begin"));
                qDebug()<<"0:BEGIN";
            }
            if(g_flag == 1 || g_flag == 4)
            {
                emit mesdata(QString("0:end"));
                qDebug()<<"0:end";
            }
            if(g_flag == 2)
            {
                emit mesdata(QString("0:printer"));

                QSettings settings("testtool.ini", QSettings::IniFormat); // 当前目录的INI文件

                settings.beginGroup("A");
                settings.setValue("string", "0:printer");
                settings.endGroup();
                qDebug()<<"0:printer";
            }
            if(g_flag == 3)
            {
                emit mesdata(QString("0:save"));
                qDebug()<<"0:save";
            }
        }
        else
        {
            //        if (desc.isString())
            {
                if(g_flag == 0)
                {
                    emit mesdata(QString("1:begin")+descstr);
                    qDebug()<<"1:begin";
                }
                if(g_flag == 1 || g_flag == 4)
                {
                    emit mesdata(QString("1:end")+descstr);
                    qDebug()<<"1:end";
                }
                if(g_flag == 2)
                {
                    emit mesdata(QString("1:printer")+descstr);
                    qDebug()<<"1:printer";
                }
                if(g_flag == 3)
                {
                    emit mesdata(QString("1:save")+descstr);
                    qDebug()<<"1:save";
                }
            }
        }
    }
    qDebug()<<"mesend";

#if 0
    QByteArray bytes = reply->readAll();
    qDebug()<<"##############"<<bytes;
    QString string = QString::fromUtf8(bytes);
    qDebug()<<string;
    //    QJsonParseError jsonError;
    //    QJsonDocument doucment = QJsonDocument::fromJson(bytes.data(), &jsonError);
    //    if (jsonError.error != QJsonParseError::NoError) {
    ////        qDebug() << "解析Json失败";
    ////        return;
    //    }


    QJsonObject obj = QJsonDocument::fromJson(bytes.data()).object();
    //    int code = json_object.value("code").toInt();
    //    QString msg = json_object.value("msg").toString();

    // 解析Json
    //    if (doucment.isObject())
    {
        qDebug()<<"=================";
        //        QJsonObject obj = doucment.object();
        qDebug()<<"obj="<<obj.size();
        QJsonValue value;
        QJsonValue desc;
        QString descstr;
        if (obj.contains("code"))
        {
            qDebug()<<"value";
            value = obj.take("code");
            qDebug()<<"value"<<value;

            if (value.isDouble() || value.isString())
            {
                QString data;
                if(value.isDouble())
                {
                    qDebug()<<"isDouble";
                    double data_ = value.toDouble();
                    data = QString::number(data_);
                    //QString data = QString(data_);
                    //qDebug() << "data is : "<<data;
                    //if(QString::compare(data,"0") == 0)
                }
                if(value.isString())
                {
                    data = value.toString();
                }
                //                if (data == 0)
                if(QString::compare(data,"0") == 0)
                {
                    if(g_flag == 0)
                    {
                        emit mesdata(QString("0:begin"));
                        qDebug()<<"0:BEGIN";
                    }
                    if(g_flag == 1)
                    {
                        emit mesdata(QString("0:end"));
                        qDebug()<<"0:end";
                    }
                    if(g_flag == 2)
                    {
                        emit mesdata(QString("0:printer"));
                        qDebug()<<"0:printer";
                    }
                    if(g_flag == 3)
                    {
                        emit mesdata(QString("0:save"));
                        qDebug()<<"0:save";
                    }
                }
                else
                {
                    desc = obj.take("desc");
                    if (desc.isString())
                    {
                        descstr = desc.toString();
                        qDebug() <<"descstr" << descstr;
                        if(g_flag == 0)
                        {
                            emit mesdata(QString("1:begin")+descstr);
                            qDebug()<<"1:begin";
                        }
                        if(g_flag == 1)
                        {
                            emit mesdata(QString("1:end")+descstr);
                            qDebug()<<"1:end";
                        }
                        if(g_flag == 2)
                        {
                            emit mesdata(QString("1:printer")+descstr);
                            qDebug()<<"1:printer";
                        }
                        if(g_flag == 3)
                        {
                            emit mesdata(QString("1:save")+descstr);
                            qDebug()<<"1:save";
                        }
                    }
                }
                qDebug() << data;
            }

            qDebug() << "data";
        }
    }
    //    else
    //    {
    //    qDebug() << "end111111";
    //    }
#endif
}
