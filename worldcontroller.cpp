#include "worldcontroller.h"
#include <cmath>
#include "graphicstile.h"
#include "pathfinder.h"
#include <chrono>
#include <QDebug>
#include <QImage>
#include "worldtile.h"
#include <QPixmap>


WorldController::WorldController(QGraphicsView *view)
    : range(20), view(view), worldModel(nullptr)
{
}


void WorldController::createWorld(QString file, int enemies, int healthpacks)
{
    actionTimer.disconnect();
    GraphicsProtagonist *gprotagonist = new GraphicsProtagonist();
    gprotagonist->setZValue(3);
    view->scene()->addItem(gprotagonist);

    World world;

    std::vector<std::unique_ptr<Tile>> tilesList = world.createWorld(file);
    auto tiles = std::make_shared<DenseMatrix<std::shared_ptr<WorldTile>>>(world.getRows(), world.getCols());

    QGraphicsPixmapItem *backgroundImage = new QGraphicsPixmapItem;
    backgroundImage->setPos(0, 0);
    backgroundImage->setPixmap(QPixmap::fromImage(QImage(file)));
    view->scene()->addItem(backgroundImage);

    for(std::unique_ptr<Tile> &tile: tilesList) {
        std::shared_ptr<WorldTile> wt = std::make_shared<WorldTile>(std::move(tile));
        tiles->set(wt->getY(), wt->getX(), wt);
    }

    auto enemiesList = world.getEnemies(enemies);
    for(std::unique_ptr<Enemy> &enemy: enemiesList) {
        int row = enemy->getYPos();
        int col = enemy->getXPos();
        tiles->get(row, col)->setEnemy(std::move(enemy));
    }

    auto healthpacksList = world.getHealthPacks(healthpacks);
    for(std::unique_ptr<Tile> &healthpack: healthpacksList) {
        int row = healthpack->getYPos();
        int col = healthpack->getXPos();
        tiles->get(row, col)->setHealthpack(std::move(healthpack));
    }

    worldModel = std::make_shared<WorldModel>(tiles, std::move(world.getProtagonist()));
    QObject::connect(&*worldModel->protagonist(), &Protagonist::posChanged, [gprotagonist](int x, int y) {
        gprotagonist->setPos(x, y);
    });
    int searchRange = 0;
    while(std::isinf(worldModel->tiles()->get(worldModel->protagonist()->getYPos(), worldModel->protagonist()->getXPos())->getDifficulty())) {
        searchRange++;
        auto searchTiles = worldModel->tilesAroundProtagonist(searchRange);
        for(std::shared_ptr<WorldTile> wt: *searchTiles) {
            if(!std::isinf(wt->getDifficulty()))
                worldModel->protagonist()->setPos(wt->getX(), wt->getY());
        }
    }
}

void WorldController::render()
{
    for(std::shared_ptr<WorldTile> & t: *worldModel->tiles()) {
        if(!t->hasDrawable() || t->graphicsConstructed)
            continue;
        auto graphicsPos = new GraphicsPosition(t);
        graphicsPos->setPos(t->getX(), t->getY());
        view->scene()->addItem(graphicsPos);
        t->graphicsConstructed=true;
    }
}

void WorldController::moveProtagonist(int x, int y)
{
    worldModel->moveProtagonist(x, y);
}

void WorldController::doPathfinderSteps(int xTarget, int yTarget)
{
    if(std::isinf(worldModel->tiles()->get(yTarget, xTarget)->getDifficulty())) {
        qDebug() << "Sorry, I'm not going to visit an unreachable tile.";
        return;
    }
    std::shared_ptr<PathFinder> pathfinder = std::make_shared<PathFinder>(worldModel->protagonist()->getXPos(), worldModel->protagonist()->getYPos(), xTarget, yTarget, worldModel->tiles());

    pathfinder->AStarInit();
    actionTimer.connect([this, pathfinder]()->bool {
        int i = 0;
        bool solved = false;
        do {
            i++;
            solved = pathfinder->RunAStarStep();
        } while(i < std::pow(10, this->animationSpeed/10.f) && !solved);

        if(solved)
            pathfinder->AStarSolution();
        pathfinder->showVisuals();
        this->render();
        return !solved;
    });
}

void WorldController::playStrategy(){
    auto strategy = std::make_shared<Strategy>(worldModel);
    actionTimer.connect([this, strategy]() {
        int i = 0;
        Strategy::StepType hasNextStep = Strategy::StepType::none;
        do {
            i++;
            hasNextStep = strategy->doNextStep();
            if(hasNextStep == Strategy::StepType::pathfind)
                this->render();
        } while(i < this->animationSpeed && hasNextStep != Strategy::StepType::none);
        return hasNextStep != Strategy::StepType::none;
    });
}

void WorldController::stopTimer()
{
    actionTimer.disconnect();
}

ActionTimer& WorldController::getTimer()
{
    return actionTimer;
}


std::shared_ptr<WorldModel> WorldController::getWorldModel()
{
 return worldModel;
}
float WorldController::getProtagonistEnergy(){

    return worldModel->protagonist()->getEnergy();
}
float WorldController::getProtagonistHealth(){
    return worldModel->protagonist()->getHealth();
}






