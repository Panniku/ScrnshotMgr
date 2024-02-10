#ifndef SNAPPER_H
#define SNAPPER_H

#include <QObject>
#include <QApplication>
#include <QScreen>
#include <QPixmap>
#include <QFileDialog>

class Snapper : public QObject
{

    Q_OBJECT

    public:
        void screenshot();
};

#endif // SNAPPER_H
