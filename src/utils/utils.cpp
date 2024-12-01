#include "utils.h"

#include <QGuiApplication>
#include <QPainter>
#include <QStyleHints>


QPixmap Utils::getMaskedRecoloredIconPixmap(QPixmap pixmap, QBrush brush)
{
    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.setBrush(brush);

    painter.drawRect(pixmap.rect());
    return pixmap;
}

QString Utils::getSystemTheme()
{
    const auto scheme = QGuiApplication::styleHints()->colorScheme();
    if (scheme == Qt::ColorScheme::Light) {
        return "light";
    } else if (scheme == Qt::ColorScheme::Dark) {
        return "dark";
    } else return "light";
}
