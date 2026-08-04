#ifndef CHANNELSTATE_H
#define CHANNELSTATE_H

#pragma once

#include <QImage>
#include<QString>

enum class TextureChannel
{
    Red,
    Green,
    Blue,
    Alpha
};


struct ChannelState
{
    //Output slot where this data will be packed.
    TextureChannel channel;

    //Component to read from the loaded source image.
    TextureChannel sourceComponent = TextureChannel::Red;

    QImage sourceImage;
    QString sourcePath;

    bool hasTexture;
    bool inverted;
    bool isolated;
    bool previewEnabled = true;
    bool comesFromPackedTexture;
};

#endif // CHANNELSTATE_H
