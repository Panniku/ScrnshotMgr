#include "capturecontainer.h"
#include "qapplication.h"

#include <QMediaCaptureSession>
#include <QScreenCapture>

CaptureContainer::CaptureContainer()
{
    mGraphicsView = new QGraphicsView(this);

    mDisplayScene = new QGraphicsScene();
    mGraphicsView->setScene(mDisplayScene);

    mDisplayRender = new QGraphicsVideoItem();
    mDisplayScene->addItem(mDisplayRender);

    QScreenCapture *screenCapture = new QScreenCapture(qApp->primaryScreen());
    screenCapture->start();
    QMediaCaptureSession *media = new QMediaCaptureSession();
    media->setScreenCapture(screenCapture);
    media->setVideoOutput(mDisplayRender);
}

void CaptureContainer::setDimens(float width, float height)
{
    mWidth = width;
    mHeight = height;
}


QGraphicsScene *CaptureContainer::getDisplayScene()
{
    return mDisplayScene;
}

void CaptureContainer::setDisplayScene(QGraphicsScene *displayScene)
{
    mDisplayScene = displayScene;
}

QGraphicsVideoItem *CaptureContainer::getDisplayRender()
{
    return mDisplayRender;
}

void CaptureContainer::setDisplayRender(QGraphicsVideoItem *displayRender)
{
    mDisplayRender = displayRender;
}

QString CaptureContainer::getDisplayText()
{
    return mDisplayText->text();
}

void CaptureContainer::setDisplayLabel(QLabel *displayLabel)
{
    mDisplayText = displayLabel;
}

void CaptureContainer::setDisplayText(QString *newText)
{
    mDisplayText->setText(*newText);
}

QGroupBox *CaptureContainer::getPreviewBox()
{
    return mPreviewBox;
}

QRectF CaptureContainer::getRect()
{
    return mRect;
}

void CaptureContainer::setPreviewBox(QGroupBox *newGroupBox)
{
    mPreviewBox = newGroupBox;
}

void CaptureContainer::setRect(QRectF rect)
{
    mRect = rect;
}

QString *CaptureContainer::getPreviewName()
{
    return mPreviewName;
}

void CaptureContainer::setPreviewName(QString *newPreviewName)
{
    mPreviewName = newPreviewName;
    mPreviewBox->setTitle(*mPreviewName);
}

QPixmap *CaptureContainer::getCapturePixmap()
{
    return mCaptureRenderPixmap;
}

void CaptureContainer::setCapturePixmap(QPixmap *newCapturePixmap)
{
    mCaptureRenderPixmap = newCapturePixmap;
}

// --------------------------------------------------------------------

void CaptureContainer::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
    updateCapture();
}

void CaptureContainer::showEvent(QShowEvent *event) {
    QFrame::showEvent(event);
    updateCapture();
    QRectF bounds = rect(); // CANT FOOL ME WITH THIS ONE, Qt!
    mGraphicsView->fitInView(bounds, Qt::KeepAspectRatio);
}


void CaptureContainer::updateCapture()
{
    const float aspectRatio = mWidth / mHeight;
    int containerWidth = width();
    int containerHeight = height();

    int newLabelWidth = containerWidth;
    int newLabelHeight = static_cast<int>(containerWidth / aspectRatio);

    if (newLabelHeight > containerHeight) {
        newLabelHeight = containerHeight;
        newLabelWidth = static_cast<int>(containerHeight * aspectRatio);
    }

    // Scale the display render
    int labelX = (containerWidth - newLabelWidth) / 2;
    int labelY = (containerHeight - newLabelHeight) / 2;

    mGraphicsView->setGeometry(labelX, labelY, newLabelWidth, newLabelHeight);
    QRectF bounds = mDisplayScene->sceneRect();
    mGraphicsView->fitInView(bounds, Qt::KeepAspectRatio);

    // Adjust the groupBox position and size based on the label size
    // Here we keep the groupBox's position and size relative to the label size
    // Credits: the last remaining tokens of a GPT-4o Trial :3
    float relativeX         = mRect.x() / mWidth;
    float relativeY         = mRect.y() / mHeight;
    float relativeWidth     = mRect.width() / mWidth;
    float relativeHeight    = mRect.height() / mHeight;

    int groupBoxX = labelX + static_cast<int>(relativeX * newLabelWidth);
    int groupBoxY = labelY + static_cast<int>(relativeY * newLabelHeight);
    int groupBoxWidth = static_cast<int>(relativeWidth * newLabelWidth);
    int groupBoxHeight = static_cast<int>(relativeHeight * newLabelHeight);

    mDisplayText->setGeometry(mGraphicsView->x(), mGraphicsView->y(), mGraphicsView->width(), mGraphicsView->height());
    mPreviewBox->setGeometry(groupBoxX, groupBoxY, groupBoxWidth, groupBoxHeight);
}
