#include"keypushbutton.h"
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QLabel>
#include <QDebug>

Button::Button( QString FileName, int xnum , QWidget *parent,
                int ynum, QString bkGrnd):QToolButton(parent)
{
    QPixmap pixmap(FileName);

    for (int cnt = 0; cnt < xnum; ++cnt)
    {
        for (int y = 0; y < ynum; ++ y)
        {
            pixmatpList.push_back( pixmap.copy(cnt * (pixmap.width() / xnum),
                                               y * (pixmap.height() / ynum),
                                               pixmap.width() / xnum ,
                                               pixmap.height() / ynum));
        }
    }

    if (bkGrnd != NULL)
        background  = new QPixmap(bkGrnd);
    else
        background = NULL;

    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed	);
    curIndex = 0;
}

Button::Button(QVector<QString> &list, QWidget *parent,
               QString bkGrnd):QToolButton(parent)
{
    setPixmapList(list);
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    if (bkGrnd != NULL)
        background  = new QPixmap(bkGrnd);
    else
        background = NULL;

    curIndex = 0;
}

void Button::setPixmapList(QVector<QString> &list)
{
    if (list.size() <= 0) return;
    pixmatpList.clear();

    for (QVector<QString>::iterator pos = list.begin();
             pos != list.end(); ++pos)
    {
        pixmatpList.push_back(QPixmap(*pos));
    }
}

void Button::paintEvent ( QPaintEvent * event)
{
     QPainter painter(this);
     painter.drawPixmap(event->rect(), pixmatpList[curIndex]);
}

void Button::enterEvent(QEvent *event)
{
    if (pixmatpList.size() > 1)
        curIndex = 1;
    else
        curIndex = 0;
    update();
    QToolButton::enterEvent(event);
}

void Button::leaveEvent(QEvent *event)
{
    curIndex = 0;
    update();
    QToolButton::leaveEvent(event);
}

void Button::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (pixmatpList.size() > 2)
        {
            curIndex = 2;
            update();
        }
    }
    QToolButton::mousePressEvent(event);
}

void Button::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (pixmatpList.size() > 1)
        {
            curIndex = 1;
            update();
        }
    }
    QToolButton::mouseReleaseEvent(event);
}

QSize Button::sizeHint() const
{
    return background != NULL? QSize(background->width(), background->height()):
                        QSize(pixmatpList[0].width(), pixmatpList[0].height());
}
