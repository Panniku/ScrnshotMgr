#include <QDebug>
#include <QPainter>
#include <QStandardPaths>
#include <QTimer>
#include <snapper/snapper.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logcat/logcat.h"

QPixmap *mCapture;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Logcat::setLogcat(ui->textEdit);
    Logcat::logInfo("Hello World");
    Logcat::logInfo("Running with qt6.6.0");

    QCoreApplication::setApplicationName("config");
    QCoreApplication::setOrganizationName("ScrnMgr");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings settings;

    QString settingsPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

    // Default settings initialization
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/ScrnMgr");
    if(!dir.exists())
    {
        dir.mkpath(".");
    }

    settings.setValue("application/snaps_dir", QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/ScrnMgr");

    Logcat::logInfo("Set path: " + settings.value("application/snaps_dir").toString());

    qDebug() << "Settings file path:" << settings.fileName();

    // Post load
    //QTimer *timer = new QTimer(this);
    //connect(timer, &QTimer::timeout, this, &MainWindow::updateCapture);
    //timer->start(500);
    // this->setStyle("fusion");
    // QApplication::setStyle("Fusion");

    qDebug() << QApplication::screens();

    QPixmap mCapture = ui->capturePreviewLabel->pixmap();
    // capturePixmap = &mPixmap;


    // im ending it all
    QImage image(QSize(1920, 1080), QImage::Format_RGB32);
    QPainter painter(&image);
    painter.fillRect(QRectF(0,0,1920,1920),QColor(220, 220, 220, 255));
    painter.setBrush(Qt::green);
    painter.setPen(QPen(Qt::black));
    painter.setFont(QFont("font", 48));
    painter.drawText(QRectF(0, 0, 1920, 1080), Qt::AlignCenter, "Your screen");
    // image.save("testImage.png");

    // placeholder coords
    // in this case, a youtube video from my browser
    painter.fillRect(QRectF(99, 175, 1277, 718), QColor(255, 0, 0, 63));

    capturePixmap = QPixmap::fromImage(image);
    ui->capturePreviewLabel->setPixmap(capturePixmap);
    ui->capturePreviewLabel->setMinimumSize(1, 1);
    ui->capturePreviewLabel->setGeometry(0, 0, capturePixmap.width(), capturePixmap.height());
    updateCapture();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updateCapture();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::updateCapture() {
    int w = ui->capturePreviewLabel->width();
    int h = ui->capturePreviewLabel->height();
    ui->capturePreviewLabel->setPixmap(capturePixmap.scaled(w, h, Qt::KeepAspectRatio));
}
void MainWindow::on_snapButton_clicked()
{
    Snapper snapper;
    snapper.screenshot();

    QSettings s;
    qDebug() << "why" << s.value("application/snaps_dir");

    qDebug() << "Settings file path:" << s.fileName();
}

