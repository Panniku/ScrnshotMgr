#include "snapwindow.h"
#include "ui_snapwindow.h"

SnapWindow::SnapWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SnapWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);
    showFullScreen();
}

SnapWindow::~SnapWindow()
{
    delete ui;
}

void SnapWindow::setScreenshot(QPixmap screenshot)
{
    mPixmap = screenshot;
}

QPixmap SnapWindow::getResult()
{
    return mPixmap;
}
