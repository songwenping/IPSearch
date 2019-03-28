#include "printermanager.h"
#include "ui_printermanager.h"
#include <QGridLayout>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/qprinter.h>
#include <QtPrintSupport/QPrinterInfo>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QImage>
#include <QPainter>
#include <QPagedPaintDevice>
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>
#include <QGridLayout>
#include <./qrencode/qrencode.h>
#include <QVBoxLayout>
#include <windows.h>
#include <QDebug>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif

PrinterManager::PrinterManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PrinterManager)
{
    ui->setupUi(this);

    QPixmap map = genQrcodeImage("12333333333");
    QWidget *widget = new QWidget();
    QVBoxLayout *vlayout = new QVBoxLayout(widget);
    label1 = new QLabel();
    label2 = new QLabel();
    setpicture(":/png/image/中国移动.png",label1,0,0,200,180);
    label3 = new QLabel("和目智能4G摄像头");
    label3->setAlignment(Qt::AlignBottom);
    label4= new QLabel("扫一扫");
    label4->setAlignment(Qt::AlignHCenter);
    label5= new QLabel("型号： ABCFDSDGYGUY");
    label6= new QLabel("电源： 12V--9W max");
    label7= new QLabel(QString("IMEI号： %1").arg(1444444444444444));
    label8= new QLabel(QString("序列号： %1").arg(12333333333));
    label9= new QLabel("生产日期：2019-01-01");
    vlayout->addWidget(label1);
    vlayout->addWidget(label3);
    //    label1->setPixmap(QPixmap(":/png/image/中国移动.png"));
    label2->setPixmap(QPixmap(map));
    QGridLayout *layout = new QGridLayout(ui->centralwidget);
    layout->addWidget(widget,0,0,1,1);
    layout->addWidget(label2,0,1,1,1);
    layout->addWidget(label4,1,1,1,1);
    layout->addWidget(label5,2,0,1,2);
    layout->addWidget(label6,3,0,1,2);
    layout->addWidget(label7,4,0,1,2);
    layout->addWidget(label8,5,0,1,2);
    layout->addWidget(label9,6,0,1,2);
    setStyleSheet("QLabel{font:bold 28px;font-family:MicrosoftYaHeiUI;color:#101010;width:56px;height:17px;line-height:20px;"
                  "QPushButton{font:bold 28px;}}");
}

PrinterManager::~PrinterManager()
{
    delete ui;
}

void PrinterManager::setwidget(QString sn, QString IMEI)
{
    QPixmap map = genQrcodeImage(sn);
    QWidget *widget = new QWidget();
    QVBoxLayout *vlayout = new QVBoxLayout(widget);
    label1 = new QLabel();
    label2 = new QLabel();
    setpicture(":/png/image/中国移动.png",label1,0,0,200,180);
    label3 = new QLabel("和目智能4G摄像头");
    label3->setAlignment(Qt::AlignBottom);
    label4= new QLabel("扫一扫");
    label4->setAlignment(Qt::AlignHCenter);
    label5= new QLabel("型号： ABCFDSDGYGUY");
    label6= new QLabel("电源： 12V--9W max");
    label7= new QLabel(QString("IMEI号： %1").arg(IMEI));
    label8= new QLabel(QString("序列号： %1").arg(sn));
    label9= new QLabel("生产日期：2019-01-01");
    vlayout->addWidget(label1);
    vlayout->addWidget(label3);
    //    label1->setPixmap(QPixmap(":/png/image/中国移动.png"));
    label2->setPixmap(QPixmap(map));
    QGridLayout *layout = new QGridLayout(ui->centralwidget);
    layout->addWidget(widget,0,0,1,1);
    layout->addWidget(label2,0,1,1,1);
    layout->addWidget(label4,1,1,1,1);
    layout->addWidget(label5,2,0,1,2);
    layout->addWidget(label6,3,0,1,2);
    layout->addWidget(label7,4,0,1,2);
    layout->addWidget(label8,5,0,1,2);
    layout->addWidget(label9,6,0,1,2);
    setStyleSheet("QLabel{font:bold 28px;font-family:MicrosoftYaHeiUI;color:#101010;width:56px;height:17px;line-height:20px;"
                  "QPushButton{font:bold 28px;}}");

    doprinter();
}

