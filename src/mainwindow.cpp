#include <QDebug>
#include <QPainter>
#include <QScrollArea>
#include <QStandardPaths>
#include <QTimer>
#include <QPair>
#include <QLibraryInfo>
#include <QScreenCapture>
#include <QImageCapture>
#include <QMediaCaptureSession>
#include "configmanager/settingsdialog.h"
#include "qclipboard.h"
#include "qlineedit.h"
#include <QDockWidget>
#include "qradiobutton.h"
#include "snapper/snapcrop.h"
#include "snapper/snapinterface.h"
#include "ui_components/capture/capturerenderer.h"
#include "ui_components/logtextedit.h"
#include "ui_components/presets/editor/preseteditor.h"
#include "ui_components/presets/presetobject.h"
#include "configmanager/configmanager.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logcat/logcat.h"
#include "./ui_components/capture/captureitem.h"
#include "utils/utils.h"
#include "configmanager/settingsdialog.h"
#include <QStyleHints>

QPixmap *mCaptureRender;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // CONFIGMANAGER
    ConfigManager configManager;
    Logcat *logcat = new Logcat();
    // logcat->setLogcat(logTextEdit);

    configManager.initDefault();
    Logcat::log(LogType::Warning, "NOTICE", "If colors are bugged, this may be due to how we initialize colors. Sorry for the inconvenience!");
    logcat->log(LogType::Info, "―――――――――――――――――――――――――――――――――――――――――――――", "");

    // :3
    settings = configManager.getSettings();

    QString theme = settings->value("Application/Theme").toString();

    // windows only for now :3
    #if defined(Q_OS_WIN32)
        if (theme == "light" || (theme == "system" && Utils::getSystemTheme() == "light")) {
            //
        } else if (theme == "dark" || (theme == "system" && Utils::getSystemTheme() == "dark")) {
            qApp->setStyle("fusion");
            QPalette p;
            // QPalette winColor;
            // p.setColor(QPalette::Normal, QPalette::Window, p.base().color());

            qApp->setPalette(p);
        }

    #endif

    QScreen *screen = QApplication::primaryScreen();
    displayWidth = screen->size().width();
    displayHeight = screen->size().height();

    /******************************************************************************
     * Toolbar init
     */
    snapButton = new QPushButton("Snap");
    snapButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    connect(snapButton, &QPushButton::clicked, this, &MainWindow::snapButton_click);

    recordButton = new QPushButton("Record");
    recordButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    connect(recordButton, &QPushButton::clicked, this, &MainWindow::recordButton_click);

    presetsComboBox = new QComboBox();
    presetsComboBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    connect(presetsComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::presetsComboBoxIndexChanged);

    QWidget *primarySpacer = new QWidget();
    primarySpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    toggleVisibility = new QPushButton();
    toggleVisibility->setCheckable(true);
    toggleVisibility->setFlat(true);
    toggleVisibility->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    connect(toggleVisibility, &QPushButton::toggled, this, &MainWindow::toggleVisibility_toggled);

    toggleOnTop = new QPushButton();
    toggleOnTop->setCheckable(true);
    toggleOnTop->setFlat(true);
    toggleOnTop->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    connect(toggleOnTop, &QPushButton::toggled, this, &MainWindow::toggleOnTop_toggled);

    ui->primaryToolbar->addWidget(snapButton);
    ui->primaryToolbar->addWidget(recordButton);
    ui->primaryToolbar->addWidget(presetsComboBox);
    ui->primaryToolbar->addWidget(primarySpacer);
    ui->primaryToolbar->addWidget(toggleVisibility);
    ui->primaryToolbar->addWidget(toggleOnTop);

    /******************************************************************************/

    /******************************************************************************
     * Preview list init
     */

    presetRoot = new QFrame();
    QVBoxLayout *presetLayout = new QVBoxLayout(presetRoot);
    presetLayout->setContentsMargins(QMargins(0, 0, 0, 0));
    presetLayout->setSpacing(0);
    ui->presetListToolbar->addWidget(presetRoot);

    QHBoxLayout *presetWidgets = new QHBoxLayout();
    presetWidgets->setContentsMargins(QMargins(0, 0, 0, 0));
    presetWidgets->setSpacing(0);

    presetLayout->addLayout(presetWidgets);

    presetSearchItem = new QLineEdit();
    presetSearchItem->setPlaceholderText("Search");
    presetSearchItem->setClearButtonEnabled(true);
    presetWidgets->addWidget(presetSearchItem);

    presetAddItem = new QPushButton();
    connect(presetAddItem, &QPushButton::pressed, this, &MainWindow::addPreset);
    presetWidgets->addWidget(presetAddItem);

    presetListWidget = new QListWidget();
    presetLayout->addWidget(presetListWidget);

    ui->presetListToolbar->setFixedWidth(160);

    /******************************************************************************/

    /******************************************************************************
     * Capture preview init
     */

    captureContainer = new CaptureContainer();
    captureContainer->setDimens(displayWidth, displayHeight);
    ui->centralwidget->layout()->addWidget(captureContainer);

    capturePreviewBox = new QGroupBox(captureContainer);
    capturePreviewBox->setStyleSheet("border: 2px solid red;");

    captureContainer->setPreviewBox(capturePreviewBox);
    captureContainer->setRect(captureRect);
    captureContainer->setDisplayText("Display 1 / " + QString::number(displayWidth) + "x" + QString::number(displayHeight));

    screenCapture = new QScreenCapture(qApp->primaryScreen());
    screenCapture->start();
    mediaCaptureSession = new QMediaCaptureSession();
    mediaCaptureSession->setScreenCapture(screenCapture);
    mediaCaptureSession->setVideoOutput(captureContainer->getDisplayRender());

    imageCapture = new QImageCapture(screenCapture);
    mediaCaptureSession->setImageCapture(imageCapture);

    connect(imageCapture, &QImageCapture::imageCaptured, this, &MainWindow::imageCaptured);

    /******************************************************************************/

    /******************************************************************************
     * Capture list init
     */

    capturesSplitter = new QSplitter();
    capturesSplitter->setOrientation(Qt::Vertical);
    ui->capturesToolbar->addWidget(capturesSplitter);

    lastCapturePreview = new CaptureSimple();
    lastCapturePreview->setStyleSheet("border: 1px solid gray;");
    capturesSplitter->addWidget(lastCapturePreview);
    lastPreview = new QLabel(lastCapturePreview);
    lastPreview->setMinimumSize(1, 1);
    lastPreview->setScaledContents(false);
    lastPreview->setAlignment(Qt::AlignCenter);
    lastPreview->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QImage image(QSize(screen->size().width(), screen->size().height()),QImage::Format_RGB32);
    QPainter painter(&image);
    QPalette p;
    painter.fillRect(QRectF(0,0,screen->size().width(), screen->size().height()), p.brush(QPalette::Base));
    QPixmap lsp = QPixmap::fromImage(image);
    lastPreview->setPixmap(lsp);
    lastCapturePreview->setCapture(lastPreview);
    lastCapturePreview->setCapturePixmap(lsp);

    lastTemp = new QLabel(lastCapturePreview);
    lastTemp->setText("This area renders the \nlast taken screenshot.\nNo screenshots taken yet!");
    lastCapturePreview->setTempLabel(lastTemp);

    captureList = new QListWidget(capturesSplitter);
    capturesSplitter->addWidget(captureList);

    ui->capturesToolbar->setFixedWidth(160);

    /******************************************************************************/

    // WORK IN PROGRESS
    // WORK IN PROGRESS
    // WORK IN PROGRESS
    // WORK IN PROGRESS 


    //QScreen *screen = QApplication::primaryScreen();
    // QPixmap screenshot = screen->grabWindow(0, 0, 0, screen->size().width(), screen->size().height());

    PresetObject freedraw("freedraw", 0, "Free draw", PresetType::Freedraw, QRectF(0, 0, 0, 0));
    PresetObject fullscreen("fullscreen", 1, "Fullscreen", PresetType::Fullscreen, QRectF(0, 0, screen->size().width(), screen->size().height()));
    PresetObject layout1("layout1", 2, "Layout 1", PresetType::Custom, QRectF(64, 64, 256, 256));
    PresetObject layout2("layout2", 3, "Layout 2", PresetType::Custom, QRectF(128, 256, 300, 300));

    presetsComboBox->addItem(freedraw.getRegionName(), freedraw.toVariant());
    presetsComboBox->addItem(fullscreen.getRegionName(), fullscreen.toVariant());
    presetsComboBox->addItem(layout1.getRegionName(), layout1.toVariant());
    presetsComboBox->addItem(layout2.getRegionName(), layout2.toVariant());


    QListWidgetItem *freedrawitem = new QListWidgetItem("Free draw");
    QListWidgetItem *fullscreenitem = new QListWidgetItem("Fullscreen");
    QListWidgetItem *layout1item = new QListWidgetItem("Layout 1");
    freedrawitem->setData(Qt::UserRole, freedraw.toVariant());
    fullscreenitem->setData(Qt::UserRole, fullscreen.toVariant());
    layout1item->setData(Qt::UserRole, layout1.toVariant());

    presetListWidget->addItem(freedrawitem);
    presetListWidget->addItem(fullscreenitem);
    presetListWidget->addItem(layout1item);

    // Misc setup
    presetSearchItem->setClearButtonEnabled(true);
    setupIcons();

    QLabel *n = new QLabel("ScrnMgr v0 ALPHA :3");
    ui->statusbar->addWidget(n);
    resize(640, 360);

    qDebug() << Utils::getSystemTheme();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::hideEvent(QHideEvent *event)
{
    QMainWindow::hideEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
    Logcat::log(LogType::Info, "MainWindow", "Ending process.");
}

