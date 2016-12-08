#include "pathfinder.h"
#include <algorithm>
#include <set>
#include <cmath>
PathFinder::PathFinder(int xstart, int ystart, int xend, int yend, Matrix<std::shared_ptr<PStruct>>* matrix):
    _xstart(xstart), _ystart(ystart), _xend(xend), _yend(yend),_matrix(matrix)
{

}


Node solutionNode;
bool solutionFound = false;
bool PathFinder::RunAStarStep()
{
    if(solutionFound)return true;
    Node currentNode;
    //3.1 +3.2

    do {
        currentNode = openList.top(); //get the best node form open List

        openList.pop();//delete currentNode from open

    } while(currentNode.pstruct->pathStatus == Status::closedlist);


    closedList.push_back(currentNode); //add current node to closed
    currentNode.pstruct->pathStatus = Status::closedlist;

    if (currentNode.pstruct->tile->getXPos() == _xend && currentNode.pstruct->tile->getYPos()== _yend){ // the current node is the solution node
        solutionNode = currentNode;
        solutionFound = true;
        return solutionFound;
    }

    //std::cout << "Best node: " << currentNode.x << ", " << currentNode.y << std::endl;

    // 3.3

    std::shared_ptr<Node> parent_ptr = std::make_shared<Node>(currentNode);
    for(int i= -1; i<=1;i++){ // ga alle posities rond current af
        for(int j=-1; j <= 1; j++){
            if(i==0 && j==0) continue;// no need to do the current node all over again.
            if(!(i==0 || j==0)) continue; // no diagonals
            int x = currentNode.pstruct->tile->getXPos() +i;
            int y = currentNode.pstruct->tile->getYPos() +j;

            if(_matrix->contains(y,x)){ //positie in range of matrix
                Node nearbyNode;
                nearbyNode.pstruct = _matrix->get(y,x);
                nearbyNode.x = x;nearbyNode.y = y;

                if(!std::isinf(nearbyNode.pstruct->tile->getValue())){ // if passable


                    // bool found = std::find(openList.begin(), openList.end(), nearbyNode) != openList.end();
                    // if true, dan al in open lijst
                    // if(!found) found = std::find(closedList.begin(), closedList.end(), nearbyNode) != closedList.end();// check in closedList



                    nearbyNode.parent= parent_ptr; // set parent pointer to current node
                    nearbyNode.givenCost = currentNode.givenCost + (1-nearbyNode.pstruct->tile->getValue());
                    nearbyNode.finalCost = nearbyNode.givenCost + calcHeuristicScore(x,y)/10;
                    //std::cout << nearbyNode.x << ", " << nearbyNode.y <<" GivenCost: " << nearbyNode.givenCost << " FinalCost: " << nearbyNode.finalCost << std::endl;
                    if( nearbyNode.pstruct->pathStatus != Status::closedlist){ //  if not already in Lists
                        openList.push(nearbyNode); //add to open list
                        nearbyNode.pstruct->pathStatus = Status::openlist;
                    }


                }
            }
        }

    }





    //std::sort(openList.begin(),openList.end(),CompareNode); //sort the open list on final cost
    return false;

}

void PathFinder::AStarInit()
{
    //step 1 Breadth-First
    Node startNode;
    startNode.parent = nullptr;
    startNode.finalCost = 0;
    startNode.givenCost = 0;
    startNode.pstruct = _matrix->get(_ystart,_xstart);

    //step 2
    openList.push(startNode);
    startNode.pstruct->pathStatus = Status::openlist;
}

std::deque<Node> PathFinder::AStarSolution()
{
    Node currentNode = solutionNode;
    // sollution should be found now
    if (currentNode.pstruct->tile->getXPos() == _xend && currentNode.pstruct->tile->getYPos()== _yend){ // oplossing gevonden
        while(currentNode.parent != nullptr){
            resultList.push_front(currentNode);
            currentNode.pstruct->pathStatus = Status::solution;
            currentNode = *(currentNode.parent);
        }
        resultList.push_front(currentNode);
        currentNode.pstruct->pathStatus = Status::solution;
    } //else geen oplossing, resultList is leeg
    return resultList;

}

std::deque<Node> PathFinder::RunAStar()
{
    AStarInit();

    //step 3
    while(!openList.empty()){
        if(RunAStarStep())
            break;
    }

    return AStarSolution();

}

float PathFinder::calcHeuristicScore(int x, int y) // distance to end point
{
    return sqrtf(pow(x-_xend,2) + pow(y-_yend,2));
}


