#include <QDebug>
#include <QStandardPaths>
#include <QTimer>
#include <snapper/snapper.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logcat/logcat.h"

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

}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::updateCapture() {
    //
    QScreen *screen = QApplication::primaryScreen();
    QPixmap screenshot = screen->grabWindow(0, 0, 0, screen->size().width(), screen->size().height());
    //QPixmap scaled = screenshot.scaled(ui->capturePreviewLabel->width(), ui->capturePreviewLabel->height(), Qt::KeepAspectRatio);
    int w = ui->capturePreviewLabel->width();
    int h = ui->capturePreviewLabel->height();
    // qDebug() << w << h;
    ui->capturePreviewLabel->setPixmap(screenshot.scaled(w, h, Qt::KeepAspectRatio));
}
void MainWindow::on_snapButton_clicked()
{
    Snapper snapper;
    snapper.screenshot();

    QSettings s;
    qDebug() << "why" << s.value("application/snaps_dir");

    qDebug() << "Settings file path:" << s.fileName();
}

