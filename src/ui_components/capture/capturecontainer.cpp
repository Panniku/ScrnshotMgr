#include "capturecontainer.h"

CaptureContainer::CaptureContainer()
{
    // Empty constructor
}

QLabel *CaptureContainer::getCapture()
{
    return mCaptureRender;
}

void CaptureContainer::setCapture(QLabel *newCapture)
{
    mCaptureRender = newCapture;
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

void CaptureContainer::updateCapture()
{
    // This is fixed at 1920x1080 for standard.
    const float aspectRatio = 1920.0f / 1080.0f;
    int containerWidth = width();
    int containerHeight = height();

    int newLabelWidth = containerWidth;
    int newLabelHeight = static_cast<int>(containerWidth / aspectRatio);

    if (newLabelHeight > containerHeight) {
        newLabelHeight = containerHeight;
        newLabelWidth = static_cast<int>(containerHeight * aspectRatio);
    }

    // Center/Scale the pixmap (temp) label
    int labelX = (containerWidth - newLabelWidth) / 2;
    int labelY = (containerHeight - newLabelHeight) / 2;
    mCaptureRender->setGeometry(labelX, labelY, newLabelWidth, newLabelHeight);

    // Adjust the groupBox position and size based on the label size
    // Here we keep the groupBox's position and size relative to the label size
    // Credits: the last remaining tokens of a GPT-4o Trial :3
    float relativeX         = mRect.x() / 1920.0f;
    float relativeY         = mRect.y() / 1080.0f;
    float relativeWidth     = mRect.width() / 1920.0f;
    float relativeHeight    = mRect.height() / 1080.0f;

    int groupBoxX = labelX + static_cast<int>(relativeX * newLabelWidth);
    int groupBoxY = labelY + static_cast<int>(relativeY * newLabelHeight);
    int groupBoxWidth = static_cast<int>(relativeWidth * newLabelWidth);
    int groupBoxHeight = static_cast<int>(relativeHeight * newLabelHeight);


    QPixmap scaled = mCaptureRenderPixmap->scaled(containerWidth, containerHeight, Qt::KeepAspectRatio);
    mCaptureRender->setPixmap(scaled);
    mDisplayText->setGeometry(mCaptureRender->x(), mCaptureRender->y(), mCaptureRender->width(), mCaptureRender->height());
    mPreviewBox->setGeometry(groupBoxX, groupBoxY, groupBoxWidth, groupBoxHeight);
}
