#include "pathfinder.h"
#include <algorithm>
#include <set>
#include <cmath>
PathFinder::PathFinder(int xstart, int ystart, int xend, int yend, Matrix<std::shared_ptr<PStruct>>* matrix):
    _xstart(xstart), _ystart(ystart), _xend(xend), _yend(yend),_matrix(matrix)
{

}

std::deque<Node> PathFinder::Run()
{
    //step 1 Breadth-First
    Node startNode;
    startNode.parent = nullptr;
    startNode.finalCost = 0;
    startNode.pstruct = _matrix->get(_xstart, _ystart);

    Node currentNode;
    //step 2
    openList.push_back(startNode);
    startNode.pstruct->pathStatus = Status::openlist;

    //step 3
    while(!openList.empty()){
        //3.1 +3.2
        currentNode = openList.back();
        if (currentNode.pstruct->tile->getXPos() == _xend && currentNode.pstruct->tile->getYPos()== _yend){ // the current node is the solution node
            break;
        }

        // 3.3

        std::shared_ptr<Node> parent_ptr = std::make_shared<Node>(currentNode);
        for(int i= 0; i<=2;i++){ // ga alle posities rond current af
            for(int j=0; j <= 2; j++){
                int x = currentNode.pstruct->tile->getXPos() +i-1;
                int y = currentNode.pstruct->tile->getYPos() +j - 1;
                if(_matrix->contains(y,x)){ //positie in range of matrix
                    Node nearbyNode;
                    nearbyNode.pstruct = _matrix->get(y,x);
                    if(!std::isinf(nearbyNode.pstruct->tile->getValue())){ // if passable
                        // bool found = std::find(openList.begin(), openList.end(), nearbyNode) != openList.end(); // std::find(openList.begin(), openList.end(), nearbyNode) != openList.end()
                        // if true, dan al in open lijst
                        // if(!found) found = std::find(closedList.begin(), closedList.end(), nearbyNode) != closedList.end();// check in closedList
                        if( nearbyNode.pstruct->pathStatus == Status::none){ //  if not already in Lists
                            nearbyNode.parent= parent_ptr; // set parrent pointer to current node
                            openList.push_front(nearbyNode); //add to open list
                            nearbyNode.pstruct->pathStatus = Status::openlist;
                        }
                    }
                }

            }

        }
        openList.pop_back();//delete currentNode from open
        closedList.push_back(currentNode); //add current node to closed
        currentNode.pstruct->pathStatus = Status::closedlist;
    }

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
    //step 1 Breadth-First
    Node startNode;
    startNode.parent = nullptr;
    startNode.finalCost = 0;
    startNode.givenCost = 0;
    startNode.pstruct = _matrix->get(_ystart,_xstart);

    Node currentNode;
    //step 2
    openList.push_back(startNode);
    startNode.pstruct->pathStatus = Status::openlist;

    //step 3
    while(!openList.empty()){
        //3.1 +3.2
        currentNode = openList.back(); //get the best node form open List
        if (currentNode.pstruct->tile->getXPos() == _xend && currentNode.pstruct->tile->getYPos()== _yend){ // the current node is the solution node
            break;
        }

        // 3.3

        std::shared_ptr<Node> parent_ptr = std::make_shared<Node>(currentNode);
        for(int i= 0; i<=2;i++){ // ga alle posities rond current af
            for(int j=0; j <= 2; j++){
                if(i==1 && j==1) continue; // no need to do the current node all over again.
                int x = currentNode.pstruct->tile->getXPos() +i-1;
                int y = currentNode.pstruct->tile->getYPos() +j - 1;

                if(_matrix->contains(y,x)){ //positie in range of matrix
                    Node nearbyNode;
                    nearbyNode.pstruct = _matrix->get(y,x);
                    nearbyNode.x = x;nearbyNode.y = y;

                    if(!std::isinf(nearbyNode.pstruct->tile->getValue())){ // if passable


                        // bool found = std::find(openList.begin(), openList.end(), nearbyNode) != openList.end();
                        // if true, dan al in open lijst
                        // if(!found) found = std::find(closedList.begin(), closedList.end(), nearbyNode) != closedList.end();// check in closedList



                        nearbyNode.parent= parent_ptr; // set parent pointer to current node
                        nearbyNode.givenCost = currentNode.givenCost + nearbyNode.pstruct->tile->getValue();
                        nearbyNode.finalCost = nearbyNode.givenCost + calcHeuristicScore(x,y);
                         if( nearbyNode.pstruct->pathStatus == Status::none){ //  if not already in Lists
                             openList.push_front(nearbyNode); //add to open list
                             nearbyNode.pstruct->pathStatus = Status::openlist;
                         }
                        else if(nearbyNode.pstruct->pathStatus == Status::openlist){ // er is een oude node in openList
                            std::deque<Node>::iterator it = std::find(openList.begin(),openList.end(), nearbyNode);
                            if( (*it).givenCost > nearbyNode.pstruct->tile->getValue() + currentNode.givenCost ){ // old node is slechter

                                openList.erase(it);
                                openList.push_front(nearbyNode); //add to open list
                                nearbyNode.pstruct->pathStatus = Status::openlist;
                            }
                        }

                        else if(nearbyNode.pstruct->pathStatus == Status::closedlist){
                            std::deque<Node>::iterator it = std::find(closedList.begin(),closedList.end(), nearbyNode);
                            if( (*it).givenCost > nearbyNode.pstruct->tile->getValue() + currentNode.givenCost ){ // old node is slechter
                                closedList.erase(it); //remove old node
                                openList.push_front(nearbyNode); //add to open list
                                nearbyNode.pstruct->pathStatus = Status::openlist;
                            } //else old node is beter, keep old node, don't add new node
                        }



                    }
                }
            }

        }


        openList.pop_back();//delete currentNode from open
        closedList.push_back(currentNode); //add current node to closed
        currentNode.pstruct->pathStatus = Status::closedlist;

        std::sort(openList.begin(),openList.end(),CompareNode); //sort the open list on final cost
    }

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

float PathFinder::calcHeuristicScore(int x, int y) // distance to end point
{
    return sqrtf(pow(x-_xend,2) + pow(y-_yend,2));
}


