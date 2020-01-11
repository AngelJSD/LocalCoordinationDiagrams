#include <iostream>
#include <vector>

#include "Astar.hpp"


using namespace std;

int main(){

    vector<int> dimSizes;
    vector<cCell> path;
    dimSizes.push_back(500);
    dimSizes.push_back(500);

    map<int, vector< pair<cCell, int> > > pathsMap;
    cCell auxCell;
    vector< pair<cCell, int> > auxVect;
    auxVect.push_back(make_pair(auxCell, 2));
    pathsMap[0] = auxVect;
    pathsMap[1] = auxVect;

    //cGrid myGrid = cGrid(dimSizes);
    //Astar myPlanner = Astar(&myGrid);
    Astar myPlanner = Astar(pathsMap, dimSizes);
    vector<int> posIni;
    vector<int> posFin;
    posIni.push_back(0);
    posIni.push_back(0);
    posFin.push_back(499);
    posFin.push_back(499);
    /*cCell ini = *myGrid.getCell(posIni);
    cCell fin = *myGrid.getCell(posFin);*/
    cCell ini, fin;
    ini.pos = posIni;
    fin.pos = posFin;

    myPlanner.newSearch(ini, fin);
    path = myPlanner.newReconstructionMap(ini, fin);

    for(int i=0; i<path.size(); ++i){
        for(int j = 0; j < path[i].pos.size(); ++j)
        {
            cout<<path[i].pos[j]<<", ";
        }
        cout<<endl;
    }

    return 0;

}
