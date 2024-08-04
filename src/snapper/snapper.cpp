#include "snapper.h"
#include "snapinterface.h"
#include "../logcat/logcat.h"

#include <QLabel>

void Snapper::screenshot(QWidget *parent, PresetType type, QRectF dimen) {

    mParent = parent;
    qDebug() << "parent: " << mParent;

    QScreen *screen = QApplication::primaryScreen();
    QPixmap screenshot = screen->grabWindow(0, 0, 0, screen->size().width(), screen->size().height());

    if(type == PresetType::Freedraw) {

        // mParent->hide();
        // si = new SnapInterface(mParent, &screenshot);
        si->showFullScreen();
        // connect(si, &SnapInterface::onFinish, this, &Snapper::onReceiveFinalImage);
        // si->setScreenshot(screenshot);



        // sn->
        // Last statement
        // sn->showMaximized();


    } else if (PresetType::Fullscreen) {

        if (PresetType::Custom){
            //
        }

    } else {
        // Log error?
        // Or just default idk
    }






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

void Snapper::onReceiveFinalImage(QPixmap &pixmap)
{
    si->close();
    qDebug() << "received image";
    QPixmap screenshot = pixmap;

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
}
