#include "ui_mainwindow.h"
#include "mainwindow.h"


#include "channelstate.h"
#include "textureprocessor.h"
#include "exportservice.h"
#include "popup_about.h"

#include <qdebug.h>

#include <QLabel>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QLineEdit>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    //Window title
    setWindowTitle("PaniCPacker");

    //Set initial size
    resize(960,720);

    //Build using main window.ui (ui from designer)
    ui->setupUi(this);

    setupOutputSizes();
    initializeChannels();
    setupConnections();
    ui->btn_overwrite->setEnabled(false);
    ui->btn_overwrite->setStyleSheet(
        "QPushButton { background-color: rgb(255, 253, 255); color: rgb(30, 30, 30); }"
        "QPushButton:disabled { background-color: rgb(83, 83, 83); color: rgb(150, 150, 150); }"
    );
    ui->statusBar->setStyleSheet(
        "QStatusBar { color: rgb(235, 235, 235); background-color: rgb(30, 30, 30); }"
    );
    updateTextureInfo();
    updatePreview();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::helperCopyTextureData(ChannelState &source, ChannelState &target)
{
    target.sourceImage = source.sourceImage;
    target.sourcePath = source.sourcePath;
    target.hasTexture = source.hasTexture;
    target.comesFromPackedTexture = source.comesFromPackedTexture;
    target.sourceComponent = source.sourceComponent;
    target.inverted = source.inverted;
}

void MainWindow::helperUpdateAllThumbnails()
{
    updateChannelThumbnail(redChannel, ui->img_RedPreview);
    updateChannelThumbnail(greenChannel,ui->img_GreenPreview);
    updateChannelThumbnail(blueChannel, ui->img_BluePreview);
    updateChannelThumbnail(alphaChannel, ui->img_AlphaPreview);
}

void MainWindow::initializeChannels()
{
    ui->f_AlphaChannel->setVisible(false);

    redChannel.channel = TextureChannel::Red;
    greenChannel.channel = TextureChannel::Green;
    blueChannel.channel = TextureChannel::Blue;
    alphaChannel.channel = TextureChannel::Alpha;

    redChannel.sourceComponent = TextureChannel::Red;
    greenChannel.sourceComponent = TextureChannel::Green;
    blueChannel.sourceComponent = TextureChannel::Blue;
    alphaChannel.sourceComponent = TextureChannel::Alpha;

    redChannel.previewEnabled = true;
    greenChannel.previewEnabled = true;
    blueChannel.previewEnabled = true;
    alphaChannel.previewEnabled = true;

    redChannel.inverted = false;
    greenChannel.inverted = false;
    blueChannel.inverted = false;
    alphaChannel.inverted = false;

    redChannel.isolated = false;
    greenChannel.isolated = false;
    blueChannel.isolated = false;
    alphaChannel.isolated = false;

    redChannel.comesFromPackedTexture = false;
    greenChannel.comesFromPackedTexture = false;
    blueChannel.comesFromPackedTexture = false;
    alphaChannel.comesFromPackedTexture = false;

    ui->cb_swap_red->setPlaceholderText("Swap channel");
    ui->cb_swap_red->addItem("None",0);
    ui->cb_swap_red->addItem("Swap To Green",2);
    ui->cb_swap_red->addItem("Swap To Blue",3);
    ui->cb_swap_red->addItem("Swap To Alpha",4);
    
    
    ui->cb_swap_green->setPlaceholderText("Swap channel");
    ui->cb_swap_green->addItem("None",0);
    ui->cb_swap_green->addItem("Swap To Red",1);
    ui->cb_swap_green->addItem("Swap To Blue",3);
    ui->cb_swap_green->addItem("Swap To Alpha",4);

    ui->cb_swap_blue->setPlaceholderText("Swap channel");
    ui->cb_swap_blue->addItem("None",0);
    ui->cb_swap_blue->addItem("Swap To Red",1);
    ui->cb_swap_blue->addItem("Swap To Green",2);
    ui->cb_swap_blue->addItem("Swap To Alpha",4);

    ui->cb_swap_alpha->setPlaceholderText("Swap channel");
    ui->cb_swap_alpha->addItem("None",0);
    ui->cb_swap_alpha->addItem("Swap To Red",1);
    ui->cb_swap_alpha->addItem("Swap To Green",2);
    ui->cb_swap_alpha->addItem("Swap To Blue",3);



    ui->tggle_RedInvert->setChecked(false);
    ui->tggle_RedIsolated->setChecked(false);

    ui->tggl_preview_r->setChecked(true);
    ui->tggl_preview_g->setChecked(true);
    ui->tggl_preview_b->setChecked(true);
    ui->tggl_preview_a->setChecked(true);

    updateChannelThumbnail(
        redChannel,
        ui->img_RedPreview
        );

    updateChannelThumbnail(
        greenChannel,
        ui->img_GreenPreview
        );

    updateChannelThumbnail(
        blueChannel,
        ui->img_BluePreview
        );

    updateChannelThumbnail(
        alphaChannel,
        ui->img_AlphaPreview
        );
}

