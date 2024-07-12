#ifndef SNAPWINDOW_H
#define SNAPWINDOW_H

#include <QMainWindow>

namespace Ui {
class SnapWindow;
}

class SnapWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SnapWindow(QWidget *parent = nullptr);
    ~SnapWindow();

    //
    void setScreenshot(QPixmap screenshot);
    QPixmap getResult();

private:
    Ui::SnapWindow *ui;
    //
    QPixmap mPixmap;
};

#endif // SNAPWINDOW_H
