#ifndef NEWMESSAGEBOX_H
#define NEWMESSAGEBOX_H
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QDialog>
#include <QToolButton>
#include <QMouseEvent>
class QLabel;
class QDialog;

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif

class NewMessageBox : public QDialog
{
    Q_OBJECT

public:
    explicit NewMessageBox(QWidget *parent = 0, const QString &title = tr("Tip"), const QString &text = "",
                        QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::Ok);
    ~NewMessageBox();
    QAbstractButton *clickedButton() const;
    QMessageBox::StandardButton standardButton(QAbstractButton *button) const;
    // 设置默认按钮
    void setDefaultButton(QPushButton *button);
    void setDefaultButton(QMessageBox::StandardButton button);
    // 设置窗体标题
    void setTitle(const QString &title);
    // 设置提示信息
    void setText(const QString &text);
    // 设置窗体图标
    void setIcon(const QString &icon);
    // 添加控件-替换提示信息所在的QLabel
    void addWidget(QWidget *pWidget);
    // 鼠标相对于窗体的位置 event->globalPos() - this->pos()
    void mousePressEvent(QMouseEvent *event);
    // 若鼠标左键被按下，移动窗体位置
    void mouseMoveEvent(QMouseEvent *event);
    // 设置鼠标未被按下
    void mouseReleaseEvent(QMouseEvent *event);


protected:
    // 多语言翻译
    void changeEvent(QEvent *event);


private slots:
    void onButtonClicked(QAbstractButton *button);

private:
    void translateUI();
    int execReturnCode(QAbstractButton *button);

private:
    QLabel *m_pIconLabel;
    QLabel *m_pLabel;
    QGridLayout *m_pGridLayout;
    QDialogButtonBox *m_pButtonBox;
    QDialogButtonBox *m_pNoButtonBox;
    QAbstractButton *m_pClickedButton;
    QAbstractButton *m_pDefaultButton;

    QHBoxLayout*	m_titlelayout;
    QLabel*			m_titleLbl;
    QWidget*		m_titleWgt;
    QToolButton*	m_closeBtn;

    bool m_bPressed;
    QPoint m_point;
};
class mybox:public NewMessageBox
{
    Q_OBJECT
public:
    explicit mybox(QWidget *parent = 0);
    ~mybox();

    QMessageBox::StandardButton showInformation(QWidget *parent, const QString &title,
                                             const QString &text, QMessageBox::StandardButtons buttons,
                                             QMessageBox::StandardButton defaultButton);

};

#endif // NEWMESSAGEBOX_H