void MainWindow::setupConnections()
{
    //Button start from texture
    connect(
        ui->btn_StartFromImg,
        &QPushButton::clicked,
        this,
        [this]()
        {
            loadChannelsFromTexture();
            updatePreview();
        }
        );


#pragma region ChannelRED
    //Button load image channel Red
    connect(
        ui->btn_RedLoad,
        &QPushButton::clicked,
        this,
        [this]()
        {
            loadChannelTexture(redChannel);

            updateChannelThumbnail(
                redChannel,
                ui->img_RedPreview
                );

            updatePreview();
        }
        );

    //Button remove image channel Red
    connect(
        ui->btn_RedRemove,
        &QPushButton::clicked,
        this,
        [this]()
        {
            removeChannelTexture(redChannel);

            updateChannelThumbnail(
                redChannel,
                ui->img_RedPreview
                );

            updatePreview();
        }
        );

    //Toggle invert color channel red
    connect(
        ui->tggle_RedInvert,
        &QCheckBox::toggled,
        this,
        [this](bool checked)
        {
            redChannel.inverted = checked;
            markExportOutdated();

            updateChannelThumbnail(
                redChannel,
                ui->img_RedPreview
                );

            updatePreview();
        }
        );

    //Toggle isolate channel red
    connect(
        ui->tggle_RedIsolated,
        &QCheckBox::toggled,
        this,
        [this](bool checked)
        {
            setIsolatedChannel(
                TextureChannel::Red,
                checked
                );
        }
        );

    //Combo box swap
    connect(
        ui->cb_swap_red,
        &QComboBox::activated,
        this,
        [this](int index)
        {
            swapChannel(redChannel,ui->cb_swap_red->currentData().toInt());
            helperUpdateAllThumbnails();
            markExportOutdated();
            updateTextureInfo();
            updatePreview();
        });
#pragma endregion ChannelRED

#pragma region ChannelGREEN
    //Button load image channel Green
    connect(
        ui->btn_GreenLoad,
        &QPushButton::clicked,
        this,
        [this]()
        {
            loadChannelTexture(greenChannel);

            updateChannelThumbnail(
                greenChannel,
                ui->img_GreenPreview
                );

            updatePreview();
        }
        );

    //Button remove image channel Green
    connect(
        ui->btn_GreenRemove,
        &QPushButton::clicked,
        this,
        [this]()
        {
            removeChannelTexture(greenChannel);

            updateChannelThumbnail(
                greenChannel,
                ui->img_GreenPreview
                );            

            updatePreview();
        }
        );    

    //Toggle invert color channel green
    connect(
        ui->tggle_GreenInvert,
        &QCheckBox::toggled,
        this,
        [this](bool checked)
        {
            greenChannel.inverted = checked;
            markExportOutdated();

            updateChannelThumbnail(
                greenChannel,
                ui->img_GreenPreview
                );            

            updatePreview();
        }
        );    

    //Toggle isolate channel green
    connect(
        ui->tggle_GreenIsolated,
        &QCheckBox::toggled,
        this,
        [this](bool checked)
        {
            setIsolatedChannel(
                TextureChannel::Green,
                checked
                );
        }
        );

    //Combo box swap
    connect(
        ui->cb_swap_green,
        &QComboBox::activated,
        this,
        [this](int index)
        {
            swapChannel(greenChannel,ui->cb_swap_green->currentData().toInt());
            helperUpdateAllThumbnails();
            markExportOutdated();
            updateTextureInfo();
            updatePreview();
        });
#pragma endregion ChannelGREEN

#pragma region ChannelBLUE
    //Button load image channel Blue
    connect(
        ui->btn_BlueLoad,
        &QPushButton::clicked,
        this,
        [this]()
        {
            loadChannelTexture(blueChannel);

            updateChannelThumbnail(
                blueChannel,
                ui->img_BluePreview
                );

            updatePreview();
        }
        );

    //Button remove image channel Blue
    connect(
        ui->btn_BlueRemove,
        &QPushButton::clicked,
        this,
        [this]()
        {
            removeChannelTexture(blueChannel);

            updateChannelThumbnail(
                blueChannel,
                ui->img_BluePreview
                );

            updatePreview();
        }
        );    

    //Toggle invert color channel blue
    connect(
        ui->tggle_BlueInvert,
        &QCheckBox::toggled,
        this,
        [this](bool checked)
        {
            blueChannel.inverted = checked;
            markExportOutdated();

            updateChannelThumbnail(
                blueChannel,
                ui->img_BluePreview
                );

            updatePreview();
        }
        );    

    //Toggle isolate channel blue
    connect(
        ui->tggle_BlueIsolated,
        &QCheckBox::toggled,
        this,
        [this](bool checked)
        {
            setIsolatedChannel(
                TextureChannel::Blue,
                checked
                );
        }
        );

    //Combo box swap
    connect(
        ui->cb_swap_blue,
        &QComboBox::activated,
        this,
        [this](int index)
        {
            swapChannel(blueChannel,ui->cb_swap_blue->currentData().toInt());
            helperUpdateAllThumbnails();
            markExportOutdated();
            updateTextureInfo();
            updatePreview();
        });
#pragma endregion ChannelBLUE

    //Toogle use Alpha
    connect(
        ui->tggle_useAlpha,
        &QCheckBox::toggled,
        this,
        [this](bool checked)
        {
            ui->f_AlphaChannel->setVisible(checked);
            markExportOutdated();
            updateTextureInfo();
            updatePreview();
        });

#pragma region ChannelALPHA                    
    //Button load image channel Alpha
    connect(
        ui->btn_AlphaLoad,
        &QPushButton::clicked,
        this,
        [this]()
        {
            loadChannelTexture(alphaChannel);            

            updateChannelThumbnail(
                alphaChannel,
                ui->img_AlphaPreview
                );

            updatePreview();
        }
        );

    //Button remove image channel Alpha
    connect(
        ui->btn_AlphaRemove,
        &QPushButton::clicked,
        this,
        [this]()
        {
            removeChannelTexture(alphaChannel);

            updateChannelThumbnail(
                alphaChannel,
                ui->img_AlphaPreview
                );            
            updatePreview();
        }
        );    

    //Toggle invert color channel alpha
    connect(
        ui->tggle_AlphaInvert,
        &QCheckBox::toggled,
        this,
        [this](bool checked)
        {
            alphaChannel.inverted = checked;
            markExportOutdated();

            updateChannelThumbnail(
                alphaChannel,
                ui->img_AlphaPreview
                );

            updatePreview();
        }
        );    

    //Toggle isolate channel alpha
    connect(
        ui->tggle_AlphaIsolated,
        &QCheckBox::toggled,
        this,
        [this](bool checked)
        {
            setIsolatedChannel(
                TextureChannel::Alpha,
                checked
                );
        }
        );

    //Combo box swap
    connect(
        ui->cb_swap_alpha,
        &QComboBox::activated,
        this,
        [this](int index)
        {
            swapChannel(alphaChannel,ui->cb_swap_alpha->currentData().toInt());
            helperUpdateAllThumbnails();
            markExportOutdated();
            updateTextureInfo();
            updatePreview();
        });
#pragma endregion ChannelALPHA

    //Combo box: Size width
    connect(
        ui->cb_OutputSize_width,
        &QComboBox::activated,
        this,
        [this](int index)
        {
            outputSize_width = ui->cb_OutputSize_width->currentData().toInt();
            markExportOutdated();
            updateTextureInfo();
            updatePreview();
        });

    //Combo box: Size height
    connect(
        ui->cb_OutputSize_height,
        &QComboBox::activated,
        this,
        [this](int index)
        {
            outputSize_height = ui->cb_OutputSize_height->currentData().toInt();
            markExportOutdated();
            updateTextureInfo();
            updatePreview();
        });

    //Button export
    connect(
        ui -> btn_Export,
        &QPushButton::clicked,
        this,
        [this]()
        {
            const QImage packedImage = buildCurrentPackedTexture();

            QString errorMessage;

            const bool success =
                ExportService::exportImage(
                    this,
                    packedImage,
                    &errorMessage,
                    &lastExportPath
                    );

            if (success) {
                exportIsUpToDate = true;
                ui->btn_overwrite->setEnabled(true);
                ui->btn_overwrite->setToolTip(lastExportPath);
                updateTextureInfo();
                ui->statusBar->showMessage(
                    tr("Exported: %1").arg(QFileInfo(lastExportPath).fileName()),
                    3000
                );

                showStyledMessage(
                    QMessageBox::Information,
                    tr("Export complete"),
                    tr("The packed texture was exported.")
                );
            } else {

                if (!errorMessage.isEmpty()) {
                    showStyledMessage(
                        QMessageBox::Warning,
                        tr("Export failed"),
                        errorMessage
                    );
                }
            }
        });

    //Button overwrite
    connect(
        ui->btn_overwrite,
        &QPushButton::clicked,
        this,
        [this]()
        {
            const QImage packedImage = buildCurrentPackedTexture();
            QString errorMessage;

            const bool success =
                ExportService::overwriteImage(lastExportPath, packedImage, &errorMessage);

            if (success) {
                exportIsUpToDate = true;
                updateTextureInfo();
                ui->statusBar->showMessage(
                    tr("Overwritten: %1").arg(QFileInfo(lastExportPath).fileName()),
                    3000
                );
            } else {
                showStyledMessage(
                    QMessageBox::Warning,
                    tr("Overwrite failed"),
                    errorMessage
                );
            }
        }
    );

    //Button about
    connect(
        ui->btn_About,
        &QPushButton::clicked,
        this,
        [this]()
        {
            PopupAbout aboutDialog(this);
            aboutDialog.exec();
        }
        );

#pragma region PreviewToggles
    connect(
        ui->tggl_preview_r,
        &QCheckBox::toggled,
        this,
        [this](bool checked)
        {
            redChannel.previewEnabled = checked;
            updatePreview();
        });

    connect(
        ui->tggl_preview_g,
        &QCheckBox::toggled,
        this,
        [this](bool checked)
        {
            greenChannel.previewEnabled = checked;
            updatePreview();
        });

    connect(
        ui->tggl_preview_b,
        &QCheckBox::toggled,
        this,
        [this](bool checked)
        {
            blueChannel.previewEnabled = checked;
            updatePreview();
        });

    connect(
        ui->tggl_preview_a,
        &QCheckBox::toggled,
        this,
        [this](bool checked)
        {
            alphaChannel.previewEnabled = checked;
            updatePreview();
        });
#pragma endregion PreviewToggles
}

