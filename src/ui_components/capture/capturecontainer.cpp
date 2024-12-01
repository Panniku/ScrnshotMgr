#include "capturecontainer.h"
#include "qapplication.h"

#include <QMediaCaptureSession>
#include <QScreenCapture>

CaptureContainer::CaptureContainer(QWidget *parent) : QFrame(parent)
{
    // QScreen *screen = QApplication::primaryScreen();
    // int displayWidth = screen->size().width();
    // int displayHeight = screen->size().height();

    mGraphicsView = new QGraphicsView(this);
    mGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    mDisplayScene = new QGraphicsScene();
    mGraphicsView->setScene(mDisplayScene);

    mDisplayRender = new QGraphicsVideoItem();
    mDisplayScene->addItem(mDisplayRender);

    mDisplayText = new QLabel(mGraphicsView);
    mDisplayText->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 127); border: 0;");

    if(!(mDisplayText->text() == "")) {
        mDisplayText->hide();
    }
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

void CaptureContainer::setDisplayText(QString newText)
{
    mDisplayText->setText(newText);
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

QString CaptureContainer::getPreviewName()
{
    return *mPreviewName;
}

void CaptureContainer::setPreviewName(QString newPreviewName)
{
    mPreviewName = &newPreviewName;
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
}

void CaptureContainer::updateCapture()
{
    const float aspectRatio = mWidth / mHeight;
    int containerWidth = width();
    int containerHeight = height();

    int newContainerWidth = containerWidth;
    int newContainerHeight = static_cast<int>(containerWidth / aspectRatio);

    if (newContainerHeight > containerHeight) {
        newContainerHeight = containerHeight;
        newContainerWidth = static_cast<int>(containerHeight * aspectRatio);
    }

    // Scale the display render
    int containerX = (containerWidth - newContainerWidth) / 2;
    int containerY = (containerHeight - newContainerHeight) / 2;

    // Workaround, thanks Gemini
    mGraphicsView->setGeometry(containerX, containerY, newContainerWidth, newContainerHeight);
    mDisplayRender->setSize(mGraphicsView->size());

    // Adjust the groupBox position and size based on the label size
    // Here we keep the groupBox's position and size relative to the label size
    // Credits: the last remaining tokens of a GPT-4o Trial :3
    float relativeX         = mRect.x() / mWidth;
    float relativeY         = mRect.y() / mHeight;
    float relativeWidth     = mRect.width() / mWidth;
    float relativeHeight    = mRect.height() / mHeight;

    int groupBoxX = containerX + static_cast<int>(relativeX * newContainerWidth);
    int groupBoxY = containerY + static_cast<int>(relativeY * newContainerHeight);
    int groupBoxWidth = static_cast<int>(relativeWidth * newContainerWidth);
    int groupBoxHeight = static_cast<int>(relativeHeight * newContainerHeight);

    mDisplayText->move(
        mGraphicsView->geometry().width() - mDisplayText->geometry().width(),
        mGraphicsView->geometry().height() - mDisplayText->geometry().height()
        );

    mPreviewBox->setGeometry(groupBoxX, groupBoxY, groupBoxWidth, groupBoxHeight);
}
