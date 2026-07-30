#include "textureprocessor.h"


bool TextureProcessor::isPowerOfTwo(int value)
{
    return value > 0 && (value &(value - 1)) == 0;
}

bool TextureProcessor::isValidSourceImage(const QImage &image)
{
    //Check if it has an image
    if(image.isNull())
    {
        return false;
    }

    //Check in case they have different size
    if(image.width() != image.height())
    {
        return false;
    }

    //Check if width & height are power of two
    return isPowerOfTwo(image.width()) && isPowerOfTwo(image.height());
}

QImage TextureProcessor::prepareChannelImage(const ChannelState &channel, const QSize &outputSize)
{
    QImage result;

    if(channel.hasTexture && !channel.sourceImage.isNull())
    {
        //Convert grayscale
        result = channel.sourceImage.convertToFormat(QImage::Format_Grayscale8);

        //Scale it from output size value
        result = result.scaled(outputSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    else
    {
        //Create image as fallback, it will be full white
        result = QImage(outputSize, QImage::Format_Grayscale8);
        result.fill(255);
    }

    // Invert image in case the option was selected
    if(channel.inverted)
    {
        result.invertPixels();
    }
}
