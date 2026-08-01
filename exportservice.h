#ifndef EXPORTSERVICE_H
#define EXPORTSERVICE_H

#include <QImage>
#include <QString>
#include <QWidget>

class ExportService{
public:
    static bool exportImage(
        QWidget *parent,
        const QImage &image,
        QString *errorMessage = nullptr,
        QString *exportedFilePath = nullptr
    );

    static bool overwriteImage(
        const QString &filePath,
        const QImage &image,
        QString *errorMessage = nullptr
    );

};

#endif // EXPORTSERVICE_H
