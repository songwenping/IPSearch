#include "factorytest.h"
#include "ui_factorytest.h"
#include <QDesktopWidget>
#include <QLineEdit>
#include <QSettings>
#include <QDebug>
#include <QMouseEvent>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif

FactoryTest::FactoryTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FactoryTest)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    mLocation = this->geometry();
    mIsMax = false;
    mDrag = false;

    mZoom = false;

    ui->widgetTitle->installEventFilter(this);
    ui->btnMin->setIcon(QIcon(":/png/image/min.png"));
    ui->btnMax->setIcon(QIcon(":/png/image/max1.png"));
    ui->btnExit->setIcon(QIcon(":/png/image/exit.png"));

    //给缩放的图片label处理鼠标的按下、移动、弹起消息，进行缩放窗口功能
    ui->labelZoom->installEventFilter(this);
    ui->labelZoom->setCursor(Qt::SizeFDiagCursor);
    ui->labelIcon->setPixmap(QPixmap(":/png/image/menu.png"));
    ui->labelTitle->setText("工位选择");

    setStyleSheet("QMainWindow{color:#E8E8E8;background:#ADD8E6;}");

////    m_spliter = new QSplitter(Qt::Vertical,ui->widget);
//    top_widget = new QTableWidget(m_spliter);
//    //    m_list = new QListWidget( m_spliter );
//    down_widget = new QTableWidget(m_spliter);

//    m_spliter->addWidget(top_widget);
//    m_spliter->addWidget( down_widget );
//    m_spliter->setStretchFactor(0,3);//右面窗口上下窗口高度比为1：4
//    m_spliter->setStretchFactor(1,1);

//    QGridLayout* layout = new QGridLayout(ui->widget);
//    layout->addWidget(m_spliter);
//    this->setLayout(layout);

    QStringList list;
    m_glayout = new QGridLayout(ui->widget);
    QSettings settings("testtool.ini", QSettings::IniFormat);
    list= settings.value("buttons/mianbutton").toStringList();
    qDebug()<<"list.at(i)"<<list.size();
    if(list.size() == 1)
    {
        list.clear();
        int j=1;
        list= settings.value("buttons/menubutton").toStringList();
        qDebug()<<"list.at(i)"<<list.size();
        for(int i=0;i<list.size();++i)
        {
            qDebug()<<"list.at(i)"<<list.at(i);
            m_sensortest = new QPushButton(QString(list.at(i)));
            m_glayout->addWidget(m_sensortest,j-1,(i%3),1,1);
            if(((i+1)%3) == 0)
            {
                j++;
            }
            connect(m_sensortest,SIGNAL(clicked()),this,SLOT(turntoui()));
        }
    }
    else
    {
        ui->labelTitle->setText("菜单");
        QLabel *label = new QLabel;
        m_glayout->addWidget(label,0,0,1,3);
        for(int i=0;i<list.size();++i)
        {
            qDebug()<<"list.at(i)"<<list.at(i);
            m_sensortest = new QPushButton(QString(list.at(i)));
            m_glayout->addWidget(m_sensortest,i,1,2,1);
            connect(m_sensortest,SIGNAL(clicked()),this,SLOT(turntoui()));
        }
    }
}

FactoryTest::~FactoryTest()
{
    delete ui;
}


void FactoryTest::deletelayout()
{
    QLayoutItem *item = NULL;
    QGridLayout * layout = (QGridLayout *)ui->widget->layout();
    if(layout)
    {
        while(NULL!=(item = layout->takeAt(0)))
        {
            item->widget()->close();//关闭但不释放
            delete item;
            item = NULL;
        }
    }
    delete layout;
    layout = NULL;
}

bool FactoryTest::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == ui->widgetTitle)
    {
        if(e->type() == QEvent::MouseButtonDblClick)
        {
            on_btnMax_clicked();
            return true;
        }
    }
    if (obj == ui->labelZoom)
    {
        //实现拖动右下角缩放窗口
        if(e->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *event = (QMouseEvent *)e;
            if (event->buttons() & Qt::LeftButton)
            {
                if(mIsMax)
                {
                    //已最大化，就不让再拖动
                    return true;
                }
                mZoom = true;
                mZoomLocation = geometry();
                mZoomPos = event->globalPos();
                return true;
            }
        }
        else if(e->type() == QEvent::MouseMove)
        {
            QMouseEvent *event = (QMouseEvent *)e;
            if (mZoom && (event->buttons() & Qt::LeftButton))
            {
                int dx = event->globalPos().x() - mZoomPos.x();
                int dy = event->globalPos().y() - mZoomPos.y();
                QRect rc = mZoomLocation;
                rc.setRight(rc.right() + dx);
                rc.setBottom(rc.bottom() + dy);
                setGeometry(rc);
                update();
                return true;
            }
        }
        else if(e->type() == QEvent::MouseButtonRelease)
        {
            mZoom = false;
            return true;
        }
    }
    return QObject::eventFilter(obj, e);

}

