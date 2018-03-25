#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.app_dir = a.applicationDirPath();
    w.xmlpf_init();
    w.show();

    return a.exec();
}
