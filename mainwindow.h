#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QSettings>

#include <ui_components/capturecontainer.h>

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
        CaptureContainer *captureContainer;
        QHBoxLayout *tempBox;
        QLabel *capturePreviewLabel, *captureDisplayLabel;
        //
        // void updateCapture();

    public:
        MainWindow(QWidget *parent = nullptr);
        void resizeEvent(QResizeEvent *event);
        ~MainWindow();

    private slots:
        void on_snapButton_clicked();
        void on_presetsComboBox_currentIndexChanged(int i);

    private:
        Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