void MainWindow::setupOutputSizes()
{
    //Combo box width
    ui->cb_OutputSize_width->clear();
    ui->cb_OutputSize_width->addItem("128", 128);
    ui->cb_OutputSize_width->addItem("256", 256);
    ui->cb_OutputSize_width->addItem("512", 512);
    ui->cb_OutputSize_width->addItem("1024", 1024);
    ui->cb_OutputSize_width->addItem("2048", 2048);
    ui->cb_OutputSize_width->addItem("4096", 4096);
    
    ui->cb_OutputSize_width->setCurrentIndex(3);

    outputSize_width = 1024;
    
    //Combo box height
    ui->cb_OutputSize_height->clear();

    ui->cb_OutputSize_height->addItem("128", 128);
    ui->cb_OutputSize_height->addItem("256", 256);
    ui->cb_OutputSize_height->addItem("512", 512);
    ui->cb_OutputSize_height->addItem("1024", 1024);
    ui->cb_OutputSize_height->addItem("2048", 2048);
    ui->cb_OutputSize_height->addItem("4096", 4096);

    ui->cb_OutputSize_height->setCurrentIndex(3);

    outputSize_height = 1024;
}

QImage MainWindow::buildCurrentPackedTexture() const
{
    return TextureProcessor::buildPackedTexture(
        redChannel,
        greenChannel,
        blueChannel,
        alphaChannel,
        QSize(outputSize_width, outputSize_height),
        ui->tggle_useAlpha->isChecked()
    );
}

