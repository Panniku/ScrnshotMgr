#ifndef SnapInterface_H
#define SnapInterface_H

#include <QMainWindow>
#include "../ui_components/presets/presetobject.h"

namespace Ui {
class SnapInterface;
}

class SnapInterface : public QMainWindow
{
    Q_OBJECT

public:
    SnapInterface(QWidget *parent, PresetType type, QRectF rect, QPixmap screenshot);
    ~SnapInterface();


    //
    QPixmap crop(QPoint start, QPoint end);

    void cropOnly(QPoint start, QPoint end);


private:
    Ui::SnapInterface *ui;
    //
    // int mPresetType; // Integer, so to compare with Enum values
    QPixmap mPixmap;


signals:
    void onFinishScreenshot(QPixmap &pixmap);
    void onFinishCrop(QPixmap &pixmap);
    void onCancel();

private slots:
    // This is the event when the DrawSnap is finished
    void onDrawingFinished(QPoint start, QPoint end);
    //
    void on_closeWindow_clicked();
};

#endif // SnapInterface_H