void MainWindow::setupUi()
{
    // may be unused :33333333
}

void MainWindow::setupIcons()
{
    QPalette p;
    QBrush b = p.brush(QPalette::Text);
    // Misc search icon
    QPixmap searchPixmap = QPixmap(":/res/icons/ph--magnifying-glass.svg");
    presetSearchItem->addAction(QIcon(Utils::getMaskedRecoloredIconPixmap(searchPixmap, b)), QLineEdit::LeadingPosition);

    // Add icon
    QPixmap addPixmap = QPixmap(":/res/icons/ph--plus.svg");
    presetAddItem->setIcon(QIcon(Utils::getMaskedRecoloredIconPixmap(addPixmap, b)));

    // Visibility icon
    QPixmap visibilityPixmap = QPixmap(":/res/icons/ph--eye-slash-light.svg");
    QPixmap visibilityPixmapOn = QPixmap(":/res/icons/ph--eye-light.svg");
    QIcon visibilityIcon;
    visibilityIcon.addPixmap(
        Utils::getMaskedRecoloredIconPixmap(visibilityPixmap, b),
        QIcon::Normal,
        QIcon::Off
        );
    visibilityIcon.addPixmap(
        Utils::getMaskedRecoloredIconPixmap(visibilityPixmapOn, b),
        QIcon::Normal,
        QIcon::On
        );
    toggleVisibility->setIcon(visibilityIcon);

    // AlwaysOnTop icon
    QPixmap topPixmap = QPixmap(":/res/icons/ph--push-pin-slash-light.svg");
    QPixmap topPixmapOn = QPixmap(":/res/icons/ph--push-pin-light.svg");
    QIcon topIcon;
    topIcon.addPixmap(
        Utils::getMaskedRecoloredIconPixmap(topPixmap, b),
        QIcon::Normal,
        QIcon::Off
        );
    topIcon.addPixmap(
        Utils::getMaskedRecoloredIconPixmap(topPixmapOn, b),
        QIcon::Normal,
        QIcon::On
        );
    toggleOnTop->setIcon(topIcon);
}

