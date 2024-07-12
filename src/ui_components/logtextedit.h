#ifndef LOGTEXTEDIT_H
#define LOGTEXTEDIT_H

#include <QContextMenuEvent>
#include <QObject>
#include <QTextEdit>

class LogTextEdit: public QTextEdit
{
    Q_OBJECT
    public:
        LogTextEdit();
        void contextMenuEvent(QContextMenuEvent *event);

    private slots:
        void copyToClipboard();
};

#endif // LOGTEXTEDIT_H
