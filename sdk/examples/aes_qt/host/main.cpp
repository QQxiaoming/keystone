#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStringList list = QStyleFactory::keys();
    foreach (QString str, list) {
        if(str.contains("Fusion")) {
            QApplication::setStyle(QStyleFactory::create(str));
        }
    }

    MainWindow w;
    w.show();
    return a.exec();
}
