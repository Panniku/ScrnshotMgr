#include "logtextedit.h"

#include <QApplication>
#include <QClipboard>
#include <QMenu>

#include "../logcat/logcat.h"

LogTextEdit::LogTextEdit() {}

void LogTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    QAction *copyClipboard = new QAction("Copy contents");
    menu.addAction(copyClipboard);
    connect(copyClipboard, &QAction::triggered, this, &LogTextEdit::copyToClipboard);
    menu.exec(event->globalPos());
}

void LogTextEdit::copyToClipboard()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(this->toPlainText());
    Logcat::log(LogType::Debug, "LogTextEdit", "Copied to clipboard");
}
