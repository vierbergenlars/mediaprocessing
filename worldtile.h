#ifndef WORLDTILE_H
#define WORLDTILE_H

#include <world.h>

class WorldTile
{
public:
    enum Status {none, openlist, closedlist, solution};
    WorldTile(std::shared_ptr<Tile> tile = nullptr);
    void setEnemy(std::shared_ptr<Enemy> enemy);
    void setHealthpack(std::shared_ptr<Tile> healthpack);
    void setStatus(Status status);
    Status status() const;
    int getX() const;
    int getY() const;
    float getDifficulty() const;
    float getHealthpack() const;
    float getHealthEffect() const;
    bool hasItem() const {
        return _status != none || _enemy != nullptr || _healthpack != 0 || _poison != 0;
    }
    bool graphicsConstructed = false;
    std::shared_ptr<Enemy> enemy() const;
    void depleteHealthpack();
    void addPoisonEffect(float poison);
    float getPoisonEffect() const;
private:
    std::shared_ptr<Tile> _tile;
    std::shared_ptr<Enemy> _enemy;
    float _healthpack = 0;
    float _poison = 0;
    Status _status;
};

#endif // WORLDTILE_H
