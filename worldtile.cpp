#include "worldtile.h"
#include <cassert>
#include <limits>
#include <cmath>

WorldTile::WorldTile(std::shared_ptr<Tile> tile):
    _tile(tile), _status(WorldTile::Status::none), _enemy(nullptr)
{

}

void WorldTile::setEnemy(std::shared_ptr<Enemy> enemy)
{
    assert(_enemy == nullptr);
    assert(enemy->getXPos() == getX() && enemy->getYPos() == getY());
    _enemy = enemy;
    emit changed();
}

void WorldTile::setHealthpack(std::shared_ptr<Tile> healthpack)
{
    assert(_healthpack == 0);
    assert(healthpack->getXPos() == getX() && healthpack->getYPos() == getY());
    _healthpack = healthpack->getValue();
    emit changed();
}

void WorldTile::setStatus(WorldTile::Status status)
{
    _status = status;
    emit changed();
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
    return (1-_tile->getValue()) + getPoisonEffect() / 10.f;
}

float WorldTile::getHealthpack() const
{
    return _healthpack;
}

float WorldTile::getHealthEffect() const
{
    float healthEffect = getHealthpack();
    if(_enemy != nullptr && std::dynamic_pointer_cast<PEnemy>(_enemy) == nullptr)
        healthEffect-=_enemy->getValue();
    return healthEffect;
}

void WorldTile::depleteHealthpack()
{
    if(_healthpack != 0) {
        _healthpack = 0;
        emit changed();
    }
    _healthpack = 0;
}

bool WorldTile::killEnemy()
{
    if(_enemy == nullptr || _enemy->getDefeated())
        return false;
    _enemy->setDefeated(true);
    emit changed();
    return true;
}

void WorldTile::addPoisonEffect(float poison)
{

    _poison+=poison;
    emit changed();
}

float WorldTile::getPoisonEffect() const
{
    return std::min(_poison,100.f);
}

std::shared_ptr<Enemy> WorldTile::enemy() const
{
    return _enemy;
}
