#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QSettings>


// NOTE:
// This class serves to simplify the usage of QSettings in the project.
// Initializing QSettings is confusing and reusing it everywhere is a mess.

class ConfigManager: public QObject
{
    Q_OBJECT
private:
    static inline QSettings *settings = nullptr;

public:
    void initDefault();
    static QVariant getValue(QAnyStringView key);
    static void setValue(QString key, QVariant value);

    QSettings *getSettings();
};

#endif // CONFIGMANAGER_H
