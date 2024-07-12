#ifndef CAPTURECONTAINER_H
#define CAPTURECONTAINER_H

#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include <QGroupBox>

class CaptureContainer : public QFrame
{
    Q_OBJECT

    private:
        QLabel *mCaptureRender, *mDisplayText;
        QGroupBox *mPreviewBox;
        QString *mPreviewName;
        QRectF mRect;
        static inline QPixmap *mCaptureRenderPixmap = nullptr;

    public:
        CaptureContainer();
        void resizeEvent(QResizeEvent *event);
        void updateCapture();

        // Get
        QLabel *getCapture();
        QString getDisplayText();
        QGroupBox *getPreviewBox();
        QRectF getRect();
        QString *getPreviewName();
        QPixmap *getCapturePixmap();

        // Set
        void setCapture(QLabel *newCapture);
        void setDisplayLabel(QLabel *displayLabel);
        void setDisplayText(QString *newText);
        void setPreviewBox(QGroupBox *newGroupBox);
        void setRect(QRectF rect);
        void setPreviewName(QString *newPreviewName);
        static void setCapturePixmap(QPixmap *newCapturePixmap);
};

#endif // CAPTURECONTAINER_H
