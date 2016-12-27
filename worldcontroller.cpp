#include "worldcontroller.h"
#include <cmath>
#include "graphicstile.h"
#include "pathfinder.h"
#include <chrono>
#include <QDebug>
#include <QImage>
#include "worldtile.h"
#include <QPixmap>


WorldController::WorldController(QGraphicsScene *scene)
    : range(20), scale(10), scene(scene), worldModel(nullptr)
{
    gprotagonist = new GraphicsProtagonist();
    gprotagonist->setZValue(3);
    scene->addItem(gprotagonist);

    backgroundImage = new QGraphicsPixmapItem;
    backgroundImage->setPos(0, 0);
    scene->addItem(backgroundImage);
}

WorldController::~WorldController()
{
    delete worldModel;
    delete path;

}

void WorldController::createWorld(QString file)
{
    World world;

    std::vector<std::unique_ptr<Tile>> tilesList = world.createWorld(file);
    auto tiles = new DenseMatrix<std::shared_ptr<WorldTile>>(world.getRows(), world.getCols());

    backgroundImage->setPixmap(QPixmap::fromImage(QImage(file)));

    for(std::unique_ptr<Tile> &tile: tilesList) {
        std::shared_ptr<WorldTile> wt = std::make_shared<WorldTile>(std::move(tile));
        tiles->set(wt->getY(), wt->getX(), wt);
    }

    auto enemiesList = world.getEnemies(8);
    for(std::unique_ptr<Enemy> &enemy: enemiesList) {
        int row = enemy->getYPos();
        int col = enemy->getXPos();
        tiles->get(row, col)->setEnemy(std::move(enemy));
    }

    auto healthpacksList = world.getHealthPacks(8);
    for(std::unique_ptr<Tile> &healthpack: healthpacksList) {
        int row = healthpack->getYPos();
        int col = healthpack->getXPos();
        tiles->get(row, col)->setHealthpack(std::move(healthpack));
    }

    for(GraphicsPosition* gp: positions) {
        scene->removeItem(gp);
        delete gp;
    }

    positions.clear();

    for(std::shared_ptr<WorldTile> & t: *tiles) {
        if(!t->hasDrawable() || t->graphicsConstructed)
            continue;
        auto graphicsPos = new GraphicsPosition(t);
        positions.push_back(graphicsPos);
        scene->addItem(graphicsPos);
        t->graphicsConstructed=true;
    }

    if(worldModel != nullptr)
        delete worldModel;

    worldModel = new WorldModel(tiles, std::move(world.getProtagonist()));
    updateScale(1);
}

void WorldController::render()
{
    for(std::shared_ptr<WorldTile> & t: *worldModel->tiles()) {
        if(!t->hasDrawable() || t->graphicsConstructed)
            continue;
        auto graphicsPos = new GraphicsPosition(t);
        positions.push_back(graphicsPos);
        graphicsPos->updateScale(scale);
        scene->addItem(graphicsPos);
        t->graphicsConstructed=true;
    }


    gprotagonist->setPos(worldModel->protagonist()->getXPos()*scale, worldModel->protagonist()->getYPos()*scale);
}

void WorldController::moveProtagonist(int x, int y)
{
    worldModel->moveProtagonist(x, y);
}

void WorldController::doPathfinderSteps(int xTarget, int yTarget, int timerLength)
{
    std::shared_ptr<PathFinder> pathfinder = std::make_shared<PathFinder>(worldModel->protagonist()->getXPos(), worldModel->protagonist()->getYPos(), xTarget, yTarget, worldModel->tiles());
    if(timerLength == 0) {
        pathfinder->RunAStar();
        pathfinder->showVisuals();
        this->render();
        return;
    }

    pathfinder->AStarInit();
    actionTimer.connect([this, pathfinder, timerLength]()->bool {
        int i = 0;
        bool solved = false;
        do {
            i++;
            solved = pathfinder->RunAStarStep();
        } while(i < timerLength && !solved);

        if(solved)
            pathfinder->AStarSolution();
        pathfinder->showVisuals();
        this->render();
        return !solved;
    });
}


bool WorldController::doPathfinderStep()
{

    if(path->RunAStarStep()) {
        path->AStarSolution();
        return true;
    }
   return false;
}

void WorldController::playStrategy(){
    auto strategy = std::make_shared<Strategy>(worldModel);
    actionTimer.connect([this, strategy]() {
        bool hasNextStep = strategy->doNextStep();
        this->render();
        return hasNextStep;
    });
}

void WorldController::stopTimer()
{
    actionTimer.disconnect();
}

void WorldController::updateScale(float scaleDiff)
{
    scale*=scaleDiff;
    range/=scaleDiff;
    for(GraphicsPosition *&graphicsPos: positions) {
        graphicsPos->updateScale(scale);
    }
    backgroundImage->setScale(scale);
    gprotagonist->setScale(scale);
    gprotagonist->setPos(worldModel->protagonist()->getXPos()*scale, worldModel->protagonist()->getYPos()*scale);
}



void WorldController::doPathfinder()
{

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        path->RunAStar();
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        qDebug() << duration << "µs";

}
float WorldController::getProtagonistEnergy(){

    return worldModel->protagonist()->getEnergy();
}
float WorldController::getProtagonistHealth(){
    return worldModel->protagonist()->getHealth();
}






