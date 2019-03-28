#ifndef VLCMANAGER_H
#define VLCMANAGER_H

#include <QMainWindow>
#include<vlc/vlc.h>

namespace Ui {
class VlcManager;
}

class VlcManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit VlcManager(QWidget *parent = 0);
    ~VlcManager();

    void closeEvent(QCloseEvent *event);
private:
    Ui::VlcManager *ui;

    libvlc_instance_t *vlcInstance;
    libvlc_media_player_t *mediaPlayer;
    libvlc_media_t *media;

Q_SIGNALS:
    void vlcmsg(QString string);
};

#endif // VLCMANAGER_H
