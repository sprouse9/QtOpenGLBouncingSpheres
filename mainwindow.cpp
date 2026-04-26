#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QSpinBox>
#include <QLabel>
#include "ballviewport.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto *central = new QWidget(this);
    setCentralWidget(central);

    auto *mainLayout = new QHBoxLayout(central);

    auto *controlsLayout = new QVBoxLayout();

    auto *radiusLabel = new QLabel("Ball radius:");
    auto *radiusSpin = new QSpinBox();
    radiusSpin->setRange(1, 100);
    radiusSpin->setValue(10);

    controlsLayout->addWidget(radiusLabel);
    controlsLayout->addWidget(radiusSpin);
    controlsLayout->addStretch();

    auto *viewport = new BallViewport();

    mainLayout->addLayout(controlsLayout);
    mainLayout->addWidget(viewport, 1);
}

MainWindow::~MainWindow()
{
    delete ui;
}
