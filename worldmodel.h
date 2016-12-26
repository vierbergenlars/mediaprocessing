#ifndef WORLDMODEL_H
#define WORLDMODEL_H

#include "matrix.h"
#include "worldtile.h"
#include <vector>

class WorldModel
{
public:
    WorldModel(Matrix<std::shared_ptr<WorldTile>> *tiles, std::shared_ptr<Protagonist> protagonist);
    std::unique_ptr<Matrix<std::shared_ptr<WorldTile>>> tilesAroundProtagonist(int range);
    bool moveProtagonist(int dx, int dy);
    std::shared_ptr<Protagonist> protagonist() const { return _protagonist; }
    Matrix<std::shared_ptr<WorldTile>> *tiles() const { return _tiles; }
    ~WorldModel();
    std::vector<std::shared_ptr<WorldTile>> getEnemyTiles();
    std::vector<std::shared_ptr<WorldTile>> getHealtPackTiles();
private:
    Matrix<std::shared_ptr<WorldTile>> *_tiles;
    std::shared_ptr<Protagonist> _protagonist;
    void doPoison(std::shared_ptr<WorldTile> tile);
    std::unique_ptr<Matrix<std::shared_ptr<WorldTile> > > tilesAround(std::shared_ptr<Tile> tile, int range);
};

#endif // WORLDMODEL_H
