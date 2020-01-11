//To compile: g++ testCloud.cpp  -o testCloud.out -std=c++11

#include <iostream>
#include "grid.hpp"
#include "cloud.hpp"
#include "common.hpp"

using namespace std;


int main(){
	cout<<"Test of Cloud.cpp \n";
	cGrid myGrid = cGrid(INPUT_GRID_HIGH,INPUT_GRID_WIDH);

	cCloud myCloud = cCloud(myGrid);
	myCloud.printOutputMatrix();
	
	
	myCloud.generateAndSavePaths();
	cout<<"\n-------------------------\n";
	myCloud.printOutputMatrix();

	return 0;
}