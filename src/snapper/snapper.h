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

class Snapper : public QObject
{

    Q_OBJECT

private:
    QWidget *mParent;

    public:
        void screenshot(QWidget *parent, PresetType type, QRectF dimen);
        //
        SnapInterface *si;

    public slots:
        void onReceiveFinalImage(QPixmap &pixmap);
};

#endif // SNAPPER_H
