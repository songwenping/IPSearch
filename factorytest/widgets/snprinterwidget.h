#ifndef SNPRINTERWIDGET_H
#define SNPRINTERWIDGET_H

#include <QMainWindow>
#include <QPushButton>
#include <QLayout>
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include "keypushbutton.h"

namespace Ui {
class SnPrinterWidget;
}

class SnPrinterWidget : public QMainWindow
{
    Q_OBJECT

public:
    friend class SnprinterDlg;
    explicit SnPrinterWidget(QWidget *parent = 0,QWidget *tablewidget=0);
    ~SnPrinterWidget();
    void closeui();
    bool  eventFilter(QObject *target, QEvent *event);

private:
    void clearitem();
    void print_ero(int row, QString string);
    void print_success(int row, QString string);

private:
    Ui::SnPrinterWidget *ui;
    QLabel *m_label;
    QLabel *m_imeilabel;
    QLineEdit * m_line;
    QLineEdit * m_imeiline;
    QListWidget *m_listwidget;
    Button *m_button;
    QListWidgetItem *itemer;
    static QStringList g_itemlist;
    static int g_testnumber;
    static int g_erronumber;
};

#endif // SNPRINTERWIDGET_H
