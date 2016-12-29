#include "mainwindow.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QInputDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QStatusBar>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    MainWindowCentralWidget* mainView = new MainWindowCentralWidget(this);
    this->setCentralWidget(mainView);

    // Energy & health bars
    QProgressBar* energyBar = new QProgressBar(this);
    energyBar->setFormat("Energy: %v/%m");
    QObject::connect(mainView, &MainWindowCentralWidget::energyUpdated, energyBar, &QProgressBar::setValue);

    QProgressBar* healthBar = new QProgressBar(this);
    healthBar->setFormat("Health: %v/%m");
    QObject::connect(mainView, &MainWindowCentralWidget::healthUpdated, healthBar, &QProgressBar::setValue);

    this->statusBar()->insertPermanentWidget(0, energyBar);
    this->statusBar()->insertPermanentWidget(1, healthBar);
    QToolBar* toolBar = new QToolBar(this);
    this->addToolBar(toolBar);

    // Map open action
    QAction *openAction = new QAction("Open map...", this);
    toolBar->addAction(openAction);

    QFileDialog *mapSelectDialog = new QFileDialog(this);
    mapSelectDialog->setMimeTypeFilters(QStringList(QString("image/png")));
    MapConfigInputDialog *mapConfigDialog = new MapConfigInputDialog(this);
    QObject::connect(openAction, &QAction::triggered, mapSelectDialog, &QFileDialog::exec);
    QObject::connect(mapSelectDialog, &QFileDialog::accepted, mapConfigDialog, &MapConfigInputDialog::exec);
    QObject::connect(mapConfigDialog, &MapConfigInputDialog::accepted, [mapSelectDialog, mapConfigDialog, mainView]() {
        mainView->createWorld(mapSelectDialog->selectedFiles().value(0), mapConfigDialog->getEnemies(), mapConfigDialog->getHealthpacks());
    });

    // Run strategy action
    QAction *runAction = new QAction("Run strategy", this);
    toolBar->addAction(runAction);
    QObject::connect(runAction, &QAction::triggered, [mainView, this]() {
        mainView->runStrategy();
        emit this->actionRunning(true);
    });
    QObject::connect(mainView, &MainWindowCentralWidget::worldLoaded,[runAction](int, int) {
        runAction->setEnabled(true);
    });

    // Pathfinder action
    QAction *pathfindAction = new QAction("Run pathfinder...", this);
    toolBar->addAction(pathfindAction);
    QObject::connect(mainView, &MainWindowCentralWidget::worldLoaded, [pathfindAction](int, int) {
        pathfindAction->setEnabled(true);
    });
    CoordinateInputDialog *dialog = new CoordinateInputDialog(0, 0, this);
    QObject::connect(dialog, &CoordinateInputDialog::accepted, [dialog, mainView, this]() {
        mainView->runPathfinder(dialog->getXPos(), dialog->getYPos());
        emit actionRunning(true);
    });
    QObject::connect(mainView, &MainWindowCentralWidget::worldLoaded, dialog, &CoordinateInputDialog::setMaxDims);
    QObject::connect(pathfindAction, &QAction::triggered, dialog, &QDialog::exec);

    // Stop action
    QAction *stopAction = new QAction("Stop", this);
    toolBar->addAction(stopAction);
    QObject::connect(this, &MainWindow::actionRunning, stopAction, &QAction::setEnabled);
    QObject::connect(stopAction, &QAction::triggered, [mainView, this]() {
        mainView->stopAction();
        emit this->actionRunning(false);
    });

    QObject::connect(mainView, &MainWindowCentralWidget::worldLoaded, [this](int, int) {
        emit this->actionRunning(false);
    });
}

void MainWindowCentralWidget::createWorld(QString file, int enemies, int healthpacks)
{
    QGraphicsScene* newScene = new QGraphicsScene(this);
    QGraphicsScene* oldScene = graphicsView->scene();
    graphicsView->setScene(newScene);
    delete oldScene;


    controller->createWorld(file, enemies, healthpacks);
    QObject::connect(&*controller->getWorldModel()->protagonist(), &Protagonist::posChanged, [this](int, int) {
        emit this->healthUpdated(this->controller->getWorldModel()->protagonist()->getHealth());
        emit this->energyUpdated(this->controller->getWorldModel()->protagonist()->getEnergy());
    });
    controller->render();
    emit worldLoaded(controller->getWorldModel()->tiles()->rows(), controller->getWorldModel()->tiles()->cols());
    emit this->healthUpdated(this->controller->getWorldModel()->protagonist()->getHealth());
    emit this->energyUpdated(this->controller->getWorldModel()->protagonist()->getEnergy());
}

void MainWindowCentralWidget::keyPressEvent(QKeyEvent *event)
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
    case Qt::Key_F:
        controller->debugMode=!controller->debugMode;
        break;
    }
}

void MainWindowCentralWidget::runStrategy()
{
    this->controller->playStrategy(heuristicsWeight);
}

void MainWindowCentralWidget::runPathfinder(int targetX, int targetY)
{
    this->controller->doPathfinderSteps(targetX, targetY, heuristicsWeight);
}

void MainWindowCentralWidget::stopAction()
{
    this->controller->stopTimer();
}

MainWindowCentralWidget::MainWindowCentralWidget(QWidget *parent)
    :QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    // Scene
    graphicsView = new QGraphicsView(this);
    layout->addWidget(graphicsView);
    QGraphicsScene *scene = new QGraphicsScene(graphicsView);
    graphicsView->setScene(scene);

    // Controller
    controller = std::make_shared<WorldController>(graphicsView);

    // Controls panel
    QHBoxLayout *controlsLayout = new QHBoxLayout;
    layout->addLayout(controlsLayout);

    // Animation speed
    QSlider *animationSpeed = new QSlider(Qt::Horizontal, this);
    animationSpeed->setMinimum(1);
    animationSpeed->setMaximum(100);
    controlsLayout->addWidget(animationSpeed);

    QObject::connect(animationSpeed, &QSlider::valueChanged, [this](int value) {
        this->controller->updateAnimationSpeed(value);
    });

    QSlider *sceneScale = new QSlider(Qt::Horizontal, this);
    sceneScale->setMinimum(1);
    sceneScale->setMaximum(1000);
    controlsLayout->addWidget(sceneScale);
    QObject::connect(sceneScale, &QSlider::valueChanged, [this](int value) {
        this->graphicsView->setTransform(QTransform::fromScale(value/100.f, value/100.f));
    });

    // Heuristic weight
    QSlider *heuristicsWeight = new QSlider(Qt::Horizontal, this);
    controlsLayout->addWidget(heuristicsWeight);
    heuristicsWeight->setMinimum(0);
    heuristicsWeight->setMaximum(100);

    QObject::connect(heuristicsWeight, &QSlider::valueChanged, [this](int value) {
        this->heuristicsWeight = value/10.f;
    });
    // Disable adjusting heuristics weight when an action is running (it has no effect anyways)
    QObject::connect(&controller->getTimer(), &ActionTimer::activated, heuristicsWeight, &QSlider::setDisabled);


    // Controls startup values
    animationSpeed->setValue(5);
    sceneScale->setValue(1);
    heuristicsWeight->setValue(10);
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
    formLayout->addRow(new QLabel("x position:"), xPos);
    formLayout->addRow(new QLabel("y position:"), yPos);
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