void MainWindow::addPreset() {
    PresetEditor *editor = new PresetEditor();
    editor->exec();
}

void MainWindow::snapButton_click()
{
    // This does nothing but check if it should cooldown and trigger or trigger immediately
    if (isHide) {
        hide();
        QTimer *timer = new QTimer(this);
        timer->setInterval(400); // hardcoded, could change anytime
        timer->setSingleShot(true);

        connect(timer, &QTimer::timeout, this, &MainWindow::screenshot);
        timer->start();
    } else {
        screenshot();
    }
}

void MainWindow::recordButton_click()
{
    // meowmeowmeowmeowmeowmeowmeowmeow
}

void MainWindow::imageCaptured(int id, QImage image)
{
    capture = QPixmap::fromImage(image);

    if(currentType == Freedraw) {
        SnapInterface *sWin = new SnapInterface(this, currentType, currentRect, capture);
        connect(sWin, &SnapInterface::onFinishScreenshot, this, &MainWindow::onScreenshotFinish);
        connect(sWin, &SnapInterface::onCancel, this, &MainWindow::onScreenshotCancel);
    } else {
        QPointF start = currentRect.topLeft();
        QPointF end = currentRect.bottomRight();
        SnapCrop *crop = new SnapCrop();
        connect(crop, &SnapCrop::onFinishCrop, this, &MainWindow::onScreenshotFinish);
        crop->crop(capture, start.toPoint(), end.toPoint());
    }
}

