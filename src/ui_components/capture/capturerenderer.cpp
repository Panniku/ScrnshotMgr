#include "capturerenderer.h"



CaptureRenderer::CaptureRenderer()
{
    // drawBaseImage();
}

QPixmap CaptureRenderer::drawRenderedImage(QRectF rect)
{
    baseImage = new QImage(QSize(1920, 1080), QImage::Format_RGB32);
    painter = new QPainter(baseImage);
    painter->fillRect(QRectF(0,0,1920,1920),QColor(220, 220, 220, 255));
    //
    pen = new QPen();
    pen->setColor(Qt::red);
    pen->setWidth(24);
    pen->setJoinStyle(Qt::MiterJoin);
    painter->setPen(*pen);
    painter->setBackgroundMode(Qt::TransparentMode);
    // painter->drawRect(rect);
    // commented out for obvious reasons :3

    painter = nullptr;
    return QPixmap::fromImage(*baseImage);
}
