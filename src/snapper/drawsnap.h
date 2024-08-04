#ifndef DRAWINGFRAME_H
#define DRAWINGFRAME_H

#include <QWidget>
#include <QPoint>
#include <QFrame>
#include <QMouseEvent>
#include <QPainter>
#include <QLabel>

class DrawSnap : public QFrame {
    Q_OBJECT

public:
    explicit DrawSnap(QWidget *parent = nullptr);

private:
    QLabel *pos;
    QPoint startPoint;
    QPoint endPoint;
    bool isDrawing = false;

private slots:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

signals:
    void drawingFinished(QPoint start, QPoint end);
};

#endif // DRAWINGFRAME_H
