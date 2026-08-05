#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

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

    int outputSize_width;
    int outputSize_height;
    bool exportIsUpToDate = false;
    QImage infoSourceImage;
    QString infoSourcePath;
    QString lastExportPath;
    const ChannelState *isolatedChannel() const;

    void helperCopyTextureData(ChannelState &source, ChannelState &target);
    void helperUpdateAllThumbnails();
    void helperShowStyledMessage(QMessageBox::Icon icon, const QString &title, const QString &message);
    
    void initializeChannels();
    void setupConnections();
    void setupOutputSizes();

    void textureInfo_markExportOutdated();
    QString textureInfo_formatByteSize(qint64 bytes) const;
    void updateTextureInfo();


    void loadChannelsFromTexture();
    void loadChannelTexture(ChannelState &channel);
    void removeChannelTexture(ChannelState &channel);
    void swapChannel(ChannelState &sourceChannel, int indexToSwap);

    void setIsolatedChannel(TextureChannel channel, bool enabled);

    void updateChannelThumbnail(const ChannelState &channel, QLabel *label);
    void updatePreview();

    QImage buildCurrentPackedTexture() const;

protected:
    void resizeEvent(QResizeEvent *event) override;
};
#endif // MAINWINDOW_H
