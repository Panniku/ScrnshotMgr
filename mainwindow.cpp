#include <QDebug>
#include <QPainter>
#include <QScrollArea>
#include <QStandardPaths>
#include <QTimer>
#include <QPair>
#include <QLibraryInfo>
#include <ui_components/capture/capturerenderer.h>
#include <ui_components/logtextedit.h>
#include <ui_components/presets/presetobject.h>
#include <snapper/snapper.h>
#include <configmanager/configmanager.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logcat/logcat.h"

QPixmap *mCapture;

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

    ConfigManager *configManager = new ConfigManager();
    Logcat *logcat = new Logcat();
    logcat->setLogcat(logTextEdit);


    configManager->initDefault();

    Logcat::log(LogType::Warning, "NOTICE", "If colors are bugged, this may be due to how we initialize colors. Sorry for the inconvenience!");
    logcat->log(LogType::Info, "―――――――――――――――――――――――――――――――――――――――――――――", "");
    // CONFIGMANAGER




    // Capture preview initialization
    captureContainer = new CaptureContainer();
    ui->splitter->addWidget(captureContainer);

    // THIS IS TEMPORARY
    // THIS IS TEMPORARY
    // THIS IS TEMPORARY
    QScrollArea *temparea = new QScrollArea();
    ui->splitter->addWidget(temparea);

    capturePreviewLabel = new QLabel(captureContainer);
    capturePreviewLabel->setPixmap(capturePixmap);
    capturePreviewLabel->setMinimumSize(1, 1);
    capturePreviewLabel->setScaledContents(false);
    capturePreviewLabel->setAlignment(Qt::AlignCenter);
    capturePreviewLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    capturePreviewLabel->setGeometry(0, 0, capturePixmap.width(), capturePixmap.height());

    // captureDisplayLabel = new QLabel(captureContainer);
    // captureDisplayLabel->setText("Display 1");
    // captureDisplayLabel->setAlignment(Qt::AlignCenter);
    // captureDisplayLabel->setGeometry(0, 0, capturePixmap.width(), capturePixmap.height());

    capturePreviewBox = new QGroupBox(captureContainer);
    capturePreviewBox->setTitle("Display 1 PLACEHOLDER");
    // capturePreviewBox->setAlignment(Qt::AlignCenter);
    capturePreviewBox->setStyleSheet("border: 2px solid red;");
    capturePreviewBox->setGeometry(0, 0, capturePixmap.width(), capturePixmap.height());

    captureContainer->setCapture(capturePreviewLabel);
    captureContainer->setPreviewBox(capturePreviewBox);
    captureContainer->setCapturePixmap(&capturePixmap);
    captureContainer->setRect(captureRect);


    // WORK IN PROGRESS
    // WORK IN PROGRESS
    // WORK IN PROGRESS
    // WORK IN PROGRESS


    QScreen *screen = QApplication::primaryScreen();
    // QPixmap screenshot = screen->grabWindow(0, 0, 0, screen->size().width(), screen->size().height());

    PresetObject freedraw("freedraw", 0, "Free draw", PresetType::Freedraw, QRectF(0, 0, 0, 0));
    PresetObject fullscreen("fullscreen", 1, "Fullscreen", PresetType::Fullscreen, QRectF(0, 0, screen->size().width(), screen->size().height()));
    PresetObject layout1("layout1", 2, "Layout 1", PresetType::Custom, QRectF(64, 64, 256, 256));

    ui->presetsComboBox->addItem("Free draw", freedraw.toVariant());
    ui->presetsComboBox->addItem("Fullscreen",fullscreen.toVariant());
    ui->presetsComboBox->addItem("Layout 1", layout1.toVariant());

    // Misc setup
    ui->presetSearchItem->setClearButtonEnabled(true);
    ui->presetSearchItem->addAction(QIcon(":/resources_root/icons/ph--magnifying-glass.svg"), QLineEdit::LeadingPosition);


}

MainWindow::~MainWindow()
{
    delete ui;
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
    Snapper snapper;
    snapper.screenshot();

    QSettings s;
    qDebug() << "why" << s.value("Application/SnapsDir");

    qDebug() << "Settings file path:" << s.fileName();
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

    QRectF newRect = obj.getRect();
    CaptureRenderer *render = new CaptureRenderer();
    capturePixmap = render->drawRenderedImage(newRect);
    captureContainer->setCapturePixmap(&capturePixmap);
    captureContainer->setRect(newRect);
    captureContainer->updateCapture();
}

