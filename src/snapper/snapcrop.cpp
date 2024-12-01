#include "snapcrop.h"
#include "snapinterface.h"
#include "../logcat/logcat.h"

#include <QLabel>

SnapCrop::SnapCrop() { }

void SnapCrop::crop(QPixmap &pixmap, QPoint start, QPoint end) {
    // This code is literally copy pasted from snapinterface.h. oh well
    // Calculate width
    int x = std::min(start.x(), end.x());
    int y = std::min(start.y(), end.y());
    int width = qAbs(end.x() - start.x());
    int height = qAbs(end.y() - start.y());

    // Create the rectangle using start point and calculated width/height
    QRectF rect(x, y, width, height);
    qDebug() << rect;

    QPixmap newPixmap = pixmap.copy(rect.x(), rect.y(), rect.width(), rect.height());
    emit onFinishCrop(newPixmap);
}

