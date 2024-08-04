#ifndef CAPTURESIMPLE_H
#define CAPTURESIMPLE_H

#include "qframe.h"
#include <QLabel>
#include <QObject>

class CaptureSimple : public QFrame
{
    Q_OBJECT

private:
    bool noSnaps = true;
    QLabel *mCaptureRender, *mCaptureTemp;
    QPixmap mCaptureRenderPixmap;

public:
    CaptureSimple();
    void resizeEvent(QResizeEvent *event);
    void updatePixmap();

    // Set
    void setCapture(QLabel *newCapture);
    void setTempLabel(QLabel *temp);
    void setCapturePixmap(QPixmap newCapturePixmap);
    void triggerSnapped();
};

#endif // CAPTURESIMPLE_H
