#ifndef PRESETEDITOR_H
#define PRESETEDITOR_H

#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>

namespace Ui {
class PresetEditor;
}

class PresetEditor : public QDialog
{
    Q_OBJECT

public:
    explicit PresetEditor(QWidget *parent = nullptr);
    ~PresetEditor();


private:
    Ui::PresetEditor *ui;


    // Editor View
    QLineEdit *nameEdit;
    QComboBox *typeEdit;
    QSpinBox *xEdit, *yEdit, *widthEdit, *heightEdit;
};

#endif // PRESETEDITOR_H
