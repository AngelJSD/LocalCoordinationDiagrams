#include "grid.hpp"
#include "Astar.hpp"

#define GRID_HIGH 5
#define GRID_WIDH 5

using namespace std;

int main(){

	cGrid myGrid = cGrid(GRID_HIGH,GRID_WIDH);

    // Set the obstacles
    for (int i=0; i<GRID_WIDH; ++i){
        myGrid.getCell(i,i)->setValueH(10);
        myGrid.getCell(i,i)->setValueG(1.4);
        myGrid.getCell(i,i)->setCost(15);
        myGrid.getCell(i,i)->setState("CLOSED");
    }
    myGrid.setObtacleAt(2,1);

    // Pass the grid to your planner
	Astar Astar1(myGrid);
    cCell start(0,2);
    cCell goal(4,2);
 	map<cCell, double> costSoFar;
 	Astar1.AstarSerch(start,goal, costSoFar);
 	
    // Return the path generated from 'a' to 'b' as a vector<cCell>
	vector<cCell> path = Astar1.reconstructionMap(start, goal);
	
    // Use de function myGrid.printPath(path) to print the path
	myGrid.printPath(path);

    return 0;
    
}
