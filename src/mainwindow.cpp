#include <QDebug>
#include <QPainter>
#include <QScrollArea>
#include <QStandardPaths>
#include <QTimer>
#include <QPair>
#include <QLibraryInfo>
#include "qclipboard.h"
#include "src/ui_mainwindow.h"
#include <QDockWidget>
#include "ui_components/capture/capturerenderer.h"
#include "ui_components/logtextedit.h"
#include "ui_components/presets/presetobject.h"
#include "snapper/snapper.h"
#include "configmanager/configmanager.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logcat/logcat.h"
#include "./ui_components/capture/captureitem.h"

QPixmap *mCaptureRender;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // TOOD: remove this
    QFont font("Consolas");
    LogTextEdit *logTextEdit = new LogTextEdit();
    logTextEdit->setFont(font);
    logTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);
    logTextEdit->setWordWrapMode(QTextOption::NoWrap);
    ui->consoleTab->layout()->addWidget(logTextEdit);

    // CONFIGMANAGER
    ConfigManager *configManager = new ConfigManager();
    Logcat *logcat = new Logcat();
    logcat->setLogcat(logTextEdit);

    configManager->initDefault();
    Logcat::log(LogType::Warning, "NOTICE", "If colors are bugged, this may be due to how we initialize colors. Sorry for the inconvenience!");
    logcat->log(LogType::Info, "―――――――――――――――――――――――――――――――――――――――――――――", "");



    QScreen *screen = QApplication::primaryScreen();


    // Capture preview initialization
    captureContainer = new CaptureContainer();
    captureContainer->setStyleSheet("border: 1px solid gray;");
    ui->splitter->addWidget(captureContainer);

    // THIS IS TEMPORARY
    // THIS IS TEMPORARY
    // THIS IS TEMPORARY
    capturesSplitter = new QSplitter();
    ui->splitter->addWidget(capturesSplitter);
    capturesSplitter->setOrientation(Qt::Vertical);

    lastCapturePreview = new CaptureSimple();
    lastCapturePreview->setStyleSheet("border: 1px solid gray;");
    capturesSplitter->addWidget(lastCapturePreview);
    lastPreview = new QLabel(lastCapturePreview);
    lastPreview->setMinimumSize(1, 1);
    lastPreview->setScaledContents(false);
    lastPreview->setAlignment(Qt::AlignCenter);
    lastPreview->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QImage image(QSize(1920, 1080),QImage::Format_RGB32);
    QPainter painter(&image);
    painter.fillRect(QRectF(0,0,1920,1920),QColor(200, 200, 200, 255));
    QPixmap lsp = QPixmap::fromImage(image);
    lastPreview->setPixmap(lsp);
    lastCapturePreview->setCapture(lastPreview);
    lastCapturePreview->setCapturePixmap(lsp);

    lastTemp = new QLabel(lastCapturePreview);
    lastTemp->setText("This area renders the last taken screenshot.\nNo screenshots taken yet.");
    lastCapturePreview->setTempLabel(lastTemp);

    captureList = new QListWidget(capturesSplitter);
    capturesSplitter->addWidget(captureList);

    captureRenderLabel = new QLabel(captureContainer);
    captureRenderLabel->setPixmap(capturePixmap);
    captureRenderLabel->setMinimumSize(1, 1);
    captureRenderLabel->setScaledContents(false);
    captureRenderLabel->setAlignment(Qt::AlignCenter);
    captureRenderLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    captureRenderLabel->setStyleSheet("border: 1px solid black;");

    captureDisplayLabel = new QLabel(captureContainer);
    captureDisplayLabel->setText("Display 1");
    captureDisplayLabel->setAlignment(Qt::AlignBottom | Qt::AlignRight);

    capturePreviewBox = new QGroupBox(captureContainer);
    capturePreviewBox->setTitle("Display 1 PLACEHOLDER");
    // capturePreviewBox->setAlignment(Qt::AlignCenter);
    capturePreviewBox->setStyleSheet("border: 2px solid red;");

    captureContainer->setCapture(captureRenderLabel);
    captureContainer->setDisplayLabel(captureDisplayLabel);
    // captureContainer->setDisplayText()
    captureContainer->setPreviewBox(capturePreviewBox);
    captureContainer->setCapturePixmap(&capturePixmap);
    captureContainer->setRect(captureRect);

    // WORK IN PROGRESS
    // WORK IN PROGRESS
    // WORK IN PROGRESS
    // WORK IN PROGRESS 


    //QScreen *screen = QApplication::primaryScreen();
    // QPixmap screenshot = screen->grabWindow(0, 0, 0, screen->size().width(), screen->size().height());

    PresetObject freedraw("freedraw", 0, "Free draw", PresetType::Freedraw, QRectF(0, 0, 0, 0));
    PresetObject fullscreen("fullscreen", 1, "Fullscreen", PresetType::Fullscreen, QRectF(0, 0, screen->size().width(), screen->size().height()));
    PresetObject layout1("layout1", 2, "Layout 1", PresetType::Custom, QRectF(64, 64, 256, 256));

    ui->presetsComboBox->addItem("Free draw", freedraw.toVariant());
    ui->presetsComboBox->addItem("Fullscreen",fullscreen.toVariant());
    ui->presetsComboBox->addItem("Layout 1", layout1.toVariant());


    QListWidgetItem *freedrawitem = new QListWidgetItem("Free draw");
    QListWidgetItem *fullscreenitem = new QListWidgetItem("Fullscreen");
    QListWidgetItem *layout1item = new QListWidgetItem("Layout 1");
    freedrawitem->setData(Qt::UserRole, freedraw.toVariant());
    fullscreenitem->setData(Qt::UserRole, fullscreen.toVariant());
    layout1item->setData(Qt::UserRole, layout1.toVariant());

    ui->presetListWidget->addItem(freedrawitem);
    ui->presetListWidget->addItem(fullscreenitem);
    ui->presetListWidget->addItem(layout1item);

    // Misc setup
    ui->presetSearchItem->setClearButtonEnabled(true);
    ui->presetSearchItem->addAction(QIcon(":/resources_root/icons/ph--magnifying-glass.svg"), QLineEdit::LeadingPosition);

    // ui->splitter->handle(0)->setStyleSheet("background-color: green");

    // some test
    // qDebug()  << ":/themes/dark.qss";
    // Currently, this only applies a border to the splitter handle lol
    QFile file(":/resources_root/themes/dark.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);
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

void MainWindow::on_snapButton_clicked()
{
    if (isHide) {
        hide();
        QTimer *timer = new QTimer(this);
        timer->setInterval(400); // hardcoded, could change anytime
        timer->setSingleShot(true);
        connect(timer, &QTimer::timeout, this, &MainWindow::delayedScreenshot);
        timer->start();
    } else {
        captureList->addItem("meow, taking screenshot");
        SnapInterface *sWin = new SnapInterface(this, currentType, currentRect);
        connect(sWin, &SnapInterface::onFinishScreenshot, this, &MainWindow::onScreenshotFinish);
        connect(sWin, &SnapInterface::onCancel, this, &MainWindow::onScreenshotCancel);
    }
}


void MainWindow::delayedScreenshot()
{
    // captureList->addItem("meow, taking screenshot");
    SnapInterface *sWin = new SnapInterface(this, currentType, currentRect);
    connect(sWin, &SnapInterface::onFinishScreenshot, this, &MainWindow::onScreenshotFinish);
    connect(sWin, &SnapInterface::onCancel, this, &MainWindow::onScreenshotCancel);
}


void MainWindow::onScreenshotFinish(QPixmap &pixmap)
{
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
}

void MainWindow::onScreenshotCancel()
{
    if (isHide) show();
}

void MainWindow::on_presetsComboBox_currentIndexChanged(int i)
{
    qDebug() << i;
    QString name = ui->presetsComboBox->itemText(i);
    QString _preset = ui->presetsComboBox->itemData(i).toString();

    int firstSemicolonIndex = _preset.indexOf(';');
    QString preset = _preset.mid(firstSemicolonIndex + 1);

    //qDebug() << "preset " << preset;
    PresetObject obj(name.toLower(), preset);
    qDebug() << "name: "    << obj.getRegionName();
    qDebug() << "name: "    << obj.getRect();
    qDebug() << "name: "    << obj.getPresetIntType();

    QRectF newRect = obj.getRect();
    CaptureRenderer *render = new CaptureRenderer();
    capturePixmap = render->drawRenderedImage(newRect);
    captureContainer->setCapturePixmap(&capturePixmap);
    captureContainer->setRect(newRect);
    captureContainer->setPreviewName(&name);
    captureContainer->updateCapture();
    //
    currentType = static_cast<PresetType>(obj.getPresetIntType());
    currentRect = obj.getRect();
}


void MainWindow::on_toggleVisibility_toggled(bool checked)
{
    isHide = !checked;
}


void MainWindow::on_toggleOnTop_toggled(bool checked)
{
    // hide() show() is required because of how Qt handles window flags
    // https://forum.qt.io/topic/60642/framelesswindowhint-fails-at-runtime-on-mainwindow/13
    hide();
    setWindowFlag(Qt::WindowStaysOnTopHint, checked);
    show();
}