QString MainWindow::formatByteSize(qint64 bytes) const
{
    const double kilobytes = static_cast<double>(bytes) / 1024.0;
    const double megabytes = kilobytes / 1024.0;

    if (megabytes >= 1.0) {
        return tr("%1 MB").arg(megabytes, 0, 'f', 2);
    }

    return tr("%1 KB").arg(kilobytes, 0, 'f', 1);
}

void MainWindow::markExportOutdated()
{
    if (!lastExportPath.isEmpty()) {
        exportIsUpToDate = false;
    }
}

//Text to show info about the image loaded and the current output settings
void MainWindow::updateTextureInfo()
{
    const bool useAlpha = ui->tggle_useAlpha->isChecked();
    const int bytesPerPixel = useAlpha ? 4 : 3;
    const QString outputFormat = useAlpha ? tr("RGBA8") : tr("RGB8");
    const qint64 outputBytes =
        static_cast<qint64>(outputSize_width) * outputSize_height * bytesPerPixel;
    const double outputMegabytes =
        static_cast<double>(outputBytes) / (1024.0 * 1024.0);
    QString lastExportInfo = tr("Not exported yet");

    if (!lastExportPath.isEmpty()) {
        const QString exportState = exportIsUpToDate
            ? tr("Current")
            : tr("Outdated");

        lastExportInfo = tr("%1 (%2)")
            .arg(formatByteSize(QFileInfo(lastExportPath).size()))
            .arg(exportState);
    }

    if (infoSourceImage.isNull()) {
        ui->lbl_texInfo->setText(
            tr("No source texture loaded\n"
               "Output: %1 x %2 | Format: %3 | Uncompressed: %4 MB\n"
               "Last export: %5")
                .arg(outputSize_width)
                .arg(outputSize_height)
                .arg(outputFormat)
                .arg(outputMegabytes, 0, 'f', 2)
                .arg(lastExportInfo)
        );
        return;
    }

    const QFileInfo sourceFileInfo(infoSourcePath);

    ui->lbl_texInfo->setText(
        tr("Source: %1 x %2 | Source disk: %3\n"
           "Output: %4 x %5 | Format: %6 | Uncompressed: %7 MB\n"
           "Last export: %8")
            .arg(infoSourceImage.width())
            .arg(infoSourceImage.height())
            .arg(formatByteSize(sourceFileInfo.size()))
            .arg(outputSize_width)
            .arg(outputSize_height)
            .arg(outputFormat)
            .arg(outputMegabytes, 0, 'f', 2)
            .arg(lastExportInfo)
    );
}

