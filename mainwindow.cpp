#include <QDebug>
#include <QPainter>
#include <QScrollArea>
#include <QStandardPaths>
#include <QTimer>
#include <QPair>
#include <ui_components/capturerenderer.h>
#include <ui_components/presetobject.h>
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

    // Default settings initialization
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/ScrnMgr");
    if(!dir.exists())
    {
        dir.mkpath(".");
    }

    settings.setValue("application/snaps_dir", QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/ScrnMgr");

    Logcat::logInfo("Set path: " + settings.value("application/snaps_dir").toString());

    qDebug() << "Settings file path:" << settings.fileName();
    qDebug() << QApplication::screens();

    // Capture preview initialization
    captureContainer = new CaptureContainer();
    ui->splitter->addWidget(captureContainer);

    // THIS IS TEMPORARY
    // THIS IS TEMPORARY
    // THIS IS TEMPORARY
    QScrollArea *temparea = new QScrollArea();
    ui->splitter->addWidget(temparea);

    capturePreviewLabel = new QLabel(captureContainer);
    capturePreviewLabel->setPixmap(capturePixmap);
    capturePreviewLabel->setMinimumSize(1, 1);
    capturePreviewLabel->setScaledContents(false);
    capturePreviewLabel->setAlignment(Qt::AlignCenter);
    capturePreviewLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    capturePreviewLabel->setGeometry(0, 0, capturePixmap.width(), capturePixmap.height());

    captureDisplayLabel = new QLabel(captureContainer);
    captureDisplayLabel->setText("Display 1");
    captureDisplayLabel->setAlignment(Qt::AlignCenter);
    captureDisplayLabel->setGeometry(0, 0, capturePixmap.width(), capturePixmap.height());

    captureContainer->setCapture(capturePreviewLabel);
    captureContainer->setDisplayText(captureDisplayLabel);
    captureContainer->setCapturePixmap(&capturePixmap);

    QScreen *s = QApplication::primaryScreen();
    int sw = s->size().width();
    int sh = s->size().height();
    PresetObject fullscreen(QVariant::fromValue(QPair<QString, QRectF>("Fullscreen", QRectF(0, 0, sw, sh))));
    PresetObject freestyle(QVariant::fromValue(QPair<QString, QRectF>("Free draw", QRectF(0, 0, 0, 0))));
    PresetObject layout1(QVariant::fromValue(QPair<QString, QRectF>("Layout 1", QRectF(99, 175, 1277, 718))));

    ui->presetsComboBox->addItem("Free draw", QVariant::fromValue(QPair<QString, QRectF>("Free draw", QRectF(0, 0, 0, 0))));
    ui->presetsComboBox->addItem("Fullscreen",QVariant::fromValue(QPair<QString, QRectF>("Fullscreen", QRectF(0, 0, sw, sh))));
    ui->presetsComboBox->addItem("Layout 1", QVariant::fromValue(QPair<QString, QRectF>("Layout 1", QRectF(99, 175, 1277, 718))));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    // updateCapture();
}
void MainWindow::on_snapButton_clicked()
{
    Snapper snapper;
    snapper.screenshot();

    QSettings s;
    qDebug() << "why" << s.value("application/snaps_dir");

    qDebug() << "Settings file path:" << s.fileName();
}


void MainWindow::on_presetsComboBox_currentIndexChanged(int i)
{
    qDebug() << i;
    CaptureRenderer *render = new CaptureRenderer();
    QRectF newRect(ui->presetsComboBox->currentData().value<QPair<QString, QRectF>>().second);
    capturePixmap = render->drawRenderedImage(newRect);
    // CaptureContainer::setCapturePixmap(&newPixmap);
    captureContainer->setCapturePixmap(&capturePixmap);
    capturePreviewLabel->setPixmap(capturePixmap.scaled(captureContainer->width(), captureContainer->height(), Qt::KeepAspectRatio));

}

