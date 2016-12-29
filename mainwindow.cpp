#include "mainwindow.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QInputDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QGraphicsView* graphicsView = new QGraphicsView(this);
    energyBar = new QProgressBar(this);
    energyBar->setFormat("Energy: %v/%m");
    energyBar->setValue(0);

    healthBar = new QProgressBar(this);
    healthBar->setFormat("Health: %v/%m");
    healthBar->setValue(0);
    this->statusBar()->insertPermanentWidget(0, energyBar);
    this->statusBar()->insertPermanentWidget(1, healthBar);
    this->setCentralWidget(graphicsView);
    QGraphicsScene* scene = new QGraphicsScene(graphicsView);
    controller = std::make_shared<WorldController>(scene);
    graphicsView->setScene(scene);
    QToolBar* toolBar = new QToolBar(this);
    this->addToolBar(toolBar);

    QAction *openAction = new QAction("Open map...", this);
    toolBar->addAction(openAction);

    QFileDialog *mapSelectDialog = new QFileDialog(this);
    mapSelectDialog->setMimeTypeFilters(QStringList(QString("image/png")));
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
        emit actionRunning(true);
    });
    QObject::connect(this, &MainWindow::worldLoaded, runAction, &QAction::setEnabled);

    QAction *pathfindAction = new QAction("Run pathfinder...", this);
    toolBar->addAction(pathfindAction);

    QObject::connect(this, &MainWindow::worldLoaded, pathfindAction, &QAction::setEnabled);
    CoordinateInputDialog *dialog = new CoordinateInputDialog(0, 0, this);
    QObject::connect(dialog, &CoordinateInputDialog::accepted, [dialog, this]() {
        this->controller->doPathfinderSteps(dialog->getXPos(), dialog->getYPos(), dialog->getAnimationSpeed());
        emit actionRunning(true);
    });
    QObject::connect(pathfindAction, &QAction::triggered, [dialog, this]() {
        dialog->setMaxY(this->controller->getWorldModel()->tiles()->rows()-1);
        dialog->setMaxX(this->controller->getWorldModel()->tiles()->cols()-1);
        dialog->exec();
    });

    QAction *stopAction = new QAction("Stop", this);
    toolBar->addAction(stopAction);

    QObject::connect(this, &MainWindow::worldLoaded, stopAction, &QAction::setEnabled);
    QObject::connect(this, &MainWindow::actionRunning, stopAction, &QAction::setEnabled);

    QObject::connect(stopAction, &QAction::triggered, [this]() {
        this->controller->stopTimer();
        emit this->actionRunning(false);
    });

    QObject::connect(this, &MainWindow::worldLoaded, [this]() {
        emit this->actionRunning(false);
    });
    emit worldLoaded(false);
}

void MainWindow::createWorld(QString file, int enemies, int healthpacks)
{
    controller->createWorld(file, enemies, healthpacks);
    QObject::connect(&*controller->getWorldModel()->protagonist(), &Protagonist::posChanged, [this](int x, int y) {
        this->healthBar->setValue(this->controller->getWorldModel()->protagonist()->getHealth());
        this->energyBar->setValue(this->controller->getWorldModel()->protagonist()->getEnergy());
    });
    controller->render();
    emit worldLoaded(true);
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
