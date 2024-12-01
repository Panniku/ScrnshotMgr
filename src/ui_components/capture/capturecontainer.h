#ifndef CAPTURECONTAINER_H
#define CAPTURECONTAINER_H

#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QVideoWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>

class CaptureContainer : public QFrame
{
    Q_OBJECT

    private:
        float mWidth, mHeight;
        QGraphicsView *mGraphicsView;
        QGraphicsScene *mDisplayScene;
        QGraphicsVideoItem *mDisplayRender;
        QLabel *mDisplayText;
        QGroupBox *mPreviewBox;
        QString *mPreviewName;
        QRectF mRect;
        QPixmap *mCaptureRenderPixmap;

    public:
        CaptureContainer(QWidget *parent = nullptr);
        void resizeEvent(QResizeEvent *event);
        void showEvent(QShowEvent *event);
        void updateCapture();

        // Get
        QGraphicsScene *getDisplayScene();
        QGraphicsVideoItem *getDisplayRender();
        QString getDisplayText();
        QGroupBox *getPreviewBox();
        QRectF getRect();
        QString getPreviewName();
        QPixmap *getCapturePixmap();

        // Set
        void setDimens(float width, float height);
        void setDisplayScene(QGraphicsScene *displayScene);
        void setDisplayRender(QGraphicsVideoItem *displayRender);
        void setDisplayLabel(QLabel *displayLabel);
        void setDisplayText(QString newText);
        void setPreviewBox(QGroupBox *newGroupBox);
        void setRect(QRectF rect);
        void setPreviewName(QString newPreviewName);
        void setCapturePixmap(QPixmap *newCapturePixmap);
};

#endif // CAPTURECONTAINER_H
