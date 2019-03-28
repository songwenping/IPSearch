#ifndef PRINTERMANAGER_H
#define PRINTERMANAGER_H

#include <QMainWindow>
#include <QLabel>

namespace Ui {
class PrinterManager;
}

class PrinterManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit PrinterManager(QWidget *parent = 0);
    ~PrinterManager();

    void setwidget(QString sn,QString IMEI);
    QPixmap genQrcodeImage(QString text);
    void setpicture(QString path, QLabel *label, int x, int y, int widthlength, int heightlength);
    void doprinter();
    void savePDF();
    bool printFile(const QString fileName);
    void printPDF();

private:
    Ui::PrinterManager *ui;

    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
    QLabel *label5;
    QLabel *label6;
    QLabel *label7;
    QLabel *label8;
    QLabel *label9;
};

#endif // PRINTERMANAGER_H
