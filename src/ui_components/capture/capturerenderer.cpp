#include "capturerenderer.h"



CaptureRenderer::CaptureRenderer()
{
    // drawBaseImage();
}

QPixmap CaptureRenderer::drawRenderedImage(QRectF rect)
{
    QImage baseImage(QSize(1920, 1080), QImage::Format_RGB32);
    QPainter painter(&baseImage);
    painter.fillRect(QRectF(0,0,1920,1920),QColor(220, 220, 220, 255));
    return QPixmap::fromImage(baseImage);
}
