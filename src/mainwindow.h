#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QSettings>
#include <QSplitter>

#include "ui_components/capture/capturecontainer.h"
#include "ui_components/capture/capturesimple.h"
#include "ui_components/presets/presetobject.h"

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
        QSplitter *capturesSplitter;
        CaptureSimple *lastCapturePreview;
        QLabel *lastPreview, *lastTemp;
        QListWidget *captureList;
        QGroupBox *capturePreviewBox;
        //
        PresetType currentType;
        QRectF currentRect;
        //
        // void updateCapture();
        bool isHide = true;
        bool isHideTrigger = false;

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

        // Events
        void hideEvent(QHideEvent *event);
        void resizeEvent(QResizeEvent *event);
        void closeEvent(QCloseEvent *event);

        //


    private slots:
        void on_snapButton_clicked();
        void on_presetsComboBox_currentIndexChanged(int i);

        void on_toggleVisibility_toggled(bool checked);
        void on_toggleOnTop_toggled(bool checked);

        //
        void delayedScreenshot();
        void onScreenshotFinish(QPixmap &pixmap);
        void onScreenshotCancel();

    private:
        Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
