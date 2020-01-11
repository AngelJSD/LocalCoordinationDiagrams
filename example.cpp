#include "grid.hpp"
#include "Astar.hpp"
#define GRID_HIGH 5
#define GRID_WIDH 5

using namespace std;

int main(){

    cGrid myGrid = cGrid(GRID_HIGH,GRID_WIDH);
	
    for (int i=0; i<GRID_WIDH; ++i){
        myGrid.getCell(i,i)->setValueH(10);
        myGrid.getCell(i,i)->setValueG(1.4);
        myGrid.getCell(i,i)->setCost(15);
        myGrid.getCell(i,i)->setState("CLOSED");
    }

    myGrid.setObtacleAt(2,1);
    myGrid.printValues();
    myGrid.printStates();
    myGrid.getCell(0,2)->printNeighbours();
    vector<cCell*> aux_n = myGrid.getCell(2,2)->getNeightbours();
    
    Astar Astar1(myGrid);
    cCell start(0,2);
    cCell goal(4,2);
    
 	map<cCell, double> costSoFar;
 	Astar1.AstarSerch(start,goal, costSoFar);
 	
 	vector<cCell> path = Astar1.reconstructionMap(start, goal);
    
    cout<<"\nOK************************\n";
    
    /*cout<<"Euclidean distances from (2,2) to each neighbour:"<<endl;
    for(int i=0; i<aux_n.size(); ++i){
        cout<<myGrid.euclideanDistance(myGrid.getCell(2,2),aux_n[i])<<endl;
    }

    cout<<"Manhattan distances from (2,2) to each neighbour:"<<endl;
    for(int i=0; i<aux_n.size(); ++i){
        cout<<myGrid.manhattanDistance(myGrid.getCell(2,2),aux_n[i])<<endl;
    }*/

    /*cCell p1(0,2);
    path.push_back(p1);
    cCell p2(1,2);
    path.push_back(p2);
    cCell p3(2,2);
    path.push_back(p3);
    cCell p4(3,3);
    path.push_back(p4);
*/
    cout<<"-----------------------------------------------------------\n";
    myGrid.printPath(path);
    return 0;
    
}
