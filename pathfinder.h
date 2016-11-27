#ifndef PATHFINDER_H
#define PATHFINDER_H

#include<world.h>
#include <matrix.h>
#include "worldcontroller.h"

typedef struct Node{
std::shared_ptr<Tile> tile;
std::shared_ptr<Node> parent;
float score;

bool operator==(const Node& a) const
    {
        return (tile == a.tile);
    }

};

class PathFinder
{
public:
    PathFinder(int xstart, int ystart, int xend, int yend, Matrix<PStruct>* matrix);
    std::deque<Node> Run();
private:
    Matrix<PStruct>* _matrix;
    std::deque<Node> openList;
    std::deque<Node> closedList;
    std::deque<Node> resultList;
    int _xstart;
    int _ystart;
    int _xend;
    int _yend;
};






#endif // PATHFINDER_H
