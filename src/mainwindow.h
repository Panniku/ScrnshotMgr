#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QSettings>

#include "ui_components/capture/capturecontainer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    private:
        QPixmap capturePixmap;
        QRectF captureRect;
        CaptureContainer *captureContainer;
        QHBoxLayout *tempBox;
        QLabel *captureRenderLabel, *captureDisplayLabel;
        QGroupBox *capturePreviewBox;
        //
        // void updateCapture();

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

        // Events
        void resizeEvent(QResizeEvent *event);
        void closeEvent(QCloseEvent *event);

    private slots:
        void on_snapButton_clicked();
        void on_presetsComboBox_currentIndexChanged(int i);

    private:
        Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
