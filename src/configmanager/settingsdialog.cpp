#include "settingsdialog.h"
#include "configmanager.h"
#include "qtabbar.h"
#include "ui_settingsdialog.h"
#include <QLabel>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    QTabWidget *tabWidget = ui->tabWidget;
    QTabBar *tabs = tabWidget->tabBar();

    QLabel *generalLabel = new QLabel("General");
    QLabel *colorLabel = new QLabel("Colors");
    QLabel *aboutLabel = new QLabel("About");

    tabs->setTabButton(0, QTabBar::LeftSide, generalLabel);
    tabs->setTabButton(1, QTabBar::LeftSide, colorLabel);
    tabs->setTabButton(2, QTabBar::LeftSide, aboutLabel);

    // This option is just there for windows because microsoft is too lazy to update their msstyles to have light/dark
    // However, on UNIX the themes match system, so no need for this
    ui->themeFrame->setDisabled(true);
    #if defined(Q_OS_WIN32)
        ui->themeFrame->setDisabled(false);
    #endif

    ConfigManager config;
    QSettings *s = config.getSettings();

    QString theme = s->value("Application/Theme").toString();
    if (theme == "system") {
        ui->themeSelector->setCurrentIndex(0);
    } else if (theme == "light") {
        ui->themeSelector->setCurrentIndex(1);
    } else if (theme == "dark") {
        ui->themeSelector->setCurrentIndex(2);
    }

    ui->snapDir->setText(s->value("Application/SnapsDir").toString());
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_themeSelector_currentIndexChanged(int index)
{
    ConfigManager config;
    QSettings *s = config.getSettings();

    switch(index){
    case 0:
        qDebug() << "system";
        s->setValue("Application/Theme", "system");
        break;
    case 1:
        qDebug() << "light";
        s->setValue("Application/Theme", "light");
        break;
    case 2:
        qDebug() << "dark";
        s->setValue("Application/Theme", "dark");
        break;
    }
}

