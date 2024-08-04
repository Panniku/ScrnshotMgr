#include "captureitem.h"

#include <QHBoxLayout>
#include <QSpacerItem>

CaptureItem::CaptureItem(QPixmap image, QString text)
{
    pixmap = image.scaled(24, 24, Qt::KeepAspectRatio);
    name = text;

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(QMargins(0, 0, 0, 0));
    layout->setSpacing(0);

    screenshotImage = new QLabel();
    screenshotImage->setPixmap(pixmap);
    screenshotImage->setFixedSize(QSize(24, 24));
    screenshotText = new QLabel(text);
    screenshotText->setStyleSheet("margin-left: 4px;");

    copyButton = new QPushButton();
    copyButton->setIcon(QIcon(":/resources_root/icons/ph--copy-simple-light.svg"));
    copyButton->setMinimumWidth(24);
    copyButton->setFixedSize(QSize(24, 24));

    showButton = new QPushButton();
    showButton->setIcon(QIcon(":/resources_root/icons/ph--arrow-square-out-light.svg"));
    showButton->setFixedSize(QSize(24, 24));

    deleteButton = new QPushButton();
    deleteButton->setIcon(QIcon(":/resources_root/icons/ph--trash-light.svg"));
    deleteButton->setFixedSize(QSize(24, 24));

    layout->addWidget(screenshotImage);
    layout->addWidget(screenshotText);
    layout->addStretch();
    layout->addWidget(copyButton);
    layout->addWidget(showButton);
    layout->addWidget(deleteButton);

    setFixedHeight(24);
    setLayout(layout);
}
