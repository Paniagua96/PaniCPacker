#include "popup_about.h"
#include "ui_popup_about.h"

PopupAbout::PopupAbout(QWidget *parent) : QDialog(parent), ui(new Ui::Popup_About_Main)
{

    //Window title
    setWindowTitle("About");

    //Set initial size
    //resize(960,720);


    ui->setupUi(this);
}

PopupAbout::~PopupAbout()
{
    delete ui;
}
