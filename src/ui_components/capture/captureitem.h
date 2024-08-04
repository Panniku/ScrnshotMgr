#ifndef CAPTUREITEM_H
#define CAPTUREITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class CaptureItem : public QWidget
{
    Q_OBJECT

    private:
        QPixmap pixmap;
        QString name;
        QLabel *screenshotImage, *screenshotText;
        QPushButton *copyButton, *showButton, *deleteButton;

    public:
        CaptureItem(QPixmap image, QString text);
};

#endif // CAPTUREITEM_H
