#ifndef PATHFINDER_H
#define PATHFINDER_H

#include<world.h>
#include <matrix.h>
#include "worldcontroller.h"
#include <queue>
#include <deque>


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

struct CompareNode{ // compare function used for priority queue
    bool operator()(Node const & p1, Node const & p2) {
        // return "true" if "p1" is ordered before "p2", for example:
        return p1.finalCost > p2.finalCost; //kleinste cost moet bovenaan priority queue
    }
};

template<typename T, class Container, class Compare>
class my_priority_queue : public std::priority_queue<T, std::deque<T>, Compare> //wrapper for priority queue that adds find and remove functions
{
public:

   bool erase(typename std::deque<T>::iterator it){
       if (it != this->c.end()) {
           this->c.erase(it);
           std::make_heap(this->c.begin(), this->c.end(), this->comp); // at most 3*N complexity
           return true;
       }
       else {
           return false;
       }
   };
   typename std::deque<T>::iterator find(const T& node){
       return std::find(this->c.begin(),this->c.end(), node);
   };

   void update(){
       std::push_heap(this->c.begin(),this->c.end(),this->comp);
   }
};

class PathFinder
{
public:
    PathFinder(int xstart, int ystart, int xend, int yend, Matrix<std::shared_ptr<PStruct>>* matrix);
    std::deque<Node> Run();
    std::deque<Node> RunAStar();
    void AStarInit();
    bool RunAStarStep();
    std::deque<Node> AStarSolution();
private:
    Node currentBestNode;
    Node solutionNode;
    bool solutionFound = false;
    int makeHash(int x, int y){return x<<16 |y;};
    float calcHeuristicScore(int x, int y);
    Matrix<std::shared_ptr<PStruct>>* _matrix;
    my_priority_queue<Node,std::deque<Node>, CompareNode> openList;
    std::deque<Node> closedList;
    std::deque<Node> resultList;
    int _xstart;
    int _ystart;
    int _xend;
    int _yend;
};








#endif // PATHFINDER_H
