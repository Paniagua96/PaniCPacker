#include "textureprocessor.h"


// bool TextureProcessor::isPowerOfTwo(int value)
// {
//     //Check if texture has a valid size and if the module is 0 to know if it is power of two
//     return value > 0 && (value &(value - 1)) == 0;
// }

bool TextureProcessor::isValidSourceImage(const QImage &image)
{
    //Check if it has an image
    if(image.isNull())
    {
        return false;
    }

    return true;


    //FEATURE: Uncomment to allow only textures squared
    //Check if texture is square
    //bool isSquareTexture = image.width() == image.height();

    //Check if width & height are power of two
    //return (isPowerOfTwo(image.width()) && isPowerOfTwo(image.height())) || isSquareTexture;
}

bool TextureProcessor::isSquareTexture(const QImage &image)
{
    return image.width() == image.height();
}

QImage TextureProcessor::prepareChannelImage(const ChannelState &channel, const QSize &outputSize)
{
    QImage result;

    if(channel.hasTexture && !channel.sourceImage.isNull())
    {
        //Create a new image to write info with the texture channel
        QImage source = channel.sourceImage.convertToFormat(QImage::Format_RGBA8888);

        //Convert grayscale
        result = QImage(source.size(),QImage::Format_Grayscale8);

        for (int y = 0; y < source.height(); ++y) {

            uchar* resultLine = result.scanLine(y);

            for (int x = 0; x < source.width(); ++x) {
                const QColor color = source.pixelColor(x, y);

                int value = 0;

                //Extract pixel value for each channel
                switch (channel.channel) {
                case TextureChannel::Red://Always extract info from channel red (it should be a data texture only with one channel or un grayscale)
                     value = channel.comesFromPackedTexture ? color.red() : color.red();
                    break;
                case TextureChannel::Green:
                     value = channel.comesFromPackedTexture ? color.green() : color.red();
                    break;
                case TextureChannel::Blue:
                    value = channel.comesFromPackedTexture ? color.blue() : color.red();
                    break;
                case TextureChannel::Alpha:
                    value = channel.comesFromPackedTexture ? color.alpha() : color.red();
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
        //Create image as fallback, it will be full white
        result = QImage(outputSize, QImage::Format_Grayscale8);
        result.fill(255);
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
    const QImage gray = prepareChannelImage(channel,outputSize);

    return gray.convertToFormat(QImage::Format_RGB888);
}

QImage TextureProcessor::buildPackedTexture(const ChannelState &red, const ChannelState &green, const ChannelState &blue, const ChannelState &alpha, const QSize &outputSize,const bool useAlpha)
{
    //Get images with their pixel values
    const QImage redImage = prepareChannelImage(red,outputSize);
    const QImage greenImage = prepareChannelImage(green,outputSize);
    const QImage blueImage = prepareChannelImage(blue,outputSize);
    const QImage alphaImage = prepareChannelImage(alpha,outputSize);

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
            outputLine[outputIndex + 3] = alphaLine[x];
        }
    }

    return output;
}

