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
        QPixmap drawRenderedImage(QRectF rect);
};

#endif // CAPTURERENDERER_H
