#include "factorytest.h"
#include <QApplication>
#include <QSharedMemory>
#include "printermanager.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Company");
    QCoreApplication::setApplicationName("AppName");
    QString strKey = QCoreApplication::organizationName() + QCoreApplication::applicationName();

    QSharedMemory sharedMemory(strKey);
    if (!sharedMemory.create(512, QSharedMemory::ReadWrite))
    {
        QMessageBox::information(NULL, QString("提示"), QString("程序已运行！"));
        exit(0);
    }
//    PrinterManager p;
//    p.doprinter();
//    p.show();
    FactoryTest w;
    w.setWindowTitle("测试软件");
    w.show();

    return a.exec();
}