QPixmap PrinterManager::genQrcodeImage(QString text)
{
    int margin = 2;
    if (text.length() == 0)
    {
        //      return QPixmap();
    }
    QRcode *qrcode = QRcode_encodeString(text.toLocal8Bit(), 2, QR_ECLEVEL_L, QR_MODE_8, 0);
    if (qrcode == NULL) {
        //      return QPixmap();
    }
    unsigned char *p, *q;
    p = NULL;
    q = NULL;
    int x, y, bit;
    int realwidth;

    realwidth = qrcode->width;
    QImage image = QImage(realwidth, realwidth, QImage::Format_Indexed8);
    QRgb value;
    value = qRgb(255, 255, 255);
    image.setColor(0, value);
    value = qRgb(0, 0, 0);
    image.setColor(1, value);
    image.setColor(2, value);
    image.fill(0);
    p = qrcode->data;
    for (y = 0; y<qrcode->width; y++) {
        bit = 7;
        q += margin / 8;
        bit = 7 - (margin % 8);
        for (x = 0; x<qrcode->width; x++) {
            if ((*p & 1) << bit)
                image.setPixel(x, y, 1);
            else
                image.setPixel(x, y, 0);
            bit--;
            if (bit < 0)
            {
                q++;
                bit = 7;
            }
            p++;
        }
    }
    //     QLabel *label  = new QLabel();
    //     label->setPixmap(QPixmap::fromImage(image.scaledToWidth(200)));
    //     label->show();
    return QPixmap::fromImage(image.scaled(200,180));
}

void PrinterManager::setpicture(QString path, QLabel *label, int x, int y, int widthlength, int heightlength)
{
    QString filename(path);
    QImage* img=new QImage,* scaledimg=new QImage;;
    if(! ( img->load(filename) ) ) //加载图像
    {
        delete img;
        return;
    }
    int Owidth=img->width(),Oheight=img->height();
    int Fwidth,Fheight;       //缩放后的图片大小
    label->setGeometry(x,y,widthlength,heightlength);
    int Mul;            //记录图片与label大小的比例，用于缩放图片
    if(Owidth/widthlength>=Oheight/heightlength)
        Mul=Owidth/widthlength;
    else
        Mul=Oheight/heightlength;
    Fwidth=Owidth/Mul;
    Fheight=Oheight/Mul;
    *scaledimg=img->scaled(Fwidth,Fheight,Qt::KeepAspectRatio);

    label->setPixmap(QPixmap::fromImage(*scaledimg));
}

void PrinterManager::doprinter()
{
//    savePDF();




    // 创建打印机对象
    QPrinter printer;
    // 创建打印对话框
    printer.setPrinterName("HP PageWide Pro 577 MFP");
    printer.setPaperSize(QSizeF(30, 25.1), QPrinter::Millimeter);
    QPrintDialog dlg(&printer, this);
#if 0
    //如果编辑器中有选中区域，则打印选中区域
    if (ui->textEdit->textCursor().hasSelection())
        dlg.addEnabledOption(QAbstractPrintDialog::PrintSelection);
#endif
    // 如果在对话框中按下了打印按钮，则执行打印操作
//        if (dlg.exec() == QDialog::Accepted)
    //这里使用默认打印机，不用每次选择打印机
    {
        //获取界面的图片
        QPixmap pixmap = this->QWidget::grab();

        QPainter painterPixmap;
        painterPixmap.begin(&printer);
        QRect rect = painterPixmap.viewport();
        int x = rect.width() / pixmap.width();
        int y = rect.height() / pixmap.height();
        //将图像(所有要画的东西)在pdf上按比例尺缩放
        painterPixmap.scale(x, y);
        //画图
        painterPixmap.drawPixmap(0, 0, pixmap);
        painterPixmap.end();
        //       ui->textEdit->print(&printer);
        //        painterPixmap.print(&printer);

        // print the existing document by absoult path
        //  printFile("D:/myRSM.doc");
    }
}

