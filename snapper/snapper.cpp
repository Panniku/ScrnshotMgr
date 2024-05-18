#include "snapper.h"
#include "logcat/logcat.h"

#include <mainwindow.h>

void Snapper::screenshot() {
    QScreen *screen = QApplication::primaryScreen();
    QPixmap screenshot = screen->grabWindow(0, 0, 0, screen->size().width(), screen->size().height());

    Logcat::log(LogType::Warning, "Snapper", "Took Screenshot.");

    QSettings settings;
    QString saveDir = settings.value("Application/SnapsDir").toString();
    QString name = "/Snap_";
    QString saveDT = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QString saveFileName = saveDir + name + saveDT;

    qDebug() << saveDir + " / " +  saveFileName;

    // QString filePath = QFileDialog::getSaveFileName(nullptr, "Save Screenshot", saveFileName, "Images (*.png *.jpg *.bmp");

    if(saveDir == "")
    {
        qDebug() << "No directory.";
        QString filePath = QFileDialog::getSaveFileName(nullptr, "Save Screenshot", saveFileName, "Images (*.png *.jpg *.bmp");
        if(!filePath.isEmpty()){
            screenshot.save(filePath);
            qDebug() << "saved as" << filePath;
            //
            QDir path(filePath);
            QFileInfo f(path, path.absolutePath());
            // settings.setValue("application/snaps_dir", f.absolutePath());
            // settings.sync();
            // Logcat::logInfo("Set directory: " + f.absolutePath());
            qDebug() << f.absolutePath();
        }

    }
    else
    {
        qDebug() << "!! Found a directory.";
        qDebug() << saveDir;
        qDebug() << saveDir + name + saveDT;
        screenshot.save(saveDir + name + saveDT + ".png");
        qDebug() << settings.value("Application/SnapsDir");
    }

    settings.sync();
    qDebug() << settings.status();


    // //  QFileDialog::getSave
    // if (!filePath.isEmpty())
    // {

    //     screenshot.save(filePath);

    //     if(saveDir == "") {
    //         // Logcat::logInfo("filePath: " + filePath);

    //         // QString thisDir = path.dirName();

    //     }

    //     Logcat::logInfo("Saved to directory: " + f.absoluteFilePath());
    // }
}