void MainWindow::loadChannelTexture(ChannelState &channel)
{
    //Open file explorer to load any image
    const QString filePath = QFileDialog::getOpenFileName(this,tr("Load channel texture"),QString(),tr("Images (*.png *.jpg *.jpeg *.bmp *.tga)"));

    if(filePath.isEmpty())
    {
        return;
    }

    //Get image loaded from file explorer
    const QImage loadedImage(filePath);

    //Error loading image
    if (loadedImage.isNull()) {
        showStyledMessage(
            QMessageBox::Warning,
            tr("Invalid image"),
            tr("The selected file could not be loaded.")
        );
        return;
    }

    //Is not a square texture (only warning)
    if (!TextureProcessor::isSquareTexture(loadedImage)) {
        showStyledMessage(
            QMessageBox::Warning,
            tr("Non-square texture"),
            tr("The image is %1 x %2 pixels and is not square.\n")
                .arg(loadedImage.width())
                .arg(loadedImage.height()));
    }


    //Fill channel properties
    channel.sourceImage = loadedImage;
    channel.sourcePath = filePath;
    channel.hasTexture = true;
    channel.comesFromPackedTexture = false;
    channel.sourceComponent = TextureChannel::Red;
    markExportOutdated();

    infoSourceImage = loadedImage;
    infoSourcePath = filePath;
    updateTextureInfo();

    updatePreview();
}

