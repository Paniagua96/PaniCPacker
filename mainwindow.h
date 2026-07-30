#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>

#include "channelstate.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    ChannelState redChannel {TextureChannel::Red};
    ChannelState greenChannel {TextureChannel::Green};
    ChannelState blueChannel {TextureChannel::Blue};
    ChannelState alphaChannel {TextureChannel::Alpha};

    int outputSize;

    void initializeChannels();
    void setupConnections();
    void setupPresets();
    void setupOutputSizes();

    void loadChannelTexture(ChannelState &channel);
    void removeChannelTexture(ChannelState &channel);
    void updateChannelThumbnail(const ChannelState &channel, QLabel *label);
    void setIsolatedChannel(TextureChannel channel, bool enabled);
    ChannelState *channelState(TextureChannel channel);

    void updatePreview();

    const ChannelState *isolatedChannel() const;

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
};
#endif // MAINWINDOW_H
