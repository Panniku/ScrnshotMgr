#include "configmanager.h"

#include <QCoreApplication>
#include <QDir>
#include <QLibraryInfo>
#include <QSettings>
#include <QStandardPaths>

#include "../logcat/logcat.h"

void ConfigManager::initDefault()
{

    // qDebug() << settings.value("Colors/LogCritical", "#000000");

    // Logcat::log(LogType::Debug, "Init", "Started process.");
    // Logcat::log(LogType::Info, "Init", "Qt version: " + QLibraryInfo::version().toString());
    // Logcat::log(LogType::Info, ":3", "Read if cute");

    QCoreApplication::setApplicationName("config");
    QCoreApplication::setOrganizationName("ScrnMgr");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    QSettings settings;
    Logcat::log(LogType::Debug, "ConfigManager", "Reading config...");

    // Default settings initialization
    QString snapsDir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/ScrnMgr";
    QDir dir(snapsDir);
    if(!dir.exists())
    {
        dir.mkpath(".");
        Logcat::log(LogType::Debug, "ConfigManager", "Created snapsDir directory.");
    }



    // SnapsDir
    qDebug() << settings.status();
    qDebug() << settings.fileName();
    qDebug() << settings.contains("Application/SnapsDir");
    qDebug() << settings.value("Application/SnapsDir", "??");
    if(settings.contains("Application/SnapsDir") == false) {
        settings.setValue("Application/SnapsDir", snapsDir);
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

    for (auto i = logColors.constBegin(); i != logColors.constEnd(); i++)
    {
        if(settings.contains(i.key()) == false)
        {
            settings.setValue(i.key(), i.value());
            Logcat::log(LogType::Debug, "ConfigManager", "Creating color " + i.key());
        }
    }
}

QVariant ConfigManager::getValue(QString key, QVariant fallback)
{
    QSettings settings;
    return settings.value(key, fallback);
}

void ConfigManager::setValue(QString key, QVariant value)
{
    QSettings settings;
    settings.setValue(key, value);
}
