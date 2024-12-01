#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QSettings>
#include <QSpinBox>
#include <QSplitter>
#include <QVideoWidget>
#include <QPushButton>
#include <QMediaCaptureSession>

#include "qimagecapture.h"
#include "qscreencapture.h"
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
        QSettings *settings;
        QPixmap capture;
        float displayWidth, displayHeight;
        QRectF captureRect;

        //
        QScreenCapture *screenCapture;
        QImageCapture *imageCapture;
        QMediaCaptureSession *mediaCaptureSession;

        // Toolbar stuff
        QPushButton *snapButton, *recordButton;
        QComboBox *presetsComboBox;
        QPushButton *toggleVisibility, *toggleOnTop;

        // Preset list stuff
        QFrame *presetRoot;
        QLineEdit *presetSearchItem;
        QPushButton *presetAddItem;
        QListWidget *presetListWidget;

        // Preview
        CaptureContainer *captureContainer;
        QHBoxLayout *tempBox;
        QVideoWidget *displayRender;
        // QLabel *captureDisplayLabel;
        QGroupBox *capturePreviewBox;

        // Session captures View
        QSplitter *capturesSplitter;
        CaptureSimple *lastCapturePreview;
        QLabel *lastPreview, *lastTemp;
        QListWidget *captureList;
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

        // Functions
        void setupUi();
        void setupIcons();


    private slots:

        void addPreset();

        //
        void snapButton_click();
        void recordButton_click();
        void presetsComboBoxIndexChanged(int i);

        void toggleVisibility_toggled(bool checked);
        void toggleOnTop_toggled(bool checked);

        //
        void imageCaptured(int id, QImage image);

        //
        void screenshot();
        void onScreenshotFinish(QPixmap &pixmap);
        void onScreenshotCancel();

        void on_actionPreferences_triggered();

    private:
        Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
