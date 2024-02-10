#ifndef LOGCAT_H
#define LOGCAT_H

#include <QObject>
#include <QString>
#include <QTextEdit>
#include <QMainWindow>

class Logcat : public QObject
{

    Q_OBJECT

    private:
        inline static QTextEdit *logcat = nullptr;

    public:
        // set logcat
        static void setLogcat(QTextEdit *textEdit);
        // get logger
        static QTextEdit *getLogcat();
        /*
         *  Main Functions
         */
        static void logInfo(QString text);
        // static void logError(QString text);
        // static void logWarning(QString text);
};

#endif // LOGCAT_H
