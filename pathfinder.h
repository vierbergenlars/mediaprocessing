#ifndef PATHFINDER_H
#define PATHFINDER_H

#include<world.h>
#include <matrix.h>
#include "worldtile.h"
#include <queue>
#include <deque>


struct Node{
    std::shared_ptr<WorldTile> tile;
std::shared_ptr<Node> parent;
float finalCost = 0;
float givenCost = 0;
int x =0; // maakt debuggen makkelijker
int y = 0;
};


struct CompareNode{ // compare function used for priority queue
    bool operator()(Node const & p1, Node const & p2) {
        // return "true" if "p1" is ordered before "p2", for example:
        return p1.finalCost > p2.finalCost; //kleinste cost moet bovenaan priority queue
    }
};


class PathFinder
{
public:

    PathFinder(int xstart, int ystart, int xend, int yend, std::shared_ptr<Matrix<std::shared_ptr<WorldTile>>> matrix, float heuristicsWeight);
    std::deque<Node> Run();
    std::deque<Node> RunAStar();
    void AStarInit();
    bool RunAStarStep();
    std::deque<Node> AStarSolution();
    void showVisuals();
    void clearVisuals();
private:
    Node currentBestNode;
    Node solutionNode;
    bool solutionFound = false;
    float calcHeuristicScore(int x, int y);
    std::shared_ptr<Matrix<std::shared_ptr<WorldTile>>> _matrix;
    std::priority_queue<Node,std::deque<Node>, CompareNode> openList;
    std::deque<Node> closedList;
    std::deque<Node> resultList;
    float heuristicsWeight;
    int _xstart;
    int _ystart;
    int _xend;
    int _yend;
    DenseMatrix<WorldTile::Status> statusMatrix;
};








#endif // PATHFINDER_H
