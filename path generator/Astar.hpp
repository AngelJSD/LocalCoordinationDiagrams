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
#include "grid.hpp"

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

	
class Astar
{
	public:
	
	cGrid gridMap;
	Astar(cGrid copyMap){
		loadMap(copyMap);
	}

	void loadMap(cGrid copyMap){ 		
		int h=copyMap.h;
		int w=copyMap.w;
		gridMap= cGrid(h,w);
		for(int i=0;i<h;i++){
			for(int j=0;j<w;j++){
				gridMap.getCell(i,j)->setValueH(copyMap.getCell(i,j)->getValueH());
				gridMap.getCell(i,j)->setValueG(copyMap.getCell(i,j)->getValueG());
				gridMap.getCell(i,j)->setCost(copyMap.getCell(i,j)->getCost());
				gridMap.getCell(i,j)->setState(copyMap.getCell(i,j)->getState());	
			}
		}				
	} 
	
	vector<cCell> reconstructionMap( cCell start, cCell goal) {
	  vector<cCell> path;
	  cCell current = goal;
	  while (current != start) {
		path.push_back(current);
		current = *(gridMap.getCell(current.i,current.j)->getFather());
	  }
	  path.push_back(start); 
	  reverse(path.begin(), path.end());
	  return path;
	}
	

	void AstarSerch(cCell start,cCell goal, map<cCell, double>& costSoFar)
	{
		priorityQ<cCell, double> frontier;
		frontier.put(start, 0);
		gridMap.getCell(start.i,start.j)->setFather(gridMap.getCell(start.i,start.j)->getFather());	
		costSoFar[start] = 0;
		while (!frontier.empt()) {
			cCell current = frontier.get();
			if (current == goal) { break;}
			for (cCell *next : gridMap.getCell(current.i,current.j)->getNeightbours()) {
			  double newCost = costSoFar[current] + current.getCost();//grafo.costo(current, next);
			  if (costSoFar.find(*next) == costSoFar.end() || newCost < costSoFar[*next]) {
				costSoFar[*next] = newCost;
			   	double priority = newCost + gridMap.heuristic(*next, goal);
				frontier.put(*next, priority);
				gridMap.getCell(next->i,next->j)->setFather(gridMap.getCell(current.i,current.j));
			  }
			}
		}
	}
};

#endif
