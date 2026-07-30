#include "ui_mainwindow.h"
#include "mainwindow.h"

#include "channelstate.h"
#include "textureprocessor.h"
#include "exportservice.h"

#include <QLabel>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>

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

    ui->tggle_RedInvert->setChecked(false);
    ui->tggle_RedIsolated->setChecked(false);

    ui->btn_r->setChecked(true);
    ui->btn_g->setChecked(true);
    ui->btn_b->setChecked(true);
    ui->btn_a->setChecked(true);

    updateChannelThumbnail(
        redChannel,
        ui->lbl_RedPreview
        );

    updateChannelThumbnail(
        greenChannel,
        ui->lbl_GreenPreview
        );

    updateChannelThumbnail(
        blueChannel,
        ui->lbl_BluePreview
        );

    updateChannelThumbnail(
        alphaChannel,
        ui->lbl_AlphaPreview
        );
}

void MainWindow::setupConnections()
{
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
                ui->lbl_RedPreview
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
                ui->lbl_RedPreview
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
                ui->lbl_RedPreview
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
                ui->lbl_GreenPreview
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
                ui->lbl_GreenPreview
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
                ui->lbl_GreenPreview
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
                ui->lbl_BluePreview
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
                ui->lbl_BluePreview
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
                ui->lbl_BluePreview
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
                ui->lbl_AlphaPreview
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
                ui->lbl_AlphaPreview
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
                ui->lbl_AlphaPreview
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
        ui -> btn_Export,
        &QPushButton::clicked,
        this,
        [this]()
        {
            ui->progressBar->setValue(10);

            const QSize _outputSize(outputSize, outputSize);

            const QImage packedImage =
                TextureProcessor::buildPackedTexture(
                    redChannel,
                    greenChannel,
                    blueChannel,
                    alphaChannel,
                    _outputSize
                    );

            ui->progressBar->setValue(70);

            QString errorMessage;

            const bool success =
                ExportService::exportImage(
                    this,
                    packedImage,
                    &errorMessage
                    );

            if (success) {
                ui->progressBar->setValue(100);

                QMessageBox::information(
                    this,
                    tr("Export complete"),
                    tr("The packed texture was exported.")
                    );
            } else {
                ui->progressBar->setValue(0);

                if (!errorMessage.isEmpty()) {
                    QMessageBox::warning(
                        this,
                        tr("Export failed"),
                        errorMessage
                        );
                }
            }
        });
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
    ui->cb_OutputSize->clear();

    ui->cb_OutputSize->addItem("128 × 128", 128);
    ui->cb_OutputSize->addItem("256 × 256", 256);
    ui->cb_OutputSize->addItem("512 × 512", 512);
    ui->cb_OutputSize->addItem("1024 × 1024", 1024);
    ui->cb_OutputSize->addItem("2048 × 2048", 2048);
    ui->cb_OutputSize->addItem("4096 × 4096", 4096);

    ui->cb_OutputSize->setCurrentIndex(3);

    outputSize = 1024;
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
        QMessageBox::warning(
            this,
            tr("Invalid image"),
            tr("The selected file could not be loaded.")
            );
        return;
    }

    //It is not square texture
    if (!TextureProcessor::isValidSourceImage(loadedImage)) {
        QMessageBox::warning(
            this,
            tr("Invalid texture size"),
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
    const QSize previewResolution(outputSize, outputSize);

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

