#ifndef SERVERSET_H
#define SERVERSET_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>

namespace Ui {
class ServerSet;
}

class ServerSet : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerSet(QWidget *parent = 0,QWidget *tablewidget = 0);
    ~ServerSet();

private:
    Ui::ServerSet *ui;
    QLabel *m_iplabel;
    QLabel *m_portlabel;
    QLineEdit *m_ipline;
    QLineEdit *m_portline;
    QGridLayout *m_layout;
    QPushButton *m_button;
    QCheckBox *m_checkbox;


private slots:
    void save_ipport();
    void statechange();

};

#endif // SERVERSET_H
