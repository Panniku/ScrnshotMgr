#include "capturerenderer.h"
#include "qpalette.h"



CaptureRenderer::CaptureRenderer()
{
    // drawBaseImage();
}

QPixmap CaptureRenderer::drawRenderedImage(float width, float height, QRectF rect)
{
    QImage baseImage(QSize(width, height), QImage::Format_RGB32);
    QPainter painter(&baseImage);
    QPalette p;
    painter.fillRect(QRectF(0,0,width, height), p.brush(QPalette::Dark));
    return QPixmap::fromImage(baseImage);
}
