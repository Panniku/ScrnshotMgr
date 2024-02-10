#include <QDebug>
#include <snapper/snapper.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logcat/logcat.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Logcat::setLogcat(ui->textEdit);
    Logcat::logInfo("Hello World");
    Logcat::logInfo("Running with qt6.6.0");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    Snapper snapper;
    snapper.screenshot();
    // Logcat logger;
}

