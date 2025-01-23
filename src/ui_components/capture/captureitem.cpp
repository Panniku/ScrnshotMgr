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

    QPalette p;
    QBrush b = p.brush(QPalette::Text);

    QPixmap copyPixmap = QPixmap(":/res/icons/edit-copy-symbolic.svg");
    QPixmap showPixmap = QPixmap(":/res/icons/open-link-symbolic.svg").scaled(256, 256);
    QPixmap deletePixmap = QPixmap(":/res/icons/trash-empty-symbolic.svg").scaled(22, 22);

    copyButton->setIcon(Utils::getMaskedRecoloredIconPixmap(copyPixmap, b));
    showButton->setIcon(Utils::getMaskedRecoloredIconPixmap(showPixmap, b));
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
