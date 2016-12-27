#include "pathfinder.h"
#include <algorithm>
#include <set>
#include <cmath>
PathFinder::PathFinder(int xstart, int ystart, int xend, int yend, std::shared_ptr<Matrix<std::shared_ptr<WorldTile> > > matrix):
    _xstart(xstart), _ystart(ystart), _xend(xend), _yend(yend),_matrix(matrix) ,statusMatrix(matrix->rows(), matrix->cols())
{
    for(int i =0; i<statusMatrix.rows();i++){
        for(int j = 0; j<statusMatrix.cols();j++){
            statusMatrix.set(i,j,WorldTile::Status::none);
        }
    }

    AStarInit();

}


bool PathFinder::RunAStarStep()
{
    if(solutionFound||openList.empty())return true;
    Node currentNode;
    //3.1 +3.2

    do {
        currentNode = openList.top(); //get the best node form open List
        if(!openList.empty())
            openList.pop();//delete currentNode from open
        else{
            return true;}

    } while(statusMatrix.get(currentNode.tile->getY(), currentNode.tile->getX()) == WorldTile::Status::closedlist);


    //add current node to closed
    statusMatrix.set(currentNode.tile->getY(),currentNode.tile->getX(),WorldTile::Status::closedlist);

    if (currentNode.tile->getX() == _xend && currentNode.tile->getY()== _yend){ // the current node is the solution node
        solutionNode = currentNode;
        solutionFound = true;
        return solutionFound;
    }

    //std::cout << "Best node: " << currentNode.x << ", " << currentNode.y << std::endl;

    // 3.3

    std::shared_ptr<Node> parent_ptr = std::make_shared<Node>(currentNode);
    for(int i= -1; i<=1;i++){ // ga alle posities rond current af
        for(int j=-1; j <= 1; j++){
            if(i==0 && j==0)
                continue;// no need to do the current node all over again.
            if(!(i==0 || j==0))
                continue; // no diagonals
            int x = currentNode.tile->getX() +i;
            int y = currentNode.tile->getY() +j;

            if(_matrix->contains(y,x)){ //positie in range of matrix
                Node nearbyNode;
                nearbyNode.tile = _matrix->get(y,x);
                nearbyNode.x = x;nearbyNode.y = y;

                if(!std::isinf(nearbyNode.tile->getDifficulty())){ // if passable

                    nearbyNode.parent= parent_ptr; // set parent pointer to current node
                    nearbyNode.givenCost = currentNode.givenCost + nearbyNode.tile->getDifficulty();
                    nearbyNode.finalCost = nearbyNode.givenCost + calcHeuristicScore(x,y)/10 ;
                    if( statusMatrix.get(nearbyNode.tile->getY(),nearbyNode.tile->getX())!= WorldTile::Status::closedlist){ //  if not already in Lists
                        openList.push(nearbyNode); //add to open list
                        statusMatrix.set(nearbyNode.tile->getY(),nearbyNode.tile->getX(),WorldTile::Status::openlist);
                    }


                }
            }
        }

    }





    //std::sort(openList.begin(),openList.end(),CompareNode); //sort the open list on final cost

    return false;

}

void PathFinder::AStarInit()
{   clearVisuals();
    //step 1 Breadth-First
    Node startNode;
    startNode.parent = nullptr;
    startNode.finalCost = 0;
    startNode.givenCost = 0;
    startNode.tile = _matrix->get(_ystart,_xstart);

    //step 2
    openList.push(startNode);
    statusMatrix.set(startNode.tile->getY(),startNode.tile->getX(),WorldTile::Status::openlist);

}

std::deque<Node> PathFinder::AStarSolution()
{
    Node currentNode = solutionNode;
    // sollution should be found now
    if (solutionFound && currentNode.tile->getX() == _xend && currentNode.tile->getY()== _yend){ // oplossing gevonden
        while(currentNode.parent != nullptr){
            resultList.push_front(currentNode);
            statusMatrix.set(currentNode.tile->getY(),currentNode.tile->getX(),WorldTile::Status::solution);
            currentNode = *(currentNode.parent);
        }
        resultList.push_front(currentNode);
        statusMatrix.set(currentNode.tile->getY(),currentNode.tile->getX(),WorldTile::Status::solution);
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
    return (abs(x-_xend)+abs(y-_yend));
}

void PathFinder::showVisuals(){
    for(int i =0; i<statusMatrix.rows();i++){
        for(int j = 0; j<statusMatrix.cols();j++){
            if(statusMatrix.get(i,j) !=WorldTile::Status::none){
                _matrix->get(i,j)->setStatus(statusMatrix.get(i,j));
            }
        }
    }
}

void PathFinder::clearVisuals(){
    for(auto i : *_matrix){
        if(i->status() != WorldTile::Status::none)
            i->setStatus(WorldTile::Status::none);
    }
}


