#include "worldcontroller.h"
#include <cmath>
#include "graphicstile.h"
#include "pathfinder.h"
#include <chrono>
#include <QDebug>
#include <QImage>
WorldController::WorldController()
    : range(20), scale(10)
{
}

void WorldController::createWorld(QString file)
{
    World world;

    std::vector<std::unique_ptr<Tile>> tilesList = world.createWorld(file);
    tiles = new DenseMatrix<WorldTile>(world.getRows(), world.getCols());

    backgroundImage = new QGraphicsPixmapItem(QPixmap::fromImage(QImage(file)));

    for(std::unique_ptr<Tile> &tile: tilesList) {
        WorldTile wt(std::move(tile));
        tiles->set(wt.getY(), wt.getX(), wt);
    }

    auto enemiesList = world.getEnemies(8);
    for(std::unique_ptr<Enemy> &enemy: enemiesList) {
        int row = enemy->getYPos();
        int col = enemy->getXPos();
        tiles->get(row, col).setEnemy(std::move(enemy));
    }

    auto healthpacksList = world.getHealthPacks(8);
    for(std::unique_ptr<Tile> &healthpack: healthpacksList) {
        int row = healthpack->getYPos();
        int col = healthpack->getXPos();
        tiles->get(row, col).setHealthpack(std::move(healthpack));
    }

    protagonist = std::move(world.getProtagonist());
    path = new PathFinder(5,5, 990, 937, tiles); //world map 990 937
    path->AStarInit();
}


std::unique_ptr<Matrix<WorldTile>> WorldController::getTilesAroundProtagonist()
{
    if(debugMode)
        return tiles->unsafeSlice(0, 0, tiles->rows()-1, tiles->cols()-1);
    int rowStart = protagonist->getYPos()-range;
    int colStart = protagonist->getXPos()-range;
    int rowEnd = protagonist->getYPos()+range;
    int colEnd = protagonist->getXPos()+range;
    return tiles->unsafeSlice(rowStart, colStart, rowEnd, colEnd);
}


void WorldController::render(QGraphicsScene& scene)
{
    auto tilesAroundProtagonist = getTilesAroundProtagonist();
    auto firstTile = tilesAroundProtagonist->begin();
    backgroundImage->setScale(scale);
    backgroundImage->setPos(0, 0);
    scene.addItem(backgroundImage);

    for(const WorldTile &worldtile: *tilesAroundProtagonist) {

        auto status = worldtile.status();

        if(status != WorldTile::Status::none) {
            QGraphicsRectItem* rect = new QGraphicsRectItem;
            rect->setRect(0,0,1,1);
            rect->setPos(worldtile.getX()*scale, worldtile.getY()*scale);
            rect->setScale(scale);
            rect->setZValue(4);
            rect->setPen(Qt::NoPen);
            rect->setOpacity(0.2);
            switch(status) {
            case WorldTile::Status::openlist:
                rect->setBrush(QBrush(Qt::darkYellow));
                break;
            case WorldTile::Status::closedlist:
                rect->setBrush(QBrush(Qt::blue));
                break;
            case WorldTile::Status::solution:
                rect->setOpacity(0.5);
                rect->setBrush(QBrush(Qt::green));
                break;
            }
            scene.addItem(rect);
        }

        auto enemy = worldtile.enemy();
        if(enemy != nullptr) {
            GraphicsEnemy* genemy = new GraphicsEnemy(enemy);
            genemy->setPos(worldtile.getX()*scale, worldtile.getY()*scale);
            genemy->setScale(scale);
            genemy->setZValue(2);
            scene.addItem(genemy);
        }

        auto healthpack = worldtile.healthpack();
        if(healthpack != nullptr) {
            GraphicsHealthpack* ghealthpack = new GraphicsHealthpack(healthpack);
            ghealthpack->setPos(healthpack->getXPos()*scale, healthpack->getYPos()*scale);
            ghealthpack->setScale(scale);
            ghealthpack->setZValue(2);
            scene.addItem(ghealthpack);
        }
    }

    GraphicsProtagonist* gprotagonist = new GraphicsProtagonist(protagonist);
    gprotagonist->setPos(protagonist->getXPos()*scale, protagonist->getYPos()*scale);
    gprotagonist->setScale(scale);
    gprotagonist->setZValue(3);
    scene.addItem(gprotagonist);
}

void WorldController::moveProtagonist(int x, int y)
{
    int newCol = protagonist->getXPos()+x;
    int newRow = protagonist->getYPos()+y;
    if(tiles->contains(newRow, newCol)) {
        protagonist->setPos(newCol, newRow);
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
        qDebug() << duration << "ms";

}





