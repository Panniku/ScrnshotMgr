#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    // This file will literally be untouched for eternity.
    // Yes, this is the bare template file that Qt generates
    // Maybe if i come back to add CLI stuff but oh well :(
    QApplication a(argc, argv);
    a.setApplicationName("ScrnshotMgr");
    MainWindow w;
    w.show();

    return a.exec();
}
