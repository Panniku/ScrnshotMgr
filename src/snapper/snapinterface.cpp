#include "drawsnap.h"
#include "snapinterface.h"
#include "ui_snapinterface.h"

#include <QLabel>
#include "../logcat/logcat.h"
#include "../utils/utils.h"

SnapInterface::SnapInterface(QWidget *parent, PresetType type, QRectF rect, QPixmap screenshot)
    : QMainWindow(parent)
    , ui(new Ui::SnapInterface)
{
    ui->setupUi(this);

    setCursor(Qt::CrossCursor);
    // setMouseTracking(true);

    QScreen *screen = qApp->primaryScreen();

    mPixmap = screenshot;

    QPointF start = rect.topLeft();
    QPointF end = rect.bottomRight();

    // connect(this, &SnapInterface::onFinishCrop, this, &SnapInterface::onDrawingFinished);

    if(type == Fullscreen || type == Custom) {
        qDebug() << "Selected either fullscreen or custom";
        qDebug() << rect << " / " << type;

        // QPixmap px = crop(start.toPoint(), end.toPoint());
        cropOnly(start.toPoint(), end.toPoint());

    } else if (type == Freedraw) {
        Logcat::log(LogType::Info, "Snapinterface","freedraw yippe");
        qDebug() << rect;
        //

        QPalette p;
        QBrush b = p.brush(QPalette::ButtonText);

        QPalette radioPalette;
        radioPalette.setColor(QPalette::Normal, QPalette::Base, p.base().color());

        ui->radioScreenshot->setPalette(radioPalette);
        ui->radioRecord->setPalette(radioPalette);

        QPainter painter(&screenshot);
        painter.drawPixmap(0, 0, screen->size().width(), screen->size().height(), screenshot);
        painter.fillRect(QRectF(0, 0, screen->size().width(), screen->size().height()), QBrush(QColor(0, 0, 0, 64)));

        // setWindowFlags(Qt::FramelessWindowHint);

        // Controls setup?
        QPixmap screenshotIcon = QPixmap(":/res/icons/ph--camera-light.svg");
        ui->radioScreenshot->setIcon(QIcon(Utils::getMaskedRecoloredIconPixmap(screenshotIcon, b)));

        QPixmap recordIcon = QPixmap(":/res/icons/ph--video-camera-light.svg");
        ui->radioRecord->setIcon(QIcon(Utils::getMaskedRecoloredIconPixmap(recordIcon, b)));

        QPixmap closeIcon = QPixmap(":/res/icons/ph--x-light.svg");
        ui->closeWindow->setIcon(QIcon(Utils::getMaskedRecoloredIconPixmap(closeIcon, b)));

        //TODO
        QLabel *preview = new QLabel(ui->frame);
        preview->setPixmap(screenshot);
        preview->setGeometry(0, 0, screen->size().width(), screen->size().height());

        DrawSnap *drawSnap = new DrawSnap(ui->frame);
        drawSnap->setGeometry(0, 0, screenshot.width(), screenshot.height());
        //connect(drawSnap, &DrawSnap::onMouseReleased, this, &SnapInterface::onMouseFinished);
        connect(drawSnap, &DrawSnap::drawingFinished, this, &SnapInterface::onDrawingFinished);
        qDebug() << drawSnap->geometry();
        //
        ui->controlsContainer->raise();
        ui->controlsContainer->setGeometry
            (
                (screen->size().width() / 2) - (ui->controlsContainer->width() / 2),
                6,
                ui->controlsContainer->geometry().width(),
                ui->controlsContainer->geometry().height()
                );

        showFullScreen();
        Logcat::log(LogType::Debug, "SnapInterface", QString::number(drawSnap->geometry().width()) + " " + QString::number(drawSnap->geometry().height()));
    }
}

SnapInterface::~SnapInterface()
{
    delete ui;
}

QPixmap SnapInterface::crop(QPoint start, QPoint end)
{
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

    return mPixmap.copy(rect.x(), rect.y(), rect.width(), rect.height());
}

void SnapInterface::on_closeWindow_clicked()
{
    SnapInterface::close();
    emit onCancel();
}

// will this work
void SnapInterface::cropOnly(QPoint start, QPoint end) {

    QPixmap newPixmap = crop(start, end);
    qDebug() << "are you the emitter because you are a signal? or are you a signal because you are the emitter?";
    emit onFinishCrop(newPixmap);
    close();
}

// This is the event when the DrawSnap is finished
void SnapInterface::onDrawingFinished(QPoint start, QPoint end) {
    qDebug() << "mouse done";

    QPixmap newPixmap = crop(start, end);
    qDebug() << "emitting..." << newPixmap;
    emit onFinishScreenshot(newPixmap);
    close();
}

