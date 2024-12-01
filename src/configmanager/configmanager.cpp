#include "configmanager.h"

#include <QCoreApplication>
#include <QDir>
#include <QLibraryInfo>
#include <QSettings>
#include <QStandardPaths>

#include "../logcat/logcat.h"

void ConfigManager::initDefault()
{

    // Thanks to some documentation and brain thinking, QSettings is simply summed up like this.
    // No fancy Organizations or Domains, this just works.
    settings = new QSettings(
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/config.ini",
        QSettings::IniFormat
        );
    Logcat::log(LogType::Debug, "ConfigManager", "Reading config...");

    qDebug() << settings->fileName();

    // Default settings initialization
    //
    //

    // Theme (Light/Dark)
    if(!settings->contains("Application/Theme")) {
        settings->setValue("Application/Theme", "system");
    }

    // SnapsDir
    QString snapsDir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/ScrnMgr";
    QDir dir(snapsDir);
    if(!dir.exists()) {
        dir.mkpath(".");
        Logcat::log(LogType::Debug, "ConfigManager", "Created snapsDir directory.");
    }
    if(!settings->contains("Application/SnapsDir")) {
        settings->setValue("Application/SnapsDir", snapsDir);
        Logcat::log(LogType::Debug, "ConfigManager", "Created snapsDir path.");
    }

    // Colors (thanks ai .3)
    QMap<QString, QString> logColors;
    logColors.insert("Colors/LogDefault",   "#282A2E");
    logColors.insert("Colors/LogInfo",      "#5E8D87");
    logColors.insert("Colors/LogDebug",     "#5F819D");
    logColors.insert("Colors/LogSuccess",   "#8C9440");
    logColors.insert("Colors/LogWarning",   "#DE935F");
    logColors.insert("Colors/LogError",     "#A54242");
    logColors.insert("Colors/LogCritical",  "#CC6666");

    for (auto i = logColors.constBegin(); i != logColors.constEnd(); i++) {
        if(!settings->contains(i.key())) {
            settings->setValue(i.key(), i.value());
            Logcat::log(LogType::Debug, "ConfigManager", "Creating color " + i.key());
        }
    }
}

QVariant ConfigManager::getValue(QAnyStringView key)
{
    return settings->value(key);
}

void ConfigManager::setValue(QString key, QVariant value)
{
    settings->setValue(key, value);
}

QSettings *ConfigManager::getSettings()
{
    return settings;
}