//Helper when you need to show a warning window (keep the same styles)
void MainWindow::showStyledMessage(
    QMessageBox::Icon icon,
    const QString &title,
    const QString &message
)
{
    QMessageBox messageBox(this);
    messageBox.setIcon(icon);
    messageBox.setWindowTitle(title);
    messageBox.setText(message);
    messageBox.setStyleSheet(
        "QMessageBox { background-color: rgb(30, 30, 30); }"
        "QLabel { color: rgb(235, 235, 235); }"
        "QPushButton {"
        "  color: rgb(235, 235, 235);"
        "  background-color: rgb(70, 70, 70);"
        "  border: 1px solid rgb(100, 100, 100);"
        "  padding: 5px 12px;"
        "}"
        "QPushButton:hover { background-color: rgb(90, 90, 90); }"
    );
    messageBox.exec();
}


void MainWindow::swapChannel(ChannelState &sourceChannel, int indexToSwap)
{
    if(indexToSwap==0)
        return;

    //Get channels data to swap them
    ChannelState _targetChannel;

    //Data from source -> target channel
    switch (indexToSwap) {
    case 1:
        _targetChannel = redChannel;
        helperCopyTextureData(sourceChannel, redChannel);
        break;
    case 2:
        _targetChannel = greenChannel;
        helperCopyTextureData(sourceChannel, greenChannel);
        break;
    case 3:
        _targetChannel = blueChannel;
        helperCopyTextureData(sourceChannel, blueChannel);
        break;
    case 4:
        _targetChannel = alphaChannel;
        helperCopyTextureData(sourceChannel, alphaChannel);
        break;
    }

    //Data from target -> source channel
    switch(sourceChannel.channel)
    {
    case TextureChannel::Red:
        helperCopyTextureData(_targetChannel, redChannel);
        break;
    case TextureChannel::Green:
        helperCopyTextureData(_targetChannel, greenChannel);
        break;
    case TextureChannel::Blue:
        helperCopyTextureData(_targetChannel, blueChannel);
        break;
    case TextureChannel::Alpha:
        helperCopyTextureData(_targetChannel, alphaChannel);
        break;
    }
}

