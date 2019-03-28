#include <QPainter>
#include <QTimerEvent>
#include <QFont>
#include <QDebug>
#include "BannerWidget.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif

BannerWidget::BannerWidget(QWidget *parent)
    : QWidget(parent),
      m_nStep(0),
      m_strText(QString::fromLocal8Bit("测试中"))
{
    setAutoFillBackground(true);

    // 设置文字大小
    QFont newFont = font();
    newFont.setPointSize(newFont.pointSize() + 10);
    setFont(newFont);

    m_timer.start(100, this);
}

BannerWidget::~BannerWidget()
{
    m_timer.stop();
}

void BannerWidget::setText(const QString &text)
{
    qDebug()<<"text" <<text;
    m_strText = text;
}

void BannerWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if(QString::compare(m_strText,QString("测试中"))==0)
    {
        // 计算文本绘制的起始坐标
        QFontMetrics metrics(font());
        int x = (width() - metrics.width(m_strText)) / 2;
        int y = (height() + metrics.ascent() - metrics.descent()) / 2;

        QColor color;
        QPainter painter(this);
        for (int i = 0; i < m_strText.size(); ++i)
        {
            // 设置色调（H）、饱和度（S）、亮度（V）
            int nIndex = (m_nStep + i) % 16;
            color.setHsv((15 - nIndex) * 16, 255, 191);
            painter.setPen(color);

            // 单个字符绘制
            painter.drawText(x, y, QString(m_strText[i]));

            // 计算下一个字符的x坐标起始点
            x += metrics.width(m_strText[i]);
        }
    }
    if(QString::compare(m_strText,"测试通过")==0)
    {
        // 计算文本绘制的起始坐标
        QFontMetrics metrics(font());
        int x = (width() - metrics.width(m_strText)) / 2;
        int y = (height() + metrics.ascent() - metrics.descent()) / 2;

        QColor color(0,255,0);
        QPainter painter(this);
        for (int i = 0; i < m_strText.size(); ++i)
        {
            painter.setPen(color);

            // 单个字符绘制
            painter.drawText(x, y, QString(m_strText[i]));

            // 计算下一个字符的x坐标起始点
            x += metrics.width(m_strText[i]);
        }
    }
    if(QString::compare(m_strText,"测试不通过")==0)
    {
        // 计算文本绘制的起始坐标
        QFontMetrics metrics(font());
        int x = (width() - metrics.width(m_strText)) / 2;
        int y = (height() + metrics.ascent() - metrics.descent()) / 2;

        QColor color(255,0,0);
        QPainter painter(this);
        for (int i = 0; i < m_strText.size(); ++i)
        {
            painter.setPen(color);

            // 单个字符绘制
            painter.drawText(x, y, QString(m_strText[i]));

            // 计算下一个字符的x坐标起始点
            x += metrics.width(m_strText[i]);
        }
    }
    if(QString::compare(m_strText,"等待测试")==0)
    {
        // 计算文本绘制的起始坐标
        QFontMetrics metrics(font());
        int x = (width() - metrics.width(m_strText)) / 2;
        int y = (height() + metrics.ascent() - metrics.descent()) / 2;

        QColor color(72,209,204);
        QPainter painter(this);
        for (int i = 0; i < m_strText.size(); ++i)
        {
            painter.setPen(color);

            // 单个字符绘制
            painter.drawText(x, y, QString(m_strText[i]));

            // 计算下一个字符的x坐标起始点
            x += metrics.width(m_strText[i]);
        }
    }


}

void BannerWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if (event->timerId() == m_timer.timerId())
    {
        ++m_nStep;
        update();
    }
    else
    {
        QWidget::timerEvent(event);
    }
}
