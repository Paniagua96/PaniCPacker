#include "ui_mainwindow.h"
#include "mainwindow.h"


#include "channelstate.h"
#include "textureprocessor.h"
#include "exportservice.h"

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
    resize(960,570);

    //Build using main window.ui (ui from designer)
    ui->setupUi(this);

    setupPresets();
    setupOutputSizes();
    initializeChannels();
    setupConnections();
    ui->btn_overwrite->setEnabled(false);
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

void MainWindow::initializeChannels()
{
    redChannel.channel = TextureChannel::Red;
    greenChannel.channel = TextureChannel::Green;
    blueChannel.channel = TextureChannel::Blue;
    alphaChannel.channel = TextureChannel::Alpha;

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

    redChannel.alphaComesFromAlphaChannel = false;
    greenChannel.alphaComesFromAlphaChannel = false;
    blueChannel.alphaComesFromAlphaChannel = false;
    alphaChannel.alphaComesFromAlphaChannel = false;

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


    // Combo box Preset
    connect(
        ui->cb_Presets, //Object to connect
        &QComboBox::currentIndexChanged, //Wich signal to connect
        this,
        [this](int index) //Lamda function to connect
        {
            ui->lbl_PresetDescription->setText(ui->cb_Presets->itemData(index).toString());
        });

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
#pragma endregion ChannelBLUE    

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
#pragma endregion ChannelALPHA

    connect(
        ui->cb_OutputSize_width,
        &QComboBox::activated,
        this,
        [this](int index)
        {
            outputSize_width = ui->cb_OutputSize_width->currentData().toInt();
            updateTextureInfo();
        });

    connect(
        ui->cb_OutputSize_height,
        &QComboBox::activated,
        this,
        [this](int index)
        {
            outputSize_height = ui->cb_OutputSize_height->currentData().toInt();
            updateTextureInfo();
        });

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
                ui->btn_overwrite->setEnabled(true);
                ui->btn_overwrite->setToolTip(lastExportPath);
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

void MainWindow::setupPresets()
{
    ui->cb_Presets->clear();

    ui->cb_Presets->addItem(
        "ORM",
        "R: Occlusion | G: Roughness | B: Metallic | A: Unused"
        );

    ui->cb_Presets->addItem(
        "RMA",
        "R: Roughness | G: Metallic | B: Ambient Occlusion | A: Unused"
        );

    ui->cb_Presets->addItem(
        "Custom",
        "R: Custom | G: Custom | B: Custom | A: Custom"
        );

    ui->cb_Presets->setCurrentIndex(0);

    ui->lbl_PresetDescription->setText(
        ui->cb_Presets->currentData().toString()
        );
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
        QSize(outputSize_width, outputSize_height)
    );
}

void MainWindow::updateTextureInfo()
{
    const qint64 outputBytes =
        static_cast<qint64>(outputSize_width) * outputSize_height * 4;
    const double outputMegabytes =
        static_cast<double>(outputBytes) / (1024.0 * 1024.0);

    if (infoSourceImage.isNull()) {
        ui->lbl_texInfo->setText(
            tr("No source texture loaded\n"
               "Output: %1 x %2 | Uncompressed: %3 MB (RGBA8)")
                .arg(outputSize_width)
                .arg(outputSize_height)
                .arg(outputMegabytes, 0, 'f', 2)
        );
        return;
    }

    const QFileInfo sourceFileInfo(infoSourcePath);
    const double diskMegabytes =
        static_cast<double>(sourceFileInfo.size()) / (1024.0 * 1024.0);

    ui->lbl_texInfo->setText(
        tr("Source: %1 x %2 | Disk: %3 MB\n"
           "Output: %4 x %5 | Uncompressed: %6 MB (RGBA8)")
            .arg(infoSourceImage.width())
            .arg(infoSourceImage.height())
            .arg(diskMegabytes, 0, 'f', 2)
            .arg(outputSize_width)
            .arg(outputSize_height)
            .arg(outputMegabytes, 0, 'f', 2)
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

    //It is not square texture
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

    //Fill channel properties
    channel.sourceImage = loadedImage;
    channel.sourcePath = filePath;
    channel.hasTexture = true;
    channel.alphaComesFromAlphaChannel = false;

    infoSourceImage = loadedImage;
    infoSourcePath = filePath;
    updateTextureInfo();

    updatePreview();
}

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

    //It is not square texture
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

    //Fill channel properties
    redChannel.sourceImage = loadedImage;
    redChannel.sourcePath = filePath;
    redChannel.hasTexture = true;

    greenChannel.sourceImage = loadedImage;
    greenChannel.sourcePath = filePath;
    greenChannel.hasTexture = true;

    blueChannel.sourceImage = loadedImage;
    blueChannel.sourcePath = filePath;
    blueChannel.hasTexture = true;

    alphaChannel.sourceImage = loadedImage;
    alphaChannel.sourcePath = filePath;
    alphaChannel.hasTexture = true;
    alphaChannel.alphaComesFromAlphaChannel = true;

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
                previewResolution
                );
    }

    const QSize availableSize =
        ui->lbl_MainPreview->size();

    const QPixmap pixmap =
        QPixmap::fromImage(previewImage).scaled(
            availableSize,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            );

    ui->lbl_MainPreview->setPixmap(pixmap);
    ui->lbl_MainPreview->setAlignment(Qt::AlignCenter);
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

