#include "textureprocessor.h"

bool TextureProcessor::isValidSourceImage(const QImage &image)
{
    //Check if it has an image
    if(image.isNull())
    {
        return false;
    }

    return true;
}

bool TextureProcessor::isSquareTexture(const QImage &image)
{
    return image.width() == image.height();
}

QImage TextureProcessor::getImageFromChannelData(const ChannelState &channel, const QSize &outputSize)
{
    //Get an image loaded -> convert to rgba to avoid wrong data with other formats -> convert to grayscale (8 bits data per pixel)

    QImage result;

    if(channel.hasTexture && !channel.sourceImage.isNull())
    {
        //Create a new image and converted rgba to keep uniform channels with data
        QImage source = channel.sourceImage.convertToFormat(QImage::Format_RGBA8888);

        //Convert grayscale,
        result = QImage(source.size(),QImage::Format_Grayscale8);

        for (int y = 0; y < source.height(); ++y) {

            uchar* resultLine = result.scanLine(y);

            for (int x = 0; x < source.width(); ++x) {
                const QColor color = source.pixelColor(x, y);

                int value = 0;

                //Extract the component selected for this source image.
                switch (channel.sourceComponent) {
                case TextureChannel::Red:
                    value = color.red();
                    break;
                case TextureChannel::Green:
                    value = color.green();
                    break;
                case TextureChannel::Blue:
                    value = color.blue();
                    break;
                case TextureChannel::Alpha:
                    value = color.alpha();
                    break;
                }

                //Save true value of pixel
                resultLine[x] = static_cast<uchar>(value);
            }
        }


        //Scale it from output size value
        result = result.scaled(outputSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    else
    {
        //Create image as fallback, it will be full black
        result = QImage(outputSize, QImage::Format_Grayscale8);

        //Fallback rgb = 0 (black) / alpha = 255 (white)
        uint fallbackColor = channel.channel == TextureChannel::Alpha ? 255 : 0;

        result.fill(fallbackColor);
    }

    // Invert image in case the option was selected
    if(channel.inverted)
    {
        result.invertPixels();
    }

    return result;
}


QImage TextureProcessor::buildIsolatedPreview(const ChannelState &channel, const QSize &outputSize)
{
    //Create variable to save the channel to be isolated
    const QImage gray = getImageFromChannelData(channel,outputSize);

    return gray.convertToFormat(QImage::Format_RGB888);
}

QImage TextureProcessor::buildPackedTexture(const ChannelState &red, const ChannelState &green, const ChannelState &blue, const ChannelState &alpha, const QSize &outputSize,const bool useAlpha)
{
    //Get images with their pixel values
    const QImage redImage = getImageFromChannelData(red,outputSize);
    const QImage greenImage = getImageFromChannelData(green,outputSize);
    const QImage blueImage = getImageFromChannelData(blue,outputSize);
    const QImage alphaImage = getImageFromChannelData(alpha,outputSize);

    //Var to save the final result
    const QImage::Format format = useAlpha
                                      ? QImage::Format_RGBA8888
                                      : QImage::Format_RGB888;
    QImage output(outputSize,format);
    const int channels = useAlpha ? 4 : 3;

    //Write each pixel for each channel with their values
    for (int y = 0; y < output.height(); ++y) {
        const uchar *redLine = redImage.constScanLine(y);
        const uchar *greenLine = greenImage.constScanLine(y);
        const uchar *blueLine = blueImage.constScanLine(y);
        const uchar *alphaLine = alphaImage.constScanLine(y);

        uchar *outputLine = output.scanLine(y);

        for (int x = 0; x < output.width(); ++x) {
            const int outputIndex = x * channels;

            outputLine[outputIndex + 0] = redLine[x];
            outputLine[outputIndex + 1] = greenLine[x];
            outputLine[outputIndex + 2] = blueLine[x];
            if(useAlpha)
                outputLine[outputIndex + 3] = alphaLine[x];
        }
    }

    return output;
}
