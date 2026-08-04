#ifndef POPUP_ABOUT_H
#define POPUP_ABOUT_H

#include <QDialog>

namespace Ui { class Popup_About_Main; }

class PopupAbout : public QDialog
{
    Q_OBJECT

public:
    explicit PopupAbout(QWidget *parent = nullptr);
    ~PopupAbout();

private:
    Ui::Popup_About_Main *ui;
};

#endif // POPUP_ABOUT_H
