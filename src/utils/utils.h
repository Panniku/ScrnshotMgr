#ifndef UTILS_H
#define UTILS_H

#include "qpixmap.h"
class Utils
{
public:
    static QPixmap getMaskedRecoloredIconPixmap(QPixmap pixmap, QBrush brush);
    static QString getSystemTheme();
};

#endif // UTILS_H
