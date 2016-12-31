#include "mainwindow.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QInputDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QStatusBar>
#include <QDoubleSpinBox>
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
    emit healthUpdated(controller->getWorldModel()->protagonist()->getHealth());
    emit energyUpdated(controller->getWorldModel()->protagonist()->getEnergy());
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
    graphicsView = new GraphicsView(this);
    layout->addWidget(graphicsView);
    QGraphicsScene *scene = new QGraphicsScene(graphicsView);
    graphicsView->setScene(scene);

    // Controller
    controller = std::make_shared<WorldController>(graphicsView);

    // Run the pathfinder when a tile is pressed
    QObject::connect(graphicsView, &GraphicsView::tilePressed, this, &MainWindowCentralWidget::runPathfinder);

    // Controls panel
    QFormLayout *controlsLayout = new QFormLayout;
    layout->addLayout(controlsLayout);

    // Animation speed
    QSlider *animationSpeed = new QSlider(Qt::Horizontal, this);
    animationSpeed->setMinimum(1);
    animationSpeed->setMaximum(100);
    controlsLayout->addRow(new QLabel("Animation speed"), animationSpeed);

    QObject::connect(animationSpeed, &QSlider::valueChanged, [this](int value) {
        this->controller->updateAnimationSpeed(value);
    });

    // Scene scale
    QSlider *sceneScale = new QSlider(Qt::Horizontal, this);
    sceneScale->setMinimum(0);
    sceneScale->setMaximum(1000);
    controlsLayout->addRow(new QLabel("Scale"), sceneScale);
    QObject::connect(sceneScale, &QSlider::valueChanged, [this](int value) {
        this->graphicsView->setTransform(QTransform::fromScale(std::pow(2, value/100.f)/10.f, std::pow(2, value/100.f)/10.f));
    });

    // Heuristic weight
    QDoubleSpinBox *heuristicsWeight = new QDoubleSpinBox(this);
    heuristicsWeight->setMinimum(0.f);
    heuristicsWeight->setMaximum(10.f);
    heuristicsWeight->setSingleStep(0.01f);
    controlsLayout->addRow(new QLabel("Heuristics weight"), heuristicsWeight);

    QObject::connect(heuristicsWeight, SIGNAL(valueChanged(double)), this, SLOT(setHeuristicsWeight(double)));
    // Disable adjusting heuristics weight when an action is running (it has no effect anyways)
    QObject::connect(&controller->getTimer(), &ActionTimer::activated, heuristicsWeight, &QSlider::setDisabled);

    QObject::connect(this, &MainWindowCentralWidget::worldLoaded, [this]() {
        // Keep window centered on protagonist
        QObject::connect(&*controller->getWorldModel()->protagonist(), &Protagonist::posChanged, [this](int x, int y) {
            graphicsView->centerOn(x, y);
        });
    });



    // Controls startup values
    animationSpeed->setValue(5);
    sceneScale->setValue(100);
    heuristicsWeight->setValue(0.1f);
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

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    // Mouse event coordinates are GraphicsView widget coordinates,
    // they must be mapped to scene coordinates to be usefull in the world.
    QPointF pos = mapToScene(event->pos());
    int posX = std::floor(pos.x());
    int posY = std::floor(pos.y());
    if(posX > 0 && posX < scene()->width()  && posY > 0 && posY < scene()->height()) {
        emit tilePressed(posX, posY);
    }
}
