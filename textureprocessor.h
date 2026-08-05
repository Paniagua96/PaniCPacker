#ifndef TEXTUREPROCESSOR_H
#define TEXTUREPROCESSOR_H

#include <QImage>
#include <QSize>

#include "channelstate.h"

class TextureProcessor
{
public:
    static bool isValidSourceImage(const QImage &image);
    static bool isSquareTexture(const QImage &image);

    static QImage getImageFromChannelData(const ChannelState &channel, const QSize &outputSize);
    static QImage buildIsolatedPreview(const ChannelState &channel, const QSize &outputSize);
    static QImage buildPackedTexture( const ChannelState &red, const ChannelState &green, const ChannelState &blue, const ChannelState &alpha, const QSize &outputSize, const bool useAlpha);
};

#endif // TEXTUREPROCESSOR_H
