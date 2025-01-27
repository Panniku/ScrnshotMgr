#include "logcat.h"

#include "../configmanager/configmanager.h"

void Logcat::setLogcat(QTextEdit *textEdit)
{
    logcat = textEdit;
}

QTextEdit *Logcat::getLogcat()
{
    return logcat;
}

void Logcat::log(LogType type, QString tag, QString msg)
{
    if (logcat != nullptr) {
        QTextCursor cursor = logcat->textCursor();
        QTextCharFormat format;
        // format.setForeground(QBrush(QColor::fromString(ConfigManager::getValue("Colors/LogInfo", "#000000").toString())));
        switch(type){
        case Info:
            format.setForeground(QBrush(QColor::fromString(ConfigManager::getValue("Colors/LogInfo").toString())));
            break;
        case Debug:
            format.setForeground(QBrush(QColor::fromString(ConfigManager::getValue("Colors/LogDebug").toString())));
            break;
        case Success:
            format.setForeground(QBrush(QColor::fromString(ConfigManager::getValue("Colors/LogSuccess").toString())));
            break;
        case Warning:
            format.setForeground(QBrush(QColor::fromString(ConfigManager::getValue("Colors/LogWarning").toString())));
            break;
        case Error:
            format.setForeground(QBrush(QColor::fromString(ConfigManager::getValue("Colors/LogError").toString())));
            break;
        case Critical:
            format.setForeground(QBrush(QColor::fromString(ConfigManager::getValue("Colors/LogCritical").toString())));
            break;
        }
        // qDebug() << ConfigManager::getValue("Colors/LogInfo", "#000000").toString();

        format.setFontWeight(QFont::Bold);
        cursor.setCharFormat(format);
        cursor.insertText("[" + tag + "] ");

        format.setFontWeight(QFont::Normal);
        format.setForeground(QBrush(QColor::fromString(ConfigManager::getValue("Colors/LogDefault").toString())));
        cursor.setCharFormat(format);
        cursor.insertText(msg + "\n");
    }
}
