#ifndef WORLDMODEL_H
#define WORLDMODEL_H

#include "matrix.h"
#include "worldtile.h"

class WorldModel
{
public:
    WorldModel(Matrix<std::shared_ptr<WorldTile>> *tiles, std::shared_ptr<Protagonist> protagonist);
    std::unique_ptr<Matrix<std::shared_ptr<WorldTile>>> tilesAroundProtagonist(int range);
    bool moveProtagonist(int dx, int dy);
    std::shared_ptr<Protagonist> protagonist() const { return _protagonist; }
    Matrix<std::shared_ptr<WorldTile>> *tiles() const { return _tiles; }
    ~WorldModel();
private:
    Matrix<std::shared_ptr<WorldTile>> *_tiles;
    std::shared_ptr<Protagonist> _protagonist;
};

#endif // WORLDMODEL_H
