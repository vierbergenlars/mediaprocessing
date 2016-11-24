#include "worldcontroller.h"
#include <cmath>
#include "graphicstile.h"

WorldController::WorldController()
    : range(20), scale(10)
{
}

void WorldController::createWorld(QString file)
{
    World world;

    std::vector<std::unique_ptr<Tile>> tilesList = world.createWorld(file);
    tiles = new DenseMatrix<PStruct>(world.getRows(), world.getCols());

    for(std::unique_ptr<Tile> &tile: tilesList) {
        int row = tile->getYPos();
        int col = tile->getXPos();
        tiles->set(row, col, {std::move(tile), nullptr, 0});
    }

    auto enemiesList = world.getEnemies(8);
    for(std::unique_ptr<Enemy> &enemy: enemiesList) {
        int row = enemy->getYPos();
        int col = enemy->getXPos();
        tiles->get(row, col).enemy = std::move(enemy);
    }

    auto healthpacksList = world.getHealthPacks(8);
    for(std::unique_ptr<Tile> &healthpack: healthpacksList) {
        int row = healthpack->getYPos();
        int col = healthpack->getXPos();
        tiles->get(row, col).healthpack = std::move(healthpack);
    }

    protagonist = std::move(world.getProtagonist());
}


std::unique_ptr<Matrix<PStruct>> WorldController::getTilesAroundProtagonist()
{
    int rowStart = protagonist->getYPos()-range;
    int colStart = protagonist->getXPos()-range;
    int rowEnd = protagonist->getYPos()+range;
    int colEnd = protagonist->getXPos()+range;
    return tiles->unsafeSlice(rowStart, colStart, rowEnd, colEnd);
}


void WorldController::render(QGraphicsScene& scene)
{
    auto tilesAroundProtagonist = getTilesAroundProtagonist();

    for(const auto &pstruct: *tilesAroundProtagonist) {
        auto tile = pstruct.value.tile;
        GraphicsTile* gtile = new GraphicsTile(tile);
        gtile->setPos(pstruct.col*scale - range*scale, pstruct.row*scale - range*scale);
        gtile->setScale(scale);
        gtile->setZValue(1);;
        scene.addItem(gtile);

        auto enemy = pstruct.value.enemy;
        if(enemy != nullptr) {
            GraphicsEnemy* genemy = new GraphicsEnemy(enemy);
            genemy->setPos(pstruct.col*scale - range*scale, pstruct.row*scale - range*scale);
            genemy->setScale(scale);
            genemy->setZValue(2);
            scene.addItem(genemy);
        }
        auto healthpack = pstruct.value.healthpack;
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
        if(!std::isinf(tiles->get(newRow, newCol).tile->getValue()))
            protagonist->setPos(newCol, newRow);
    }
}





