#include "preseteditor.h"
#include "qscreen.h"
#include "ui_preseteditor.h"
#include <QLabel>
#include <QMediaCaptureSession>
#include <QScreenCapture>
#include "../../capture/capturecontainer.h"

PresetEditor::PresetEditor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PresetEditor)
{
    ui->setupUi(this);

    setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, true);

    // QFrame *editorFrameRoot = new QFrame();
    QVBoxLayout *rootLayout = new QVBoxLayout();
    ui->editorFrame->setLayout(rootLayout);
    //centralViewLayout->addWidget(editorFrameRoot);
    // ui->presetSplitter->addWidget(editorFrameRoot);

    QLabel *nameLabel = new QLabel("Name");
    QLabel *typeLabel = new QLabel("Type");
    nameEdit = new QLineEdit();
    typeEdit = new QComboBox();

    QHBoxLayout *e1 = new QHBoxLayout();
    e1->setContentsMargins(QMargins(0, 0, 0, 0));
    e1->addWidget(nameLabel);
    e1->addWidget(nameEdit);

    QHBoxLayout *e2 = new QHBoxLayout();
    e2->setContentsMargins(QMargins(0, 0, 0, 0));
    e2->addWidget(typeLabel);
    e2->addWidget(typeEdit);

    QVBoxLayout *edit1 = new QVBoxLayout();
    edit1->setContentsMargins(QMargins(0, 0, 0, 0));
    edit1->addLayout(e1);
    edit1->addLayout(e2);

    rootLayout->addLayout(edit1);

    QLabel *xLabel = new QLabel("X Pos");
    QLabel *yLabel = new QLabel("Y Pos");
    QLabel *widthLabel = new QLabel("Width");
    QLabel *heightLabel = new QLabel("Height");

    xEdit = new QSpinBox();
    yEdit = new QSpinBox();
    widthEdit = new QSpinBox();
    heightEdit = new QSpinBox();

    QHBoxLayout *e3 = new QHBoxLayout();
    e3->setContentsMargins(QMargins(0, 0, 0, 0));
    e3->addWidget(xLabel);
    e3->addWidget(xEdit);
    QHBoxLayout *e4 = new QHBoxLayout();
    e4->setContentsMargins(QMargins(0, 0, 0, 0));
    e4->addWidget(yLabel);
    e4->addWidget(yEdit);
    QHBoxLayout *e5 = new QHBoxLayout();
    e5->setContentsMargins(QMargins(0, 0, 0, 0));
    e5->addWidget(widthLabel);
    e5->addWidget(widthEdit);
    QHBoxLayout *e6 = new QHBoxLayout();
    e6->setContentsMargins(QMargins(0, 0, 0, 0));
    e6->addWidget(heightLabel);
    e6->addWidget(heightEdit);

    QVBoxLayout *edit3 = new QVBoxLayout();
    edit3->setContentsMargins(QMargins(0, 0, 0, 0));
    edit3->addLayout(e3);
    edit3->addLayout(e4);
    edit3->addLayout(e5);
    edit3->addLayout(e6);

    // QLabel *creature = new QLabel("blobcatcozy");

    QScreen *screen = QApplication::primaryScreen();
    int displayWidth = screen->size().width();
    int displayHeight = screen->size().height();

    CaptureContainer *presetPreview = new CaptureContainer();
    presetPreview->setDimens(displayWidth, displayHeight);
    presetPreview->setRect(QRectF(64, 64, 128, 128));

    QGroupBox *previewBox = new QGroupBox(presetPreview);
    previewBox->setStyleSheet("border: 2px solid red;");

    presetPreview->setPreviewBox(previewBox);

    QHBoxLayout *what = new QHBoxLayout();
    what->setContentsMargins(QMargins(0, 0, 0, 0));
    what->addLayout(edit3);
    what->addWidget(presetPreview);

    rootLayout->addLayout(what);

    setFixedSize(400, 200);
    presetPreview->updateCapture();
}

PresetEditor::~PresetEditor()
{
    delete ui;
}