void MainWindow::loadChannelsFromTexture()
{
    //Open file explorer to load any image
    const QString filePath = QFileDialog::getOpenFileName(this,tr("Load RGBA texture"),QString(),tr("Images (*.png *.jpg *.jpeg *.bmp *.tga)"));

    if(filePath.isEmpty())
    {
        return;
    }

    //Get image loaded from file explorer
    const QImage loadedImage(filePath);

    //Error loading image
    if (loadedImage.isNull()) {
        showStyledMessage(
            QMessageBox::Warning,
            tr("Invalid image"),
            tr("The selected file could not be loaded.")
            );
        return;
    }

    //Check is some texture was loaded
    if (!TextureProcessor::isValidSourceImage(loadedImage)) {
        showStyledMessage(
            QMessageBox::Warning,
            tr("Invalid texture size: %1 x %2").arg(loadedImage.width()).arg(loadedImage.height()),
            tr(
                "The texture must be square and use a "
                "power-of-two resolution."
                )
            );
        return;
    }

    //Is not a square texture (only warning)
    if (!TextureProcessor::isSquareTexture(loadedImage)) {
        showStyledMessage(
            QMessageBox::Warning,
            tr("Non-square texture"),
            tr("The image is %1 x %2 pixels and is not square.\n")
                .arg(loadedImage.width())
                .arg(loadedImage.height()));
    }

    //Fill channel properties
    redChannel.sourceImage = loadedImage;
    redChannel.sourcePath = filePath;
    redChannel.hasTexture = true;
    redChannel.comesFromPackedTexture = true;
    redChannel.sourceComponent = TextureChannel::Red;

    greenChannel.sourceImage = loadedImage;
    greenChannel.sourcePath = filePath;
    greenChannel.hasTexture = true;
    greenChannel.comesFromPackedTexture = true;
    greenChannel.sourceComponent = TextureChannel::Green;

    blueChannel.sourceImage = loadedImage;
    blueChannel.sourcePath = filePath;
    blueChannel.hasTexture = true;
    blueChannel.comesFromPackedTexture = true;
    blueChannel.sourceComponent = TextureChannel::Blue;

    alphaChannel.sourceImage = loadedImage;
    alphaChannel.sourcePath = filePath;
    alphaChannel.hasTexture = true;
    alphaChannel.comesFromPackedTexture = true;
    alphaChannel.sourceComponent = TextureChannel::Alpha;
    markExportOutdated();

    infoSourceImage = loadedImage;
    infoSourcePath = filePath;
    updateTextureInfo();

    //Update previews thumbails
    updateChannelThumbnail(redChannel, ui->img_RedPreview);
    updateChannelThumbnail(greenChannel, ui->img_GreenPreview);
    updateChannelThumbnail(blueChannel, ui->img_BluePreview);
    updateChannelThumbnail(alphaChannel, ui->img_AlphaPreview);

    //Update preview
    updatePreview();
}

void MainWindow::removeChannelTexture(ChannelState &channel)
{
    channel.sourceImage = QImage();
    channel.sourcePath.clear();
    channel.hasTexture = false;
    markExportOutdated();

    updatePreview();
}

void MainWindow::updateChannelThumbnail(const ChannelState &channel, QLabel *label)
{
    //Check if label is not null
    if (label == nullptr) {
        return;
    }

    const QSize thumbnailSize(64, 64);

    const QImage thumbnail = TextureProcessor::prepareChannelImage(channel,thumbnailSize);

    //Load thumbail image
    label->setPixmap(QPixmap::fromImage(thumbnail));
    label->setAlignment(Qt::AlignCenter);
}

