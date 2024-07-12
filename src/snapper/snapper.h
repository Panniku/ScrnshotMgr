#ifndef SNAPPER_H
#define SNAPPER_H

#include "../ui_components/presets/presetobject.h"
#include <QObject>
#include <QApplication>
#include <QScreen>
#include <QPixmap>
#include <QFileDialog>
#include <QSettings>

class Snapper : public QObject
{

    Q_OBJECT

    public:
        void screenshot(PresetType type, QRectF dimen);
};

#endif // SNAPPER_H
