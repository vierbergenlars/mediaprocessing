#include "worldtile.h"
#include <cassert>
#include <limits>
#include <cmath>

WorldTile::WorldTile(std::shared_ptr<Tile> tile):
    _tile(tile), _status(WorldTile::Status::none), _enemy(nullptr), _healthpack(nullptr)
{

}

void WorldTile::setEnemy(std::shared_ptr<Enemy> enemy)
{
    assert(_enemy == nullptr);
    assert(enemy->getXPos() == getX() && enemy->getYPos() == getY());
    _enemy = enemy;
}

void WorldTile::setHealthpack(std::shared_ptr<Tile> healthpack)
{
    assert(_healthpack == nullptr);
    assert(healthpack->getXPos() == getX() && healthpack->getYPos() == getY());
    _healthpack = healthpack;
}

void WorldTile::setStatus(WorldTile::Status status)
{
    _status = status;
}

WorldTile::Status WorldTile::status() const
{
    return _status;
}

int WorldTile::getX() const
{
    return _tile->getXPos();
}

int WorldTile::getY() const
{
    return _tile->getYPos();
}

float WorldTile::getDifficulty() const
{
    if(std::isinf(_tile->getValue()))
        return std::numeric_limits<float>::infinity();
    if(_enemy != nullptr && _enemy->getDefeated())
        return std::numeric_limits<float>::infinity();
    return (1-_tile->getValue());
}

float WorldTile::getHealthpack() const
{
    if(_healthpack == nullptr)
        return 0;
    return _healthpack->getValue();
}

std::shared_ptr<Tile> WorldTile::healthpack() const
{
    return _healthpack;
}

std::shared_ptr<Enemy> WorldTile::enemy() const
{
    return _enemy;
}
