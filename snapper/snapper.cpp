#include "snapper.h"
#include "logcat/logcat.h"

void Snapper::screenshot() {
    QScreen *screen = QApplication::primaryScreen();
    QPixmap screenshot = screen->grabWindow(0, 0, 0, screen->size().width(), screen->size().height());

    Logcat::logInfo("Took Screenshot.");

    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save Screenshot", "", "Images (*.png *.jpg *.bmp");
    if (!filePath.isEmpty())
    {
        screenshot.save(filePath);
    }
}
