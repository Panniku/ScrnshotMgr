#ifndef LOGCAT_H
#define LOGCAT_H

#include <QObject>
#include <QString>
#include <QTextEdit>
#include <QMainWindow>

enum LogType {
    Info,
    Debug,
    Success,
    Warning,
    Error,
    Critical
};

class Logcat : public QObject
{

    Q_OBJECT

    private:
        inline static QTextEdit *logcat = nullptr;

    public:
        // set logcat
        void setLogcat(QTextEdit *textEdit);
        // get logger
        static QTextEdit *getLogcat();
        static void log(LogType type, QString tag, QString m);

};

#endif // LOGCAT_H
