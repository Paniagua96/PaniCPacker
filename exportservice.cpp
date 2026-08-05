#include "exportservice.h"

#include <QFileDialog>
#include <QFileInfo>


bool ExportService::exportImage(QWidget *parent, const QImage &image, QString *errorMessage, QString *exportedFilePath)
{
    const QString filePath =
        QFileDialog::getSaveFileName(
            parent,
            QObject::tr("Export packed texture"),
            QString(),
            QObject::tr(
                "PNG Image (*.png);;"
                "TGA Image (*.tga);;"
                "BMP Image (*.bmp)"
                )
            );

    if (filePath.isEmpty()) {
        return false;
    }

    if (!overwriteImage(filePath, image, errorMessage)) {
        return false;
    }

    if (exportedFilePath != nullptr) {
        *exportedFilePath = filePath;
    }

    return true;
}

bool ExportService::overwriteImage(const QString &filePath, const QImage &image,  QString *errorMessage)
{
    if (filePath.isEmpty()) {
        if (errorMessage != nullptr) {
            *errorMessage = QObject::tr("There is no previous export path.");
        }

        return false;
    }

    if (!image.save(filePath)) {
        if (errorMessage != nullptr) {
            *errorMessage =  QObject::tr("The image could not be saved.");
        }

        return false;
    }

    return true;
}
