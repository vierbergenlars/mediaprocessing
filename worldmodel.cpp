#include "worldmodel.h"
#include <cmath>
#include "mypenemy.h"
#include <QObject>

WorldModel::WorldModel(Matrix<std::shared_ptr<WorldTile> > *tiles, std::shared_ptr<Protagonist> protagonist)
    :_tiles(tiles), _protagonist(protagonist)
{

}

std::unique_ptr<Matrix<std::shared_ptr<WorldTile> > > WorldModel::tilesAroundProtagonist(int range)
{
    return tilesAround(_protagonist, range);
}

std::unique_ptr<Matrix<std::shared_ptr<WorldTile> > > WorldModel::tilesAround(std::shared_ptr<Tile> tile, int range)
{
    int rowStart = std::max(0, tile->getYPos() - range);
    int colStart = std::max(0, tile->getXPos() - range);

    int rowEnd = std::min(_tiles->rows()-1, tile->getYPos() + range);
    int colEnd = std::min(_tiles->cols()-1, tile->getXPos() + range);

    return _tiles->slice(rowStart, colStart, rowEnd, colEnd);
}

bool WorldModel::moveProtagonist(int dx, int dy)
{
    assert(-1 <= dx && dx <= 1);
    assert(-1 <= dy && dy <= 1);

    int newX = _protagonist->getXPos() + dx;
    int newY = _protagonist->getYPos() + dy;

    if(!_tiles->contains(newY, newX))
        return false;

    std::shared_ptr<WorldTile> tile = _tiles->get(newY, newX);

    float newEnergy = _protagonist->getEnergy() - tile->getDifficulty();

    if(newEnergy <= 0)
        return false;
    float newHealth= _protagonist->getHealth() + tile->getHealthEffect();
    if(newHealth<=0)
        newHealth = 0;
    if(newHealth >=100)
        newHealth=100;

    _protagonist->setEnergy(newEnergy);

    _protagonist->setHealth(newHealth);
    tile->depleteHealthpack();

    std::shared_ptr<Enemy> enemy = tile->enemy();

    if(enemy != nullptr && !enemy->getDefeated()) {
        enemy->setDefeated(true);
        _protagonist->setEnergy(100.0f);
        std::shared_ptr<MyPEnemy> penemy = std::dynamic_pointer_cast<MyPEnemy>(enemy);
        if(penemy != nullptr) {
            doPoison(tile);
            penemy->poison();
        }
    }

    _protagonist->setPos(newX, newY);
    return true;
}

void WorldModel::doPoison(std::shared_ptr<WorldTile> tile)
{
    std::shared_ptr<MyPEnemy> penemy = std::dynamic_pointer_cast<MyPEnemy>(tile->enemy());
    float radius = penemy->getPoisonLevel()/10.0f;
    auto tiles = tilesAround(penemy, radius);

    for(std::shared_ptr<WorldTile> wt: *tiles) {
        float distance = std::hypot(tile->getX() - wt->getX(), tile->getY() - wt->getY());
        if(distance > radius)
            continue;
        QObject::connect(&*penemy, &MyPEnemy::poisoned, [wt, distance](float poison) {
            if(distance < poison/10.f)
                wt->addPoisonEffect(10.f);
        });
    }
}

WorldModel::~WorldModel()
{
    delete _tiles;
}

std::vector<std::shared_ptr<WorldTile> > WorldModel::getEnemyTiles()
{
    std::vector<std::shared_ptr<WorldTile> > enemyTiles;
    for(auto tile : *_tiles){

        if(tile->enemy() != nullptr) // is an enemy
            enemyTiles.push_back(tile);
    }
    return enemyTiles;
}

std::vector<std::shared_ptr<WorldTile> > WorldModel::getHealtPackTiles()
{
    std::vector<std::shared_ptr<WorldTile> > healtTiles;
    for(auto tile : *_tiles){

            if(tile->getHealthpack()>0) // is a healthpack
                healtTiles.push_back(tile);
        }

    return healtTiles;
}
