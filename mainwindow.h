#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

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
        //
        void updateCapture();


    public:
        MainWindow(QWidget *parent = nullptr);
        void resizeEvent(QResizeEvent *event);
        ~MainWindow();

    private slots:
        void on_snapButton_clicked();

    private:
        Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
