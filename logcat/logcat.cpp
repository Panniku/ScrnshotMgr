#include "logcat.h"

void Logcat::setLogcat(QTextEdit *textEdit)
{
    logcat = textEdit;
}

QTextEdit *Logcat::getLogcat()
{
    return logcat;
}

void Logcat::logInfo(QString text)
{
    logcat->append(text);
}