void MainWindow::screenshot()
{
    // This method solemnly exists to trigger this, to trigger the function above.
    imageCapture->capture();
}


void MainWindow::onScreenshotFinish(QPixmap &pixmap)
{
    qDebug() << "it worked";
    if (isHide) show();
    lastPreview->setPixmap(pixmap);
    lastCapturePreview->triggerSnapped();
    lastCapturePreview->setCapturePixmap(pixmap);
    lastCapturePreview->updatePixmap();

    QImage img(pixmap.toImage());
    QClipboard *clip = QApplication::clipboard();
    clip->setImage(img, QClipboard::Clipboard);

    CaptureItem *item = new CaptureItem(pixmap, "Item 1");
    // QPushButton *b = new QPushButton();
    QListWidgetItem *listitem = new QListWidgetItem();
    listitem->setSizeHint(QSize(1, item->height()));
    captureList->addItem(listitem);
    captureList->setItemWidget(listitem, item);

    qDebug() << captureList->children().at(0)->children();

    // save function
    QSettings settings;
    QString saveDir = settings.value("Application/SnapsDir").toString();
    QString name = "/Snap_";
    QString saveDT = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QString saveFileName = saveDir + name + saveDT;

    qDebug() << saveDir + " / " +  saveFileName;

    if(saveDir == "") {
        qDebug() << "No directory.";
        QString filePath = QFileDialog::getSaveFileName(nullptr, "Save Screenshot", saveFileName, "Images (*.png *.jpg)");
        if(!filePath.isEmpty()){
            pixmap.save(filePath);
            qDebug() << "saved as" << filePath;
            //
            // QDir path(filePath);
            // QFileInfo f(path, path.absolutePath());
            // qDebug() << f.absolutePath();
        }
    } else {
        pixmap.save(saveDir + name + saveDT + ".png");
        qDebug() << settings.value("Application/SnapsDir");
    }
}

void MainWindow::onScreenshotCancel()
{
    if (isHide) show();
}

void MainWindow::presetsComboBoxIndexChanged(int i)
{
    qDebug() << i;
    QString name = presetsComboBox->itemText(i);
    QString _preset = presetsComboBox->itemData(i).toString();

    int firstSemicolonIndex = _preset.indexOf(';');
    QString preset = _preset.mid(firstSemicolonIndex + 1);

    //qDebug() << "preset " << preset;
    PresetObject obj(name.toLower(), preset);
    qDebug() << "name: "    << obj.getRegionName();
    qDebug() << "name: "    << obj.getRect();
    qDebug() << "name: "    << obj.getPresetIntType();

    QRectF newRect = obj.getRect();
    // capturePixmap = render->drawRenderedImage(displayWidth, displayHeight, newRect);
    // captureContainer->setCapturePixmap(&capturePixmap);
    captureContainer->setRect(newRect);
    captureContainer->setPreviewName(name);
    captureContainer->updateCapture();
    //
    currentType = static_cast<PresetType>(obj.getPresetIntType());
    currentRect = obj.getRect();
}


void MainWindow::toggleVisibility_toggled(bool checked)
{
    isHide = !checked;
}


void MainWindow::toggleOnTop_toggled(bool checked)
{
    // hide() show() is required because of how Qt handles window flags
    // https://forum.qt.io/topic/60642/framelesswindowhint-fails-at-runtime-on-mainwindow/13
    hide();
    setWindowFlag(Qt::WindowStaysOnTopHint, checked);
    show();
}

void MainWindow::on_actionPreferences_triggered()
{
    SettingsDialog *dialog = new SettingsDialog();
    dialog->exec();
}

