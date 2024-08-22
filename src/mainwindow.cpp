#include <QDebug>
#include <QPainter>
#include <QScrollArea>
#include <QStandardPaths>
#include <QTimer>
#include <QPair>
#include <QLibraryInfo>
#include <QScreenCapture>
#include <QMediaCaptureSession>
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
#include "utils/utils.h"

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
    if (theme == "light") {

    } else if (theme == "dark") {
        qApp->setStyle("fusion");
        //
        QPalette p = palette();
        // p.setBrush(QPalette::Window, p.brush(QPalette::Base));
        qApp->setPalette(p);
    }


    QScreen *screen = QApplication::primaryScreen();
    displayWidth = screen->size().width();
    displayHeight = screen->size().height();


    // Editor initialization
    QFrame *editorFrameRoot = new QFrame();
    QVBoxLayout *rootLayout = new QVBoxLayout();
    editorFrameRoot->setLayout(rootLayout);
    //centralViewLayout->addWidget(editorFrameRoot);
    // ui->presetSplitter->addWidget(editorFrameRoot);

    QLabel *nameLabel = new QLabel("Name");
    QLabel *typeLabel = new QLabel("Type");
    nameEdit = new QLineEdit();
    typeEdit = new QComboBox();

    QHBoxLayout *e1 = new QHBoxLayout();
    e1->setContentsMargins(QMargins(0, 0, 0, 0));
    e1->addWidget(nameLabel);
    e1->addWidget(nameEdit);

    QHBoxLayout *e2 = new QHBoxLayout();
    e2->setContentsMargins(QMargins(0, 0, 0, 0));
    e2->addWidget(typeLabel);
    e2->addWidget(typeEdit);

    QVBoxLayout *edit1 = new QVBoxLayout();
    edit1->setContentsMargins(QMargins(0, 0, 0, 0));
    edit1->addLayout(e1);
    edit1->addLayout(e2);

    rootLayout->addLayout(edit1);

    QLabel *xLabel = new QLabel("X Pos");
    QLabel *yLabel = new QLabel("Y Pos");
    QLabel *widthLabel = new QLabel("Width");
    QLabel *heightLabel = new QLabel("Height");

    xEdit = new QSpinBox();
    yEdit = new QSpinBox();
    widthEdit = new QSpinBox();
    heightEdit = new QSpinBox();

    QHBoxLayout *e3 = new QHBoxLayout();
    e3->setContentsMargins(QMargins(0, 0, 0, 0));
    e3->addWidget(xLabel);
    e3->addWidget(xEdit);
    QHBoxLayout *e4 = new QHBoxLayout();
    e4->setContentsMargins(QMargins(0, 0, 0, 0));
    e4->addWidget(yLabel);
    e4->addWidget(yEdit);
    QHBoxLayout *e5 = new QHBoxLayout();
    e5->setContentsMargins(QMargins(0, 0, 0, 0));
    e5->addWidget(widthLabel);
    e5->addWidget(widthEdit);
    QHBoxLayout *e6 = new QHBoxLayout();
    e6->setContentsMargins(QMargins(0, 0, 0, 0));
    e6->addWidget(heightLabel);
    e6->addWidget(heightEdit);

    QVBoxLayout *edit3 = new QVBoxLayout();
    edit3->setContentsMargins(QMargins(0, 0, 0, 0));
    edit3->addLayout(e3);
    edit3->addLayout(e4);
    edit3->addLayout(e5);
    edit3->addLayout(e6);

    QLabel *creature = new QLabel("blobcatcozy");

    QHBoxLayout *what = new QHBoxLayout();
    what->setContentsMargins(QMargins(0, 0, 0, 0));
    what->addLayout(edit3);
    what->addWidget(creature);

    rootLayout->addLayout(what);



    // Capture preview initialization
    captureContainer = new CaptureContainer();
    captureContainer->setDimens(displayWidth, displayHeight);
    captureContainer->setStyleSheet("border: 1px solid gray;");
    ui->mainSplitter->addWidget(captureContainer);
    // centralViewSplitter->addWidget(captureContainer);
    // centralViewSplitter->


    // THIS IS TEMPORARY
    // THIS IS TEMPORARY
    // THIS IS TEMPORARY

    capturesSplitter = new QSplitter();
    ui->mainSplitter->addWidget(capturesSplitter);
    capturesSplitter->setOrientation(Qt::Vertical);

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
    painter.fillRect(QRectF(0,0,screen->size().width(), screen->size().height()), p.brush(QPalette::Midlight));
    QPixmap lsp = QPixmap::fromImage(image);
    lastPreview->setPixmap(lsp);
    lastCapturePreview->setCapture(lastPreview);
    lastCapturePreview->setCapturePixmap(lsp);

    lastTemp = new QLabel(lastCapturePreview);
    lastTemp->setText("This area renders the last taken screenshot.\nNo screenshots taken yet.");
    lastCapturePreview->setTempLabel(lastTemp);

    captureList = new QListWidget(capturesSplitter);
    capturesSplitter->addWidget(captureList);

    // capture code lol
    captureDisplayLabel = new QLabel(captureContainer);
    captureDisplayLabel->setText("Display 1 / " + QString::number(displayWidth) + "x" + QString::number(displayHeight));
    captureDisplayLabel->setAlignment(Qt::AlignBottom | Qt::AlignRight);

    capturePreviewBox = new QGroupBox(captureContainer);
    capturePreviewBox->setStyleSheet("border: 2px solid red;");


    // TEMPORARY
    // TEMPORARY
    // TEMPORARY
    // TEMPORARY
    // QScreenCapture *screenCapture = new QScreenCapture();
    // screenCapture->setScreen(qApp->primaryScreen());
    // screenCapture->start();

    // QMediaCaptureSession *mediaSesh = new QMediaCaptureSession();
    // mediaSesh->setScreenCapture(screenCapture);
    // mediaSesh->setVideoOutput(displayRender);

    // captureContainer->setDisplayRender(displayRender);
    captureContainer->setDisplayLabel(captureDisplayLabel);
    // captureContainer->setDisplayText()
    captureContainer->setPreviewBox(capturePreviewBox);
    // captureContainer->setCapturePixmap(&capturePixmap);
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
    ui->presetsTabRoot->setGeometry(ui->presetsTabRoot->x(), ui->presetsTabRoot->y(), 150, ui->presetsTabRoot->height());

    // ui->splitter->handle(0)->setStyleSheet("background-color: green");

    // some test
    // qDebug()  << ":/themes/dark.qss";
    // Currently, this only applies a border to the splitter handle lol
    // QFile file(":/resources_root/themes/dark.qss");
    // file.open(QFile::ReadOnly);
    // QString styleSheet = QLatin1String(file.readAll());
    // qApp->setStyleSheet(styleSheet);

    setupIcons();

    resize(640, 360);
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
    QPalette p = palette();
    QBrush b = p.brush(QPalette::Midlight);
    // Misc search icon
    QPixmap searchPixmap = QPixmap(":/res/icons/ph--magnifying-glass.svg");
    ui->presetSearchItem->addAction(QIcon(Utils::getMaskedRecoloredIconPixmap(searchPixmap, b)), QLineEdit::LeadingPosition);

    // Add icon
    QPixmap addPixmap = QPixmap(":/res/icons/ph--plus.svg");
    ui->presetAddItem->setIcon(QIcon(Utils::getMaskedRecoloredIconPixmap(addPixmap, b)));

    // Visibility icon
    QPixmap visibilityPixmap = QPixmap(":/res/icons/ph--eye-slash-light.svg");
    QPixmap visibilityPixmapOn = QPixmap(":/res/icons/ph--eye-light.svg");
    QIcon visibilityIcon = ui->toggleVisibility->icon();
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
    ui->toggleVisibility->setIcon(visibilityIcon);

    // Hide icon
    // Visibility icon
    QPixmap topPixmap = QPixmap(":/res/icons/ph--push-pin-slash-light.svg");
    QPixmap topPixmapOn = QPixmap(":/res/icons/ph--push-pin-light.svg");
    QIcon topIcon = ui->toggleOnTop->icon();
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
    ui->toggleOnTop->setIcon(topIcon);
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
    // capturePixmap = render->drawRenderedImage(displayWidth, displayHeight, newRect);
    // captureContainer->setCapturePixmap(&capturePixmap);
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
