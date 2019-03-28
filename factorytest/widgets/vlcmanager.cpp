#include "vlcmanager.h"
#include "ui_vlcmanager.h"
#include <QDebug>
#include <QSettings>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif

VlcManager::VlcManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VlcManager)
{
    ui->setupUi(this);
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowState(Qt::WindowMaximized);
    setWindowTitle("视频播放器");
    //       创建并初始化 libvlc 实例
    QSettings settings("testtool.ini", QSettings::IniFormat);
    QString str= settings.value("ServerIP/IP").toString();
    QString str1 = QString("rtsp://")+QString(str)+QString("/");
    QByteArray tmp = str1.toLatin1();

    char *mm=tmp.data();
    vlcInstance = libvlc_new(0, NULL);
    qDebug()<<"media9";

    // 创建一个 media，参数是一个媒体资源位置（例如：有效的 URL）。
    media = libvlc_media_new_location(vlcInstance, mm);

    qDebug()<<"media";

    // 创建一个 media player 播放环境
    mediaPlayer = libvlc_media_player_new_from_media(media);
    libvlc_video_set_aspect_ratio(mediaPlayer, "18:9");

    // 现在，不需要保留 media 了
    libvlc_media_release(media);

    ui->openGLWidget->setAttribute(Qt::WA_TransparentForMouseEvents); //播放控件不接受事件，方便操作处理。
    WId id = ui->openGLWidget->winId();
    libvlc_media_player_set_hwnd(mediaPlayer, (void *)id);
    QString para = "network-caching=" + QString::number(200);
    QByteArray ba = para.toLatin1();
    char *add = ba.data();
    libvlc_media_add_option(media, add);
    //        libvlc_media_add_option(media, "--rtsp-frame-buffer-size=500000");
    libvlc_media_player_play(mediaPlayer);
}

VlcManager::~VlcManager()
{
    delete ui;
}
void VlcManager::closeEvent(QCloseEvent *event)
{

    qDebug()<<"vlcclose";
    emit  vlcmsg(QString("vlcclose"));
    // 停止播放
    libvlc_media_player_stop(mediaPlayer);

    // 释放 media player
    libvlc_media_player_release(mediaPlayer);

    // 释放 libvlc 实例
    libvlc_release(vlcInstance);
    if(vlcInstance)
    {
        qDebug()<<"ssss1";
    }
}