void MainWindow::setIsolatedChannel(TextureChannel channel, bool enabled)
{
    //Only enable pressed channel to isolate
    redChannel.isolated = enabled && channel == TextureChannel::Red;
    greenChannel.isolated = enabled && channel == TextureChannel::Green;
    blueChannel.isolated = enabled && channel == TextureChannel::Blue;
    alphaChannel.isolated = enabled && channel == TextureChannel::Alpha;

    const bool hasIsolatedChannel =
        redChannel.isolated ||
        greenChannel.isolated ||
        blueChannel.isolated ||
        alphaChannel.isolated;

    ui->tggl_preview_r->setEnabled(!hasIsolatedChannel);
    ui->tggl_preview_g->setEnabled(!hasIsolatedChannel);
    ui->tggl_preview_b->setEnabled(!hasIsolatedChannel);
    ui->tggl_preview_a->setEnabled(!hasIsolatedChannel);

    ui->tggle_RedIsolated->blockSignals(true);
    ui->tggle_GreenIsolated->blockSignals(true);
    ui->tggle_BlueIsolated->blockSignals(true);
    ui->tggle_AlphaIsolated->blockSignals(true);

    //Mark enable toggle for channel isolated
    ui->tggle_RedIsolated->setChecked(redChannel.isolated);
    ui->tggle_GreenIsolated->setChecked(greenChannel.isolated);
    ui->tggle_BlueIsolated->setChecked(blueChannel.isolated);
    ui->tggle_AlphaIsolated->setChecked(alphaChannel.isolated);

    ui->tggle_RedIsolated->blockSignals(false);
    ui->tggle_GreenIsolated->blockSignals(false);
    ui->tggle_BlueIsolated->blockSignals(false);
    ui->tggle_AlphaIsolated->blockSignals(false);

    updatePreview();
}

void MainWindow::updatePreview()
{
    //Check if there are any texture
    const bool hasAnyTexture =
        redChannel.hasTexture ||
        greenChannel.hasTexture ||
        blueChannel.hasTexture ||
        alphaChannel.hasTexture;

    //Avoid generate a empty image if there are not any channel with data
    if (!hasAnyTexture) {
        ui->img_MainPreview->clear();
        ui->img_MainPreview->setText(tr("Load textures to preview"));
        ui->img_MainPreview->setAlignment(Qt::AlignCenter);
        return;
    }

    const QSize previewResolution(outputSize_width, outputSize_height);

    QImage previewImage;

    if (const ChannelState *isolated = isolatedChannel()) {
        previewImage =
            TextureProcessor::buildIsolatedPreview(
                *isolated,
                previewResolution
                );
    } else {
        ChannelState previewRed = redChannel;
        ChannelState previewGreen = greenChannel;
        ChannelState previewBlue = blueChannel;
        ChannelState previewAlpha = alphaChannel;

        if (!previewRed.previewEnabled) {
            previewRed.sourceImage = QImage();
            previewRed.hasTexture = false;
            previewRed.inverted = true;
        }

        if (!previewGreen.previewEnabled) {
            previewGreen.sourceImage = QImage();
            previewGreen.hasTexture = false;
            previewGreen.inverted = true;
        }

        if (!previewBlue.previewEnabled) {
            previewBlue.sourceImage = QImage();
            previewBlue.hasTexture = false;
            previewBlue.inverted = true;
        }

        if (!previewAlpha.previewEnabled) {
            previewAlpha.sourceImage = QImage();
            previewAlpha.hasTexture = false;
            previewAlpha.inverted = false;
        }

        previewImage =
            TextureProcessor::buildPackedTexture(
                previewRed,
                previewGreen,
                previewBlue,
                previewAlpha,
                previewResolution,
                ui->tggle_useAlpha->isChecked());
    }

    const QSize availableSize =
        ui->img_MainPreview->size();

    const QPixmap pixmap =
        QPixmap::fromImage(previewImage).scaled(
            availableSize,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            );

    ui->img_MainPreview->setPixmap(pixmap);
    ui->img_MainPreview->setAlignment(Qt::AlignCenter);
}

const ChannelState *MainWindow::isolatedChannel() const
{
    if (redChannel.isolated) {
        return &redChannel;
    }

    if (greenChannel.isolated) {
        return &greenChannel;
    }

    if (blueChannel.isolated) {
        return &blueChannel;
    }

    if (alphaChannel.isolated) {
        return &alphaChannel;
    }

    return nullptr;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updatePreview();
}
