#include "popup_about.h"
#include "ui_popup_about.h"

PopupAbout::PopupAbout(QWidget *parent) : QDialog(parent), ui(new Ui::Popup_About_Main)
{

    //Window title
    setWindowTitle("About");

    //Build using main window.ui (ui from designer)
    ui->setupUi(this);

    //Signal btn about Qt
    connect(
        ui->btn_aboutQt,
        &QPushButton::clicked,
        this,
        [this]()
        {
            QApplication::aboutQt();
        }
        );
}

PopupAbout::~PopupAbout()
{
    delete ui;
}
