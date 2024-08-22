#include "utils.h"

#include <QPainter>


QPixmap Utils::getMaskedRecoloredIconPixmap(QPixmap pixmap, QBrush brush)
{
    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.setBrush(brush);

    painter.drawRect(pixmap.rect());
    return pixmap;
}
