#ifndef CAPTURERENDERER_H
#define CAPTURERENDERER_H

#include <QObject>
#include <QPixmap>
#include <QPainter>

class CaptureRenderer : public QObject
{
    Q_OBJECT

    private:

    public:
        CaptureRenderer();
        QPixmap drawRenderedImage(float width, float height, QRectF rect);
};

#endif // CAPTURERENDERER_H
