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

    _protagonist->setEnergy(newEnergy);
    _protagonist->setPos(newX, newY);
    return true;
}

WorldModel::~WorldModel()
{
    delete _tiles;
}
