#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>


// NOTE:
// This class serves to simplify the usage of QSettings in the project.
// Initializing QSettings is confusing and reusing it everywhere is a mess.

class ConfigManager: public QObject
{
    Q_OBJECT
public:
    void initDefault();
    static QVariant getValue(QString key, QVariant fallback);
    static void setValue(QString key, QVariant value);
};

#endif // CONFIGMANAGER_H
