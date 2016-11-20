#include "worldcontroller.h"
#include <cmath>
#include "graphicstile.h"


template<class T>
void tilesVectorToMatrix(std::vector<std::unique_ptr<T>> &tilesList, std::unique_ptr<Matrix<std::shared_ptr<T>>> &tiles)
{
    for(std::unique_ptr<T> &tile: tilesList) {
        int row = tile->getYPos();
        int col = tile->getXPos();
        if(tile->getValue() > 1.0 && ! std::isinf(tile->getValue())) {
            std::cout << row << "," << col << std::endl;
        }
        tiles->set(row, col, std::move(tile));
    }
}

WorldController::WorldController()
    : range(20), scale(10)
{
}

void WorldController::createWorld(QString file)
{
    World world;

    std::vector<std::unique_ptr<Tile>> tilesList = world.createWorld(file);

    tiles = std::unique_ptr<MTiles>(new DenseMatrix<std::shared_ptr<Tile>>(world.getRows(), world.getCols()));
    enemies = std::unique_ptr<MEnemies>(new SparseMatrix<std::shared_ptr<Enemy>>(world.getRows(), world.getCols()));
    healthpacks = std::unique_ptr<MTiles>(new SparseMatrix<std::shared_ptr<Tile>>(world.getRows(), world.getCols()));


    tilesVectorToMatrix(tilesList, tiles);
    auto enemiesList = world.getEnemies(8);
    tilesVectorToMatrix(enemiesList, enemies);
    auto healthpacksList = world.getHealthPacks(8);
    tilesVectorToMatrix(healthpacksList, healthpacks);

    protagonist = std::move(world.getProtagonist());
}


template<class T>
std::unique_ptr<T> WorldController::getTilesAroundProtagonist(std::unique_ptr<T> &matrix)
{
    int rowStart = protagonist->getYPos()-range;
    int colStart = protagonist->getXPos()-range;
    int rowEnd = protagonist->getYPos()+range;
    int colEnd = protagonist->getXPos()+range;
    return matrix->unsafeSlice(rowStart, colStart, rowEnd, colEnd);
}


void WorldController::render(QGraphicsScene& scene)
{
    scene.clear();
    auto tilesAroundProtagonist = getTilesAroundProtagonist(tiles);
    for(const auto &tile: *tilesAroundProtagonist) {
        GraphicsTile* gtile = new GraphicsTile(tile.value);
        gtile->setPos(tile.col*scale - range*scale, tile.row*scale - range*scale);
        gtile->setScale(scale);
        scene.addItem(gtile);
    }

    auto enemiesAroundProtagonist = getTilesAroundProtagonist(enemies);
    for(const auto &enemy: *enemiesAroundProtagonist) {
        GraphicsEnemy* genemy = new GraphicsEnemy(enemy.value);
        genemy->setPos(enemy.col*scale - range*scale, enemy.row*scale - range*scale);
        genemy->setScale(scale);
        scene.addItem(genemy);
    }

    auto healthpacksAroundProtagonist = getTilesAroundProtagonist(healthpacks);
    for(const auto &healthpack: *healthpacksAroundProtagonist) {
        GraphicsHealthpack* ghealthpack = new GraphicsHealthpack(healthpack.value);
        ghealthpack->setPos(healthpack.col*scale - range*scale, healthpack.row*scale - range*scale);
        ghealthpack->setScale(scale);
        scene.addItem(ghealthpack);
    }

    GraphicsProtagonist* gprotagonist = new GraphicsProtagonist(protagonist);
    gprotagonist->setPos(0, 0);
    gprotagonist->setScale(scale);
    scene.addItem(gprotagonist);
}

void WorldController::moveProtagonist(int x, int y)
{
    int newCol = protagonist->getXPos()+x;
    int newRow = protagonist->getYPos()+y;
    if(tiles->contains(newRow, newCol))
        protagonist->setPos(newCol, newRow);
}





