#include "mainwindow.h"

#include <QApplication>
#include <QIcon>
#include <QVBoxLayout>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resources/icon_app.png"));

    //Create window
    MainWindow w;
    w.show();

    //Initialize loop (update)
    return QApplication::exec();
}
