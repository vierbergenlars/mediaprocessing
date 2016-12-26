#include "worldmodel.h"
#include <cmath>

WorldModel::WorldModel(Matrix<std::shared_ptr<WorldTile> > *tiles, std::shared_ptr<Protagonist> protagonist)
    :_tiles(tiles), _protagonist(protagonist)
{

}

std::unique_ptr<Matrix<std::shared_ptr<WorldTile> > > WorldModel::tilesAroundProtagonist(int range)
{
    int rowStart = std::max(0, _protagonist->getYPos() - range);
    int colStart = std::max(0, _protagonist->getXPos() - range);

    int rowEnd = std::min(_tiles->rows()-1, _protagonist->getYPos() + range);
    int colEnd = std::max(_tiles->cols()-1, _protagonist->getXPos() + range);

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

    _protagonist->setHealth(newHealth);
    tile->depleteHealthpack();

    std::shared_ptr<Enemy> enemy = tile->enemy();

    if(enemy != nullptr && !enemy->getDefeated()) {
        enemy->setDefeated(true);
        _protagonist->setEnergy(100.0f);
        std::shared_ptr<PEnemy> penemy = std::dynamic_pointer_cast<PEnemy>(enemy);
        if(penemy != nullptr) {
            penemy->poison();
        }
    }

    _protagonist->setEnergy(newEnergy);
    _protagonist->setPos(newX, newY);
    return true;
}

WorldModel::~WorldModel()
{
    delete _tiles;
}
