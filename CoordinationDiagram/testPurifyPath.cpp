//To compile: 
//g++ testPurifyPath.cpp  -o testPP.out -std=c++11

#include <iostream>
#include <vector>
#include <map> 
#include <utility>      // std::pair, std::make_pair

#include "coordination_diagram.hpp"

using namespace std;

int main(){
	cout<<"TEST de coordination_diagram\n";
	vector<cCell> p1;
	
	cCell state;

	state.pos.push_back(0);
	state.pos.push_back(0);
	//state.pos.push_back(0);
	p1.push_back(state);

	state.pos.clear();
	state.pos.push_back(1);
	state.pos.push_back(0);
	//state.pos.push_back(0);
	p1.push_back(state);

	state.pos.clear();
	state.pos.push_back(1);
	state.pos.push_back(1);
	//state.pos.push_back(0);
	p1.push_back(state);

	state.pos.clear();
	state.pos.push_back(2);
	state.pos.push_back(1);
	//state.pos.push_back(0);
	p1.push_back(state);

	state.pos.clear();
	state.pos.push_back(2);
	state.pos.push_back(2);
	//state.pos.push_back(0);
	p1.push_back(state);

	state.pos.clear();
	state.pos.push_back(3);
	state.pos.push_back(2);
	//state.pos.push_back(0);
	p1.push_back(state);

	state.pos.clear();
	state.pos.push_back(3);
	state.pos.push_back(2);
	//state.pos.push_back(1);
	//p1.push_back(state);

	state.pos.clear();
	state.pos.push_back(3);
	state.pos.push_back(3);
	//state.pos.push_back(1);
	p1.push_back(state);

	state.pos.clear();
	state.pos.push_back(3);
	state.pos.push_back(4);
	//state.pos.push_back(1);
	p1.push_back(state);

	state.pos.clear();
	state.pos.push_back(4);
	state.pos.push_back(4);
	//state.pos.push_back(1);
	p1.push_back(state);

	state.pos.clear();
	state.pos.push_back(5);
	state.pos.push_back(4);
	//state.pos.push_back(1);
	p1.push_back(state);

	state.pos.clear();
	state.pos.push_back(6);
	state.pos.push_back(4);
	//state.pos.push_back(1);
	p1.push_back(state);

	state.pos.clear();
	state.pos.push_back(7);
	state.pos.push_back(4);
	//state.pos.push_back(1);
	p1.push_back(state);

	state.pos.clear();
	state.pos.push_back(7);
	state.pos.push_back(5);
	//state.pos.push_back(1);
	p1.push_back(state);

	vector < vector <int> > delays;
	int numberOfDims = p1[0].pos.size();
	delays.resize(numberOfDims);

	for (int i=0; i<numberOfDims; ++i){
		delays[i].resize(p1[p1.size()-1].pos[i]+1);
	}

	cout<<"DELAYS: "<<endl;
	for (int i=0; i<numberOfDims; ++i){
		for (int j=0; j<delays[i].size(); ++j){
			cout<<delays[i][j]<<", ";
		}
		cout<<endl;
	}

	
	cout<<"antes del purify:\n";
	for(int i = 0; i< p1.size();i++){
		cout<<"\npos "<<i<<" ( ";
		for(int j = 0 ; j < p1[i].pos.size();j++){
			cout<<p1[i].pos[j]<<" , ";
		}
		cout<<")";
	}

	cCoordinationDiagram coordination = cCoordinationDiagram();
	p1 = coordination.purifyPath(p1, delays);
	cout<<"\ndespues del purify:\n";
	for(int i = 0; i< p1.size();i++){
		cout<<"\npos "<<i<<" ( ";
		for(int j = 0 ; j < p1[i].pos.size();j++){
			cout<<p1[i].pos[j]<<" , ";
		}
		cout<<")";
	}

	cout<<"\nDELAYS: "<<endl;
	for (int i=0; i<numberOfDims; ++i){
		for (int j=0; j<delays[i].size(); ++j){
			cout<<delays[i][j]<<", ";
		}
		cout<<endl;
	}
	cout<<"\nTEST SUCCEDED"<<endl;

	return 0;
}