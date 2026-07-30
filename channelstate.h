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
    TextureChannel channel;

    QImage sourceImage;
    QString sourcePath;

    bool hasTexture;
    bool inverted;
    bool isolate;
    bool previewEnabled = true;
};

#endif // CHANNELSTATE_H
