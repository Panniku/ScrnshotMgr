#include "captureitem.h"

#include <QHBoxLayout>
#include <QSpacerItem>
#include "../../utils/utils.h"

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
    copyButton->setFixedSize(QSize(24, 24));
    copyButton->setFlat(true);

    showButton = new QPushButton();
    showButton->setFixedSize(QSize(24, 24));
    showButton->setFlat(true);

    deleteButton = new QPushButton();
    deleteButton->setFixedSize(QSize(24, 24));
    deleteButton->setFlat(true);

    QPalette p = palette();
    QBrush b = p.brush(QPalette::Light);

    QPixmap copyPixmap = QPixmap(":/res/icons/ph--copy-simple-light.svg");
    copyButton->setIcon(Utils::getMaskedRecoloredIconPixmap(copyPixmap, b));
    QPixmap showPixmap = QPixmap(":/res/icons/ph--arrow-square-out-light.svg");
    showButton->setIcon(Utils::getMaskedRecoloredIconPixmap(showPixmap, b));
    QPixmap deletePixmap = QPixmap(":/res/icons/ph--trash-light.svg");
    deleteButton->setIcon(Utils::getMaskedRecoloredIconPixmap(deletePixmap, b));

    layout->addWidget(screenshotImage);
    layout->addWidget(screenshotText);
    layout->addStretch();
    layout->addWidget(copyButton);
    layout->addWidget(showButton);
    layout->addWidget(deleteButton);

    setFixedHeight(24);
    setLayout(layout);
}
