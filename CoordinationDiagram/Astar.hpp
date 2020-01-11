#ifndef ASTAR_H
#define ASTAR_H

#include <iostream>
#include <iomanip>
#include <map>
#include <set>
#include <array>
#include <vector>
#include <utility>
#include <queue>
#include <tuple>
#include <algorithm>
#include <cstdlib>
#include<cmath>
#include "nd_grilla.hpp"

using namespace std;

template<typename T, typename priorityT>
	struct priorityQ {
	  typedef pair<priorityT, T> PQElement;
	  priority_queue<PQElement, vector<PQElement>,greater<PQElement>> elements;

	  bool empt() const {
		 return elements.empty();
	  }

	  void put(T object, priorityT priority) {
		elements.emplace(priority, object);
	  }

	  T get() {
		T bestObject = elements.top().second;
		elements.pop();
		return bestObject;
	  }
	};

class mycomparison
{
  bool reverse;
public:
  mycomparison(const bool& revparam=false)
    {reverse=revparam;}
  bool operator() (cCell& lhs, cCell& rhs) const
  {
    if (reverse) return (lhs.cost >rhs.cost);
    else return (lhs.cost >rhs.cost);
  }
};

struct Class1Compare
{
   bool operator() (const cCell lhs, const cCell rhs) const
   {
       return lhs.cost < rhs.cost;
   }
};
	
class Astar
{
	public:
	
	cGrid *gridMap;
	vector< vector<bool> > obsMap;
	vector<int> dimSizes;
	map <cCell, cCell> parentsMap;
	Astar(cGrid *copyMap){
		gridMap = copyMap;
		//cout<<gridMap->dimSizes[0]<<" "<<gridMap->dimSizes[1];
	} 

	Astar(map<int, vector< pair<cCell, int> > > pathsMap, vector<int> dimSizes /*global no aux*/){
		
		map<int, vector< pair<cCell, int> > >::iterator it1;
		int allocator = 0;
		for ( it1 = pathsMap.begin(); it1 != pathsMap.end(); it1++ ){
			vector<bool> obstacles;
			obstacles.resize(dimSizes[allocator]);
			for(int i=0; i<it1->second.size(); ++i){
				cout<<"Hay obs!"<<endl;
				obstacles[it1->second[i].second] = true;
			}
			this->obsMap.push_back(obstacles);
			++allocator;
		}
		this->dimSizes = dimSizes;
		//cout<<gridMap->dimSizes[0]<<" "<<gridMap->dimSizes[1];
	} 
	
	vector<cCell> reconstructionMap( cCell start, cCell goal) {
	  vector<cCell> path;
	  cCell current = goal;
	  while (!(current == start)) {
			path.push_back(current);
			//cout<<"c:"<<current.pos[0]<<","<<current.pos[1]<<endl;
			current = *(gridMap->getCell(current.pos)->father);
	  }
	  path.push_back(start);
	  //cout<<"start:"<<start.pos[0]<<","<<start.pos[1]<<endl; 
	  //reverse(path.begin(), path.end());
	  return path;
	}
	

	void AstarSerch(cCell start,cCell goal)
	{
		
		priority_queue<cCell,std::vector<cCell>,mycomparison> frontier;
		map<cCell, double> costSoFar;
		
		frontier.push(start);
		
		//gridMap->getCell(start.pos)->father=gridMap->getCell(start.pos)->father;	
		
		costSoFar[start] = 0;
		
		while (!frontier.empty()) {
			cCell current = frontier.top();
			frontier.pop();
			if (current == goal) { 
				//cout<<"ACABO"<<endl;
				break;}
			//cout<<"VIVO"<<endl;
			//cout<<gridMap->getCell(current.pos)->cost<<endl;
			for (cCell *next : gridMap->getCell(current.pos)->neightbours) {
			  if(!next->obstacle){
					double newCost = costSoFar[current] + gridMap->euclideanDistance(&current, next);//grafo.costo(current, next);
					//double newCost = gridMap->euclideanDistance(&current, next);
					//cout<<"VIVO1 "<<(costSoFar.find(*next)!= costSoFar.end())<<" "<<next->pos[0]<<" "<<next->pos[1]<<endl;
					if (costSoFar.find(*next)== costSoFar.end() || newCost < costSoFar[*next]) {
						costSoFar[*next] = newCost;
						double priority = newCost + gridMap->euclideanDistance(next, &goal);
						next->cost=priority;
						frontier.push(*next);
						gridMap->getCell(next->pos)->father=(gridMap->getCell(current.pos));
					}
				}
			}
		}
	}

	vector<cCell> newReconstructionMap( cCell start, cCell goal) {
	  vector<cCell> path;
	  cCell current = goal;
	  while (!(current == start)) {
			path.push_back(current);
			//cout<<"c:"<<current.pos[0]<<","<<current.pos[1]<<endl;
			current = parentsMap[current];
	  }
	  path.push_back(start);
	  //cout<<"start:"<<start.pos[0]<<","<<start.pos[1]<<endl; 
	  //reverse(path.begin(), path.end());
	  return path;
	}

	void newSearch(cCell start,cCell goal)
	{
		
		priority_queue<cCell,std::vector<cCell>,mycomparison> frontier;
		map<cCell, double> costSoFar;
		int nDims = start.pos.size();
		
		frontier.push(start);
		
		//gridMap->getCell(start.pos)->father=gridMap->getCell(start.pos)->father;	
		
		costSoFar[start] = 0;
		
		while (!frontier.empty()) {
			cCell current = frontier.top();
			frontier.pop();
			if (current == goal) { 
				//cout<<"ACABO"<<endl;
				break;}
			for (int i = 0; i < nDims; ++i) {
				cCell neightbour;
				for (int j = 0; j < 2; ++j) {
					neightbour.pos = current.pos;
					int newCoord = current.pos[i] + 1 - 2*j;
					if (newCoord >= 0 && newCoord < this->dimSizes[i]) {
						neightbour.pos[i] = newCoord;
						bool isObstacle = true;
						for (int k = 0; k < neightbour.pos.size(); ++k) {
							if (!this->obsMap[i][neightbour.pos[k]]){
								isObstacle = false;
								break;
							}
						}
						neightbour.obstacle = isObstacle;
						current.newNeightbours.push_back(neightbour);
					}
				}
			}
			//cout<<gridMap->getCell(current.pos)->cost<<endl;
			for (cCell next : current.newNeightbours) {
			  if(!next.obstacle){
					double newCost = costSoFar[current] + gridMap->euclideanDistance(&current, &next);//grafo.costo(current, next);
					//double newCost = gridMap->euclideanDistance(&current, next);
					//cout<<"VIVO1 "<<(costSoFar.find(*next)!= costSoFar.end())<<" "<<next->pos[0]<<" "<<next->pos[1]<<endl;
					if (costSoFar.find(next)== costSoFar.end() || newCost < costSoFar[next]) {
						costSoFar[next] = newCost;
						double priority = newCost + gridMap->euclideanDistance(&next, &goal);
						next.cost=priority;
						frontier.push(next);
						parentsMap[next] = current;
					}
				}
			}
		}
	}
};

#endif
