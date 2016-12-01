#ifndef PATHFINDER_H
#define PATHFINDER_H

#include<world.h>
#include <matrix.h>
#include "worldcontroller.h"


typedef struct Node{
std::shared_ptr<PStruct> pstruct;
std::shared_ptr<Node> parent;
float finalCost = 0;
float givenCost = 0;
int x =0; // maakt debuggen makkelijker
int y = 0;

bool operator==(const Node& a) const  // nodig voor std::find
    {
        return (pstruct->tile == a.pstruct->tile);
    }

};

struct { // compare function used for priority queue
    bool operator()(Node const & p1, Node const & p2) {
        // return "true" if "p1" is ordered before "p2", for example:
        return p1.finalCost > p2.finalCost; //kleinste cost moet bovenaan priority queue
    }
} CompareNode;

class PathFinder
{
public:
    PathFinder(int xstart, int ystart, int xend, int yend, Matrix<std::shared_ptr<PStruct>>* matrix);
    std::deque<Node> Run();
    std::deque<Node> RunAStar();
private:
    int makeHash(int x, int y){return x<<16 |y;};
    float calcHeuristicScore(int x, int y);
    Matrix<std::shared_ptr<PStruct>>* _matrix;
    std::deque<Node> openList;
    std::deque<Node> closedList;
    std::deque<Node> resultList;
    int _xstart;
    int _ystart;
    int _xend;
    int _yend;
};






#endif // PATHFINDER_H
