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
    std::shared_ptr<Tile> healthpack() const;
    std::shared_ptr<Enemy> enemy() const;
private:
    std::shared_ptr<Tile> _tile;
    std::shared_ptr<Enemy> _enemy;
    std::shared_ptr<Tile> _healthpack;
    Status _status;
};

#endif // WORLDTILE_H
