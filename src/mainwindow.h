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
        float displayWidth, displayHeight;
        QRectF captureRect;

        // Central View Containers
        QFrame *centralViewContainer;
        // QSplitter *centralViewSplitter; // unknown

        // Editor View
        QLineEdit *nameEdit;
        QComboBox *typeEdit;
        QSpinBox *xEdit, *yEdit, *widthEdit, *heightEdit;

        // Preview
        CaptureContainer *captureContainer;
        QHBoxLayout *tempBox;
        QVideoWidget *displayRender;
        QLabel *captureDisplayLabel;
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
