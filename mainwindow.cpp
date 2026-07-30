#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    //Window title
    setWindowTitle("PaniCPacker");

    //Set initial size
    resize(960,570);

    //Build using main window.ui (ui from designer)
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::exportTexture()
{

}
