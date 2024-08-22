#include "drawsnap.h"
#include "../logcat/logcat.h"

DrawSnap::DrawSnap(QWidget *parent) : QFrame(parent){
    pos = new QLabel(this);
    pos->setAlignment(Qt::AlignCenter);
    pos->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void DrawSnap::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        startPoint = event->pos();
        endPoint = startPoint;
        isDrawing = true;
        update();
    }
}

void DrawSnap::mouseMoveEvent(QMouseEvent *event) {
    if (isDrawing) {
        endPoint = event->pos();
        update();
    }
}

void DrawSnap::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        endPoint = event->pos();
        isDrawing = false;
        update();
        //
        emit drawingFinished(startPoint, endPoint);
    }
}

void DrawSnap::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setPen(Qt::white);
    // This is required such that the text doesnt clip...
    pos->setFixedSize(128, 128);
    pos->setStyleSheet("color: white;");
    if (isDrawing) {
        Logcat::log(LogType::Critical, "DrawSnap", "IS Drawinng");
        QRect r(startPoint, endPoint);
        painter.drawRect(r);
        //
        int x = std::min(startPoint.x(), endPoint.x());
        int y = std::min(startPoint.y(), endPoint.y());
        int width = qAbs(endPoint.x() - startPoint.x());
        int height = qAbs(endPoint.y() - startPoint.y());

        // Gemini magic
        int labelX = x + width / 2 - pos->width() / 2;
        int labelY = y + height / 2 - pos->height() / 2;

        //
        QString postext = QString::number(width) + "x" + QString::number(height);
        pos->setText(postext);
        pos->move(labelX, labelY);
    }
}
