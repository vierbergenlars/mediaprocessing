#include "worldcontroller.h"
#include <cmath>
#include "graphicstile.h"
#include "pathfinder.h"
#include <chrono>
#include <QDebug>
#include <QImage>

WorldController::WorldController(QGraphicsScene *scene)
    : range(20), scale(10), scene(scene)
{
}

void WorldController::createWorld(QString file)
{
    World world;

    std::vector<std::unique_ptr<Tile>> tilesList = world.createWorld(file);
    tiles = new DenseMatrix<std::shared_ptr<WorldTile>>(world.getRows(), world.getCols());

    backgroundImage = new QGraphicsPixmapItem(QPixmap::fromImage(QImage(file)));
    backgroundImage->setPos(0, 0);
    scene->addItem(backgroundImage);

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

    for(std::shared_ptr<WorldTile> & t: *tiles) {
        if(!t->hasItem() || t->graphicsConstructed)
            continue;
        auto graphicsPos = new GraphicsPosition(t);
        positions.push_back(graphicsPos);
        scene->addItem(graphicsPos);
        t->graphicsConstructed=true;
    }

    protagonist = std::move(world.getProtagonist());
    path = new PathFinder(5,5, 990, 937, tiles); //world map 990 937
    path->AStarInit();

    gprotagonist = new GraphicsProtagonist();
    gprotagonist->setZValue(3);
    scene->addItem(gprotagonist);
}


std::unique_ptr<Matrix<std::shared_ptr<WorldTile>>> WorldController::getTilesAroundProtagonist()
{
    if(debugMode)
        return tiles->unsafeSlice(0, 0, tiles->rows()-1, tiles->cols()-1);
    int rowStart = protagonist->getYPos()-range;
    int colStart = protagonist->getXPos()-range;
    int rowEnd = protagonist->getYPos()+range;
    int colEnd = protagonist->getXPos()+range;
    return tiles->unsafeSlice(rowStart, colStart, rowEnd, colEnd);
}


void WorldController::render()
{
    for(std::shared_ptr<WorldTile> & t: *tiles) {
        if(!t->hasItem() || t->graphicsConstructed)
            continue;
        auto graphicsPos = new GraphicsPosition(t);
        positions.push_back(graphicsPos);
        scene->addItem(graphicsPos);
        t->graphicsConstructed=true;
    }

    for(GraphicsPosition *&graphicsPos: positions) {
        graphicsPos->updateScale(scale);
        graphicsPos->setZValue(2);
        graphicsPos->update();
    }
    backgroundImage->setScale(scale);


    gprotagonist->setPos(protagonist->getXPos()*scale, protagonist->getYPos()*scale);
    gprotagonist->setScale(scale);
}

void WorldController::moveProtagonist(int x, int y)
{
    int newCol = protagonist->getXPos()+x;
    int newRow = protagonist->getYPos()+y;
    if(tiles->contains(newRow, newCol)) {
       float newenergy = protagonist->getEnergy() - tiles->get(newRow, newCol)->getDifficulty();
       if(newenergy>0){
        protagonist->setEnergy(newenergy);
        protagonist->setPos(newCol, newRow);
       }
    }
}

// random bullshit pls ignore
bool WorldController::doPathfinderStep()
{
    if(path->RunAStarStep()) {
        path->AStarSolution();
        return true;
    }
    return false;
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

    return protagonist->getEnergy();
}
float WorldController::getProtagonistHealth(){
    return protagonist->getHealth();
}






