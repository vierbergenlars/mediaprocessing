#ifndef WORLDTILE_H
#define WORLDTILE_H

#include <world.h>
#include <QObject>

class WorldTile: public QObject
{
    Q_OBJECT
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
        return _enemy != nullptr || _healthpack != 0;
    }
    bool hasDrawable() const {
        return _status != none || _poison != 0 || hasItem();
    }
    bool graphicsConstructed = false;
    std::shared_ptr<Enemy> enemy() const;
    void depleteHealthpack();
    void addPoisonEffect(float poison);
    float getPoisonEffect() const;
    bool killEnemy();
signals:
    void changed();
private:
    std::shared_ptr<Tile> _tile;
    std::shared_ptr<Enemy> _enemy;
    float _healthpack = 0;
    float _poison = 0;
    Status _status;
};

#endif // WORLDTILE_H
