//To compile: g++ testCreatePath.cpp  -o testCreatePath.out -std=c++11

#include <iostream>
#include <vector>
#include <map> 
#include <utility>      // std::pair, std::make_pair
#include "Astar.hpp"
#include "nd_grilla.hpp"
#include "coordination_diagram.hpp"
#include "createPath.hpp"
#include "common.hpp"
#include <fstream>

#define GRILLA_REF_X SIZE_EDGE
#define GRILLA_REF_Y SIZE_EDGE
#define GRILLA_REF_Z 



using namespace std;

int main(){
	cout<<"TEST de createPath\n";
	vector <int> dimSizes;
	dimSizes.push_back(GRILLA_REF_X);
	dimSizes.push_back(GRILLA_REF_Y);
	cGrid mGrid;
	mGrid = cGrid (dimSizes);
	if(OBSTACLES){
		fillGrid(OBSTACLES_FILE_NAME,&mGrid);	
	}	

	vector<vector<cCell> > paths = createPath (NUMBER_OF_PATHS,mGrid);
	/*
	for(int i=0;i<NUMBER_OF_PATHS;i++){

		cout<<"camino "<<i<<" :\n";
		for(int j=0;j<paths[i].size();j++){
			cout<<paths[i][j].pos[0]<<"--"<<paths[i][j].pos[1]<<"  ";
		}
		cout<<" \n";
	}*/

	exportPaths(paths,".csv");
	///vector<vector<cCell> > paths1 = importPaths(NUMBER_OF_PATHS,".csv");
	/*for(int i = 0; i<paths1.size();i++){
		cout<<"camino "<<i<<":: con"<<paths1[i].size()<<"celdas";
		for (int j = 0; j < paths1[i].size();j++)
    	{
        	cout<<"("<<paths1[i][j].pos[0]<<"-"<<paths1[i][j].pos[1]<<")";
    	}	
        	cout<<endl;
	}*/

	//exportCollisions(paths1);
}