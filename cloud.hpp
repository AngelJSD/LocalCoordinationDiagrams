
#include "grid.hpp"
#include "common.hpp"

class cCloud
{
	public:
	cGrid inputGrid;
	vector< vector< vector<cCell> > > savedPaths;
	int h,w; //dimensions of savedPaths

	cCloud(cGrid inputGrid){
		this->inputGrid = inputGrid;
		this->h = inputGrid.getHeight()/EDGE_BLOCK;
		this->w = inputGrid.getWidth()/EDGE_BLOCK;
		savedPaths.resize(h);
		for (int i = 0; i < h; i++) {
			savedPaths[i].resize(w);
		}
	}

	//general functions
	void savePath(int i, int j, vector<cCell> path);
	void printVectorPath(vector<cCell> path);
	void printOutputMatrix();

	//algorithm functins
	void divideGrid();
	void distributeBlocks();
	void generateAndSavePaths();	
};


void cCloud :: savePath(int i, int j, vector<cCell> path){
	savedPaths[i][j] = path;
}

void cCloud :: printVectorPath(vector<cCell> path){
	cout <<"{";
	for (int i = 0; i < path.size(); i++) {
		cout<< path.at(i) << ' ';
	}	
	cout <<'}';	
}

void cCloud :: printOutputMatrix(){
	cout<<"\nsavedPaths:\n";
    for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
				printVectorPath(savedPaths[i][j]);
				cout<<" ";
		}
		cout<<'\n';
	}
    cout<<'\n';
}

void cCloud :: divideGrid(){}

void cCloud :: generateAndSavePaths(){
	//TODO: 
	vector<cCell> pathExample;
	for (int i = 0; i < 3; i++) {
        cCell cell = cCell(i,0,0);
        cell.setState("UNKNOWN");
        pathExample.push_back(cell);
	}
	savePath(2,1,pathExample);
	savePath(1,2,pathExample);
}
