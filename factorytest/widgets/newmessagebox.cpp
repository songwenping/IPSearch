#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QEvent>
#include <QApplication>
#include <qDebug>
#include "newmessagebox.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif
NewMessageBox::NewMessageBox(QWidget *parent, const QString &title, const QString &text,
                             QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton)
    : QDialog(parent)
{


    //    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::Dialog);
    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    resize(320, 180);
    setStyleSheet("font-family:微软雅黑;font-size:15px;");
    m_titleLbl = new QLabel(this);
    m_closeBtn = new QToolButton(this);
    m_closeBtn->setIcon(QIcon(":/png/image/exit.png"));
    m_titleLbl->setFont(QFont("微软雅黑", 14, QFont::Bold));
    m_titleLbl->setStyleSheet("background-color: silver;");
    m_closeBtn->setStyleSheet("background-color: red;");
    m_titleLbl->setObjectName("whiteLabel");
    m_titleLbl->setText(title);
    m_titleLbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_pIconLabel = new QLabel(this);
    m_pLabel = new QLabel(this);

    QPixmap pixmap(":/png/image/2.ico");
    m_pIconLabel->setPixmap(pixmap);
    m_pIconLabel->setFixedSize(50, 50);
    m_pIconLabel->setScaledContents(true);

    m_pLabel->setOpenExternalLinks(true);
    m_pLabel->setText(text);


    m_pButtonBox = new QDialogButtonBox(this);
    m_pButtonBox->setStandardButtons(QDialogButtonBox::StandardButtons(int(buttons)));
    setDefaultButton(defaultButton);

    QPushButton *pYesButton = m_pButtonBox->button(QDialogButtonBox::Yes);
    if (pYesButton != NULL)
    {
        pYesButton->setObjectName("blueButton");
        pYesButton->setStyle(QApplication::style());
    }
    QPushButton *pOkButton = m_pButtonBox->button(QDialogButtonBox::Ok);
    if (pOkButton != NULL)
    {
        pOkButton->setObjectName("blueButton");
        pOkButton->setStyle(QApplication::style());
    }
    QPushButton *pNoButton = m_pButtonBox->button(QDialogButtonBox::No);
    if (pNoButton != NULL)
    {
        pNoButton->setObjectName("whiteButton");
        pNoButton->setStyle(QApplication::style());
    }
    QPushButton *pCancelButton = m_pButtonBox->button(QDialogButtonBox::Cancel);
    if (pOkButton != NULL)
    {
        pCancelButton->setObjectName("blueButton");
        pCancelButton->setStyle(QApplication::style());
    }

    m_pGridLayout = new QGridLayout(this);
    m_pGridLayout->addWidget(m_titleLbl, 0, 0, 1, 2);
    m_pGridLayout->addWidget(m_closeBtn, 0, 2, 1, 1);
    m_pGridLayout->addWidget(m_pIconLabel, 1, 0, 2, 1/*, Qt::AlignTop*/);
    m_pGridLayout->addWidget(m_pLabel, 1, 1,2,2/*,Qt::AlignLeft|Qt::AlignVCenter*/);
    m_pGridLayout->addWidget(m_pButtonBox, m_pGridLayout->rowCount(), 0, 1, m_pGridLayout->columnCount(),Qt::AlignHCenter);
    m_pGridLayout->setMargin(0);//setMargin(0) 设置Layout 的外部边界为0
    m_pGridLayout->setSpacing(0);

    translateUI();
    connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_pButtonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onButtonClicked(QAbstractButton*)));
}

NewMessageBox::~NewMessageBox()
{

}

void NewMessageBox::changeEvent(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::LanguageChange:
        translateUI();
        break;
    default:
        QWidget::changeEvent(event);
    }
}

void NewMessageBox::translateUI()
{
    QPushButton *pYesButton = m_pButtonBox->button(QDialogButtonBox::Yes);
    if (pYesButton != NULL)
        pYesButton->setText(tr("是"));

    QPushButton *pNoButton = m_pButtonBox->button(QDialogButtonBox::No);
    if (pNoButton != NULL)
        pNoButton->setText(tr("否"));

    QPushButton *pOkButton = m_pButtonBox->button(QDialogButtonBox::Ok);
    if (pOkButton != NULL)
        pOkButton->setText(tr("确定"));

    QPushButton *pCancelButton = m_pButtonBox->button(QDialogButtonBox::Cancel);
    if (pCancelButton != NULL)
        pCancelButton->setText(tr("取消"));
}

QMessageBox::StandardButton NewMessageBox::standardButton(QAbstractButton *button) const
{
    return (QMessageBox::StandardButton)m_pButtonBox->standardButton(button);
}

QAbstractButton *NewMessageBox::clickedButton() const
{
    return m_pClickedButton;
}

int NewMessageBox::execReturnCode(QAbstractButton *button)
{
    int nResult = m_pButtonBox->standardButton(button);
    return nResult;
}

void NewMessageBox::onButtonClicked(QAbstractButton *button)
{
    m_pClickedButton = button;
    done(execReturnCode(button));
}

void NewMessageBox::setDefaultButton(QPushButton *button)
{
    if (!m_pButtonBox->buttons().contains(button))
        return;
    m_pDefaultButton = button;
    button->setDefault(true);
    button->setFocus();
}

void NewMessageBox::setDefaultButton(QMessageBox::StandardButton button)
{
    setDefaultButton(m_pButtonBox->button(QDialogButtonBox::StandardButton(button)));
}

void NewMessageBox::setTitle(const QString &title)
{
    setWindowTitle(title);
}

void NewMessageBox::setText(const QString &text)
{
    m_pLabel->setText(text);
}

void NewMessageBox::setIcon(const QString &icon)
{
    m_pIconLabel->setPixmap(QPixmap(icon));
}

void NewMessageBox::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bPressed = true;
        m_point = event->pos();
    }
}

void NewMessageBox::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bPressed)
            move(event->pos() - m_point + pos());
}

void NewMessageBox::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    m_bPressed = false;
}



mybox::mybox(QWidget *parent)
{

}

mybox::~mybox()
{

}

QMessageBox::StandardButton mybox::showInformation(QWidget *parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton)
{
    NewMessageBox msgBox(parent, title, text, buttons, defaultButton);
    msgBox.resize(300,150);
    msgBox.setIcon(":/png/image/3.ico");
    if (msgBox.exec() == -1)
        return QMessageBox::Cancel;
    return msgBox.standardButton(msgBox.clickedButton());
}