void PrinterManager::savePDF()
{
    QString fileName = QFileDialog::getSaveFileName(this, ("保存PDF文件"), QString(), "*.pdf");
    if (!fileName.isEmpty())
    {

        // 如果文件后缀为空，则默认使用.pdf
        if (QFileInfo(fileName).suffix().isEmpty())
        {
            fileName.append(".pdf");
        }

        QPrinter printer(QPrinter::HighResolution);
        QPrintDialog dialog(&printer, this);
        //        //    if (dialog.exec() != QDialog::Accepted)
        //        //        return;
        //       printer.setPrinterName("Zebra 110Xi4 (600 dpi)");
        printer.setPrinterName("HP PageWide Pro 577 MFP");
        printer.setPaperSize(QSizeF(30, 25.1), QPrinter::Millimeter);

        //                printer.setColorMode(QPrinter::GrayScale);
        //                    printer.setPageSizeMM(QSizeF(50,35.2)); //大小
        //                    printer.setPaperSize(QSizeF(50,35.2), QPrinter::Millimeter);
        //                    printer.setResolution(300);//分辨率 203
        //                    printer.setPageMargins(0,0,0,0, QPrinter::Millimeter);
        //                    printer.setOrientation(QPrinter::Portrait);
        //                    printer.setFullPage(true);
        //        printer.setPageSize(QPrinter::Custom);
        //        //横向打印
        //        printer.setOrientation(QPrinter::Landscape);

        //        //设置输出格式为pdf
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        //获取界面的图片
        QPixmap pixmap = QWidget::grab();

        QPainter painterPixmap;
        painterPixmap.begin(&printer);
        QRect rect = painterPixmap.viewport();
        int x = rect.width() / pixmap.width();
        int y = rect.height() / pixmap.height();
        //将图像(所有要画的东西)在pdf上按比例尺缩放
        painterPixmap.scale(x, y);
        //画图
        painterPixmap.drawPixmap(0, 0, pixmap);
        painterPixmap.end();

        QMessageBox::information(this, ("生成PDF"), ("保存PDF文件成功"), QMessageBox::Ok);


        //默认为零，如果用户选择了打印范围，以1为基数。
        //printer.fromPage();
        //printer.toPage();

        //设置打印范围，以1为基数。
        //printer.setFromTo(1, LastNumberOfPage);
        printer.setPrinterName("Zebra 110Xi4 (600 dpi)");

        qDebug("The user has choiced printer.");

        QString fileName = QFileDialog::getOpenFileName(this, ("选择文件"),QString(),
                                                        ("PDF 文档 (*.pdf);;所有文件 (*.*)"));
        QProcess * p = new QProcess;
        p->start("C:\\Program Files (x86)\\XunjiePDFEditor\\XunjiePDFEditor.exe",
                 QStringList() << fileName);

    }
}

bool PrinterManager::printFile(const QString fileName)
{
    QPrinter printer;
        QString printerName = printer.printerName();
        // no default printer or no file return false
        if( printerName.size() == 0 || fileName.size() == 0)
            return false;
        QFileInfo fi(fileName);
        QString filePath = fi.absoluteFilePath();
        if(!fi.exists())
            return false;
        int ret = 0;
        ret = (int)ShellExecuteW(NULL,
                                 QString("print").toStdWString().c_str(),
                                 filePath.toStdWString().c_str(),
                                 NULL,
                                 NULL,
                                 SW_HIDE);
        // if return number bigger than 31 indicate succ
        qDebug()<<"ret:"<<ret<<"--SE_ERR_NOASSOC:"<<SE_ERR_NOASSOC;
        qDebug()<<"filePath:"<<filePath.toStdWString().c_str();
        if (ret > SE_ERR_NOASSOC)
            return true;
        return false;
}

void PrinterManager::printPDF()
{

}
