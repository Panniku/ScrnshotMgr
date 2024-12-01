#ifndef SNAPPER_H
#define SNAPPER_H

#include "../ui_components/presets/presetobject.h"
#include "snapinterface.h"
#include <QObject>
#include <QApplication>
#include <QScreen>
#include <QPixmap>
#include <QFileDialog>
#include <QSettings>

class SnapCrop : public QObject
{

    Q_OBJECT

private:

    public:
        SnapCrop();
        void crop(QPixmap &pixmap, QPoint start, QPoint end);

    signals:
        void onFinishCrop(QPixmap &pixmap);
};

#endif // SNAPPER_H
