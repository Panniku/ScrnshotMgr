#include "drawsnap.h"
#include "snapinterface.h"
#include "ui_snapinterface.h"

#include <QLabel>
#include "../logcat/logcat.h"

SnapInterface::SnapInterface(QWidget *parent, PresetType type, QRectF rect)
    : QMainWindow(parent)
    , ui(new Ui::SnapInterface)
{
    ui->setupUi(this);

    setCursor(Qt::CrossCursor);
    setMouseTracking(true);
    // Logcat::log(LogType::Debug, "SnapInterface", "does thiw rook");

    QScreen *screen = QApplication::primaryScreen();
    QPixmap screenshot = screen->grabWindow(0, 0, 0, screen->size().width(), screen->size().height());

    mPixmap = screenshot;

    switch(type) {
    case Fullscreen:
    case Custom:
        Logcat::log(LogType::Info, "Snapinterface","Selected either fullscreen or custom");
        qDebug() << rect << " / " << type;
        //
        break;

    case Freedraw:
        Logcat::log(LogType::Info, "Snapinterface","freedraw yippe");
        qDebug() << rect;
        // aaaaaa
        break;
    }

    QPainter painter(&screenshot);
    painter.drawPixmap(0, 0, screenshot.width(), screenshot.height(), screenshot);
    painter.fillRect(QRectF(0, 0, screenshot.width(), screenshot.height()), QBrush(QColor(0, 0, 0, 64)));

    // setWindowFlags(Qt::FramelessWindowHint);

    //TODO
    QLabel *preview = new QLabel(ui->frame);
    preview->setPixmap(screenshot);
    preview->setGeometry(0, 0, screenshot.width(), screenshot.height());



    //


    DrawSnap *drawSnap = new DrawSnap(ui->frame);
    drawSnap->setGeometry(0, 0, screenshot.width(), screenshot.height());
    //connect(drawSnap, &DrawSnap::onMouseReleased, this, &SnapInterface::onMouseFinished);
    connect(drawSnap, &DrawSnap::drawingFinished, this, &SnapInterface::onDrawingFinished);
    qDebug() << drawSnap->geometry();
    //
    ui->controlsContainer->raise();
    ui->controlsContainer->setGeometry
        (
            (screenshot.width() / 2) - (ui->controlsContainer->width() / 2),
            0,
            ui->controlsContainer->width(),
            ui->controlsContainer->height()
            );

    showFullScreen();
    Logcat::log(LogType::Debug, "SnapInterface", QString::number(drawSnap->geometry().width()) + " " + QString::number(drawSnap->geometry().height()));

    ui->controlsContainer->setAutoFillBackground(true);
    // ui->controlsContainer->setStyleSheet("background: blue");
}

SnapInterface::~SnapInterface()
{
    delete ui;
}

void SnapInterface::screenshot()
{

}

void SnapInterface::on_closeWindow_clicked()
{
    SnapInterface::close();
    emit onCancel();
}


// This is the event when the DrawSnap is finished
void SnapInterface::onDrawingFinished(QPoint start, QPoint end) {
    qDebug() << "mouse done";

    // Source: Gemini
    // Don't ask why i changed from GPT to Gemini
    //

    qDebug() << start << " / " << end;
    qDebug() << mPixmap;
    // Calculate width
    int x = std::min(start.x(), end.x());
    int y = std::min(start.y(), end.y());
    int width = qAbs(end.x() - start.x());
    int height = qAbs(end.y() - start.y());

    // Create the rectangle using start point and calculated width/height
    QRectF rect(x, y, width, height);
    qDebug() << rect;

    QPixmap newPixmap = mPixmap.copy(rect.x(), rect.y(), rect.width(), rect.height());
    close();
    emit onFinishScreenshot(newPixmap);
}

