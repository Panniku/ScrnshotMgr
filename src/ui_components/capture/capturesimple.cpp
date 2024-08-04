#include "capturesimple.h"

CaptureSimple::CaptureSimple() {}

void CaptureSimple::setCapture(QLabel *newCapture) {
    mCaptureRender = newCapture;
}

void CaptureSimple::setTempLabel(QLabel *temp) {
    mCaptureTemp = temp;
}

void CaptureSimple::setCapturePixmap(QPixmap newCapturePixmap) {
    mCaptureRenderPixmap = newCapturePixmap;
}

void CaptureSimple::triggerSnapped() {
    if(noSnaps) {
        noSnaps = false;
        mCaptureTemp->hide();
    }
}

void CaptureSimple::resizeEvent(QResizeEvent *event) {
    QFrame::resizeEvent(event);
    updatePixmap();
}

void CaptureSimple::updatePixmap() {
    // Yes, part of the code taken from capturecontainer.cpp
    int containerWidth = width();
    int containerHeight = height();

    QPixmap scaled = mCaptureRenderPixmap.scaled(containerWidth, containerHeight, Qt::KeepAspectRatio);

    mCaptureRender->setPixmap(scaled);
    mCaptureRender->setGeometry(0, 0, containerWidth, containerHeight);
    mCaptureTemp->setGeometry(0, 0, containerWidth, containerHeight);
}
