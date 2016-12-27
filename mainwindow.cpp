#include "mainwindow.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QInputDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), mainView(this)
{
    energyBar = new QProgressBar(&mainView);
    energyBar->setGeometry(QRect(100,10,120,25));

    energyBar->setValue(0);

    healthBar = new QProgressBar(&mainView);
    healthBar->setGeometry(QRect(230,10,120,25));
    healthBar->setValue(0);
    this->setCentralWidget(&mainView);
    mainView.show();
    QGraphicsScene* scene = new QGraphicsScene(&mainView);
    controller = new WorldController(scene);
    mainView.setScene(scene);
    toolBar = new QToolBar(this);
    this->addToolBar(toolBar);

    QAction *openAction = new QAction("Open map...", this);
    toolBar->addAction(openAction);

    QFileDialog *mapSelectDialog = new QFileDialog(this);
    MapConfigInputDialog *mapConfigDialog = new MapConfigInputDialog(this);
    QObject::connect(openAction, &QAction::triggered, mapSelectDialog, &QFileDialog::exec);
    QObject::connect(mapSelectDialog, &QFileDialog::accepted, mapConfigDialog, &MapConfigInputDialog::exec);
    QObject::connect(mapConfigDialog, &MapConfigInputDialog::accepted, [mapSelectDialog, mapConfigDialog, this]() {
        this->createWorld(mapSelectDialog->selectedFiles().value(0), mapConfigDialog->getEnemies(), mapConfigDialog->getHealthpacks());

    });

    QAction *runAction = new QAction("Run strategy", this);
    toolBar->addAction(runAction);
    QObject::connect(runAction, &QAction::triggered, [this]() {
        this->controller->playStrategy();
    });

    QAction *pathfindAction = new QAction("Run pathfinder...", this);
    toolBar->addAction(pathfindAction);

    CoordinateInputDialog *dialog = new CoordinateInputDialog(5, 5, this);
    QObject::connect(dialog, &CoordinateInputDialog::accepted, [dialog, this]() {
        this->controller->doPathfinderSteps(dialog->getXPos(), dialog->getYPos(), dialog->getAnimationSpeed());
    });
    QObject::connect(pathfindAction, &QAction::triggered, dialog, &CoordinateInputDialog::exec);

    QAction *stopAction = new QAction("Stop", this);
    toolBar->addAction(stopAction);

    QObject::connect(stopAction, &QAction::triggered, [this]() {
        this->controller->stopTimer();
    });
}

void MainWindow::createWorld(QString file, int enemies, int healthpacks)
{
    controller->createWorld(file, enemies, healthpacks);
    controller->render();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    default:
        return;
        break;
    case Qt::Key_Left:
    case Qt::Key_4:
        controller->moveProtagonist(-1, 0);
        break;
    case Qt::Key_Right:
    case Qt::Key_6:
        controller->moveProtagonist(1, 0);
        break;
    case Qt::Key_Up:
    case Qt::Key_8:
        controller->moveProtagonist(0, -1);
        break;
    case Qt::Key_Down:
    case Qt::Key_2:
        controller->moveProtagonist(0, 1);
        break;
    case Qt::Key_Plus:
        controller->updateScale(1.f/2);
        break;
    case Qt::Key_Minus:
        controller->updateScale(2);
        break;
    case Qt::Key_Enter:
        controller->doPathfinderStep();
        break;
    case Qt::Key_A:
        for(int i=0; i<40; i++)
            if(controller->doPathfinderStep())
                break;
        break;
    case Qt::Key_B:
        for(int i=0; i<400; i++)
            if(controller->doPathfinderStep())
                break;
        break;
    case Qt::Key_C:
        for(int i=0; i<4000; i++)
            if(controller->doPathfinderStep())
                break;
        break;
    case Qt::Key_D:
        for(int i=0; i<40000; i++)
            if(controller->doPathfinderStep())
                break;
        break;
    case Qt::Key_E:
        controller->doPathfinder();
        break;
    case Qt::Key_F:
        controller->debugMode=!controller->debugMode;
        break;
    case Qt::Key_P:
        controller->playStrategy();
    }
    energyBar->setValue(controller->getProtagonistEnergy());
    healthBar->setValue(controller->getProtagonistHealth());

    controller->render();
}

MainWindow::~MainWindow()
{
    delete controller;
}

CoordinateInputDialog::CoordinateInputDialog(int maxX, int maxY, QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QFormLayout* formLayout = new QFormLayout;
    xPos = new QSpinBox(this);
    xPos->setMinimum(0);
    xPos->setMaximum(maxX);
    yPos = new QSpinBox(this);
    yPos->setMinimum(0);
    yPos->setMaximum(maxY);
    animationSpeed = new QSpinBox(this);
    animationSpeed->setMinimum(0);
    animationSpeed->setMaximum(1000);
    formLayout->addRow(new QLabel("x position:"), xPos);
    formLayout->addRow(new QLabel("y position:"), yPos);
    formLayout->addRow(new QLabel("Animation speed:"), animationSpeed);
    mainLayout->addLayout(formLayout);

    QDialogButtonBox* dialogButtons = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, this);
    mainLayout->addWidget(dialogButtons);

    connect(dialogButtons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(dialogButtons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

MapConfigInputDialog::MapConfigInputDialog(QWidget *parent)
    :QDialog(parent)
{
    setModal(true);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QFormLayout* formLayout = new QFormLayout;
    enemies = new QSpinBox(this);
    enemies->setMinimum(1);
    healthpacks = new QSpinBox(this);
    healthpacks->setMinimum(0);
    formLayout->addRow(new QLabel("number of enemies:"), enemies);
    formLayout->addRow(new QLabel("number of healthpacks:"), healthpacks);
    mainLayout->addLayout(formLayout);

    QDialogButtonBox* dialogButtons = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, this);
    mainLayout->addWidget(dialogButtons);

    connect(dialogButtons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(dialogButtons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
