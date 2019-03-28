#ifndef FACTORYTEST_H
#define FACTORYTEST_H

#include <QMainWindow>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include "function/sensordlg.h"
#include "function/tfcarddlg.h"
#include "function/focuslengthdlg.h"
#include "function/gps4gdlg.h"
#include "function/hdimagedlg.h"
#include "function/lightdlg.h"
#include "function/sensorfocusdlg.h"
#include "function/uniondlg.h"
#include "function/snprinterdlg.h"
#include "widgets/serverset.h"
#include <QSplitter>
#include <QTableWidget>


#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif

namespace Ui {
class FactoryTest;
}

class FactoryTest : public QMainWindow
{
    Q_OBJECT

public:
    explicit FactoryTest(QWidget *parent = 0);
    ~FactoryTest();


    void deletelayout();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void on_btnMin_clicked();
    void on_btnMax_clicked();
    void on_btnExit_clicked();

private:
    Ui::FactoryTest *ui;
    //标题拖动、双击最大化
    bool mDrag;
    QPoint mDragPos;
    bool mIsMax;
    QRect mLocation;
    QFont mIconFont;

    //与缩放相关的变量
    int mZoom;
    QRect mZoomLocation;
    QPoint mZoomPos;

    QLabel *IP_label;
    QLabel *Port_label;
    QLineEdit *IP_line;
    QLineEdit *Port_line;
    QPushButton *button;
    QGridLayout *pLayout;
    QPushButton *sensior_button;
    QPushButton *allbefore_button;
    QPushButton *allafter_button;

private:
    QPushButton *m_sensortest;
    QGridLayout *m_glayout;
    QVBoxLayout *m_hlayout;

    SensorDlg *m_sensorteste;
    TfcardDlg *m_tftest;
    FocuslengthDlg *m_focuslength;
    Gps4GDlg *m_gps;
    HdimageDlg * m_hd;
    LightDlg *m_light;
    SensorfocusDlg *m_focus;
    SnprinterDlg *m_sn;
    UnionDlg *m_union;
    ServerSet *m_serverset;
//    TfTest* tftset;
//    LightTest *lighttest;
//    Gps4gTest *gps_4gtest;
//    ImageTest *imagetest;
//    UnionTest *uniontest;
//    SnWidget *sntest;
//    FocusTest *focoustest;
//    FocalLengthTest *focoulenstest;
//    ServerSet *serverset;

private slots:
    void turntoui();
};

#endif // FACTORYTEST_H