void FactoryTest::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        mDrag = true;
        mDragPos = e->globalPos() - pos();
        e->accept();
    }
}

void FactoryTest::mouseMoveEvent(QMouseEvent *e)
{
#if 1
    if (mDrag && (e->buttons() && Qt::LeftButton))
    {
        move(e->globalPos() - mDragPos);
        e->accept();
    }
#endif
}

void FactoryTest::mouseReleaseEvent(QMouseEvent *e)
{
    mDrag = false;
}

void FactoryTest::on_btnMin_clicked()
{
    showMinimized();
}

void FactoryTest::on_btnMax_clicked()
{
    if (mIsMax)
    {
        setGeometry(mLocation);
        ui->btnMax->setIcon(QIcon(":/png/image/max1.png"));
        ui->btnMax->setToolTip(QStringLiteral("最大化"));
    }
    else
    {
        mLocation = geometry();
        setGeometry(qApp->desktop()->availableGeometry());
        ui->btnMax->setIcon(QIcon(":/png/image/max2.png"));
        ui->btnMax->setToolTip(QStringLiteral("还原"));
    }
    mIsMax = !mIsMax;
}

void FactoryTest::on_btnExit_clicked()
{
    close();
}

void FactoryTest::turntoui()
{
    deletelayout();
    QString name =((QPushButton*)sender())->text();
    qDebug()<<"name"<<name;
    if(QString::compare(name,"sensor板测试")==0)
    {
        m_sensorteste = new SensorDlg(0,ui->widget);
        ui->labelTitle->setText("sensor板测试");
        ui->labelIcon->setPixmap(QPixmap(":/png/image/sensor.png"));
    }
    if(QString::compare(name,"tf卡板测试")==0)
    {
        m_tftest = new TfcardDlg(0,ui->widget);
        ui->labelTitle->setText("tf卡板测试");
        ui->labelIcon->setPixmap(QPixmap(":/png/image/tf.png"));
    }
    if(QString::compare(name,"补光灯板测试")==0)
    {
        m_light = new LightDlg(0,ui->widget);
        ui->labelTitle->setText("补光灯板测试");
        ui->labelIcon->setPixmap(QPixmap(":/png/image/light.png"));
    }
    if(QString::compare(name,"图像测试")==0)
    {
        m_hd = new HdimageDlg(0,ui->widget);
        ui->labelTitle->setText("图像测试");
        ui->labelIcon->setPixmap(QPixmap(":/png/image/HD.png"));
    }
    if(QString::compare(name,"联测")==0)
    {
        m_union = new UnionDlg(0,ui->widget);
        ui->labelTitle->setText("联测测试");
    }
    if(QString::compare(name,"4G-GPS测试")==0)
    {
        m_gps = new Gps4GDlg(0,ui->widget);
        ui->labelTitle->setText("4G-GPS测试");
        ui->labelIcon->setPixmap(QPixmap(":/png/image/4G.png"));
    }
    if(QString::compare(name,"SN烧录与标签打印")==0)
    {
        m_sn = new SnprinterDlg(0,ui->widget);
        ui->labelTitle->setText("SN烧录与标签打印");
        ui->labelIcon->setPixmap(QPixmap(":/png/image/printer.png"));
    }
    if(QString::compare(name,"sensor调焦")==0)
    {
        m_focus = new SensorfocusDlg(0,ui->widget);
        ui->labelTitle->setText("sensor调焦");
        ui->labelIcon->setPixmap(QPixmap(":/png/image/focus.png"));
    }
    if(QString::compare(name,"sensor焦距check")==0)
    {
        m_focuslength = new FocuslengthDlg(0,ui->widget);
        ui->labelTitle->setText("sensor焦距check");
        ui->labelIcon->setPixmap(QPixmap(":/png/image/focus.png"));
    }

    if(QString::compare(name,"MES配置")==0)
    {
        m_serverset = new ServerSet(0,ui->widget);
        ui->labelTitle->setText("MES配置");
        m_serverset->setWindowModality(Qt::ApplicationModal);
        ui->labelIcon->setPixmap(QPixmap(":/png/image/peizhi.png"));
    }
}
