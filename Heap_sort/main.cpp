#include "heap_mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Heap_MainWindow w;
    w.show();
    return a.exec();
}
