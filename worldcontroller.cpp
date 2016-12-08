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
    tiles = new DenseMatrix<std::shared_ptr<PStruct>>(world.getRows(), world.getCols());

    backgroundImage = new QGraphicsPixmapItem(QPixmap::fromImage(QImage(file)));

    for(std::unique_ptr<Tile> &tile: tilesList) {
        int row = tile->getYPos();
        int col = tile->getXPos();
        std::shared_ptr<PStruct> ps = std::make_shared<PStruct>();
        ps->tile=std::move(tile);
        ps->enemy=nullptr;
        ps->healthpack=nullptr;
        ps->pathStatus=Status::none;
        tiles->set(row, col, ps);
    }

    auto enemiesList = world.getEnemies(8);
    for(std::unique_ptr<Enemy> &enemy: enemiesList) {
        int row = enemy->getYPos();
        int col = enemy->getXPos();
        tiles->get(row, col)->enemy = std::move(enemy);
    }

    auto healthpacksList = world.getHealthPacks(8);
    for(std::unique_ptr<Tile> &healthpack: healthpacksList) {
        int row = healthpack->getYPos();
        int col = healthpack->getXPos();
        tiles->get(row, col)->healthpack = std::move(healthpack);
    }

    protagonist = std::move(world.getProtagonist());
    path = new PathFinder(5,5, 493, 493, tiles); //world map 990 937
    path->AStarInit();
}


std::unique_ptr<Matrix<std::shared_ptr<PStruct>>> WorldController::getTilesAroundProtagonist()
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
    if(debugMode)
        backgroundImage->setPos(-range*scale, -range*scale);
    else
        backgroundImage->setPos(-protagonist->getXPos()*scale,-protagonist->getYPos()*scale) ;
    scene.addItem(backgroundImage);

    for(const auto &pstruct: *tilesAroundProtagonist) {

        auto status = pstruct.value->pathStatus;

        if(status != none) {
            QGraphicsRectItem* rect = new QGraphicsRectItem;
            rect->setRect(0,0,1,1);
            rect->setPos(pstruct.col*scale - range*scale, pstruct.row*scale - range*scale);
            rect->setScale(scale);
            rect->setZValue(4);
            rect->setPen(Qt::NoPen);
            rect->setOpacity(0.2);
            switch(status) {
            case openlist:
                rect->setBrush(QBrush(Qt::darkYellow));
                break;
            case closedlist:
                rect->setBrush(QBrush(Qt::blue));
                break;
            case solution:
                rect->setOpacity(0.5);
                rect->setBrush(QBrush(Qt::green));
                break;
            }
            scene.addItem(rect);
        }

        auto enemy = pstruct.value->enemy;
        if(enemy != nullptr) {
            GraphicsEnemy* genemy = new GraphicsEnemy(enemy);
            genemy->setPos(pstruct.col*scale - range*scale, pstruct.row*scale - range*scale);
            genemy->setScale(scale);
            genemy->setZValue(2);
            scene.addItem(genemy);
        }
        auto healthpack = pstruct.value->healthpack;
        if(healthpack != nullptr) {
            GraphicsHealthpack* ghealthpack = new GraphicsHealthpack(healthpack);
            ghealthpack->setPos(pstruct.col*scale - range*scale, pstruct.row*scale - range*scale);
            ghealthpack->setScale(scale);
            ghealthpack->setZValue(2);
            scene.addItem(ghealthpack);
        }
    }

    GraphicsProtagonist* gprotagonist = new GraphicsProtagonist(protagonist);
    gprotagonist->setPos(0, 0);
    gprotagonist->setScale(scale);
    gprotagonist->setZValue(3);
    scene.addItem(gprotagonist);
}

void WorldController::moveProtagonist(int x, int y)
{
    int newCol = protagonist->getXPos()+x;
    int newRow = protagonist->getYPos()+y;
    if(tiles->contains(newRow, newCol)) {
        if(!std::isinf(tiles->get(newRow, newCol)->tile->getValue()))
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





