#ifndef ND_GRILLA_H
#define ND_GRILLA_H

#include <stdio.h>
#include <limits>
#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>

using namespace std;

struct cCell {

	cCell* father = nullptr;

	float h, g;
	float cost = 0;
	//int i,j, k;
	string state = "UNKNOWN";
	vector <cCell*> neightbours;
	vector <cCell> newNeightbours;
	vector<int> pos;

	bool obstacle = false;
	bool pgoal = false;
	/*bool operator==(const cCell& p) const
	{
		return i == p.i && j == p.j;
	}*/
};

bool operator ==(cCell a, cCell b ){
	
	for(int i=0; i<a.pos.size(); ++i){
		if(a.pos[i] != b.pos[i]){
			return false;
		}
	}
	return true; 
}

bool operator <(cCell a, cCell b){
	return tie(a.pos) < tie(b.pos);}

class cGrid {

	public:
		//vector< vector< vector<cCell> > > grid;
		vector<cCell> grid;
		vector<int> dimSizes;
		int numDims;
		int h, w, d;
		double maxVelocity;

		uint numCells = 1;

		cGrid (){};
		cGrid (vector <int> dimSizes){

			/*uint imax = std::numeric_limits<uint>::max();
			cout<<"MaxInt: "<<imax<<endl;*/
			this->dimSizes = dimSizes;
			this->numDims = this->dimSizes.size();
			for (int i=0; i<this->numDims; ++i)
				this->numCells *= (uint)this->dimSizes[i];
			
			//cout<<"NumCells: "<<this->numCells<<endl;
			
			//cout<<"Todavia vivo 1";
			grid.resize(this->numCells);
			//cout<<"Todavia vivo 2";
			this->maxVelocity = 10;
			
			init();
		}

		void init(){
			
			/*
			index = x + y * D1 + z * D1 * D2 + t * D1 * D2 * D3;
			x = Index % D1;
			y = ( ( Index - x ) / D1 ) %  D2;
			z = ( ( Index - y * D1 - x ) / (D1 * D2) ) % D3; 
			t = ( ( Index - z * D2 * D1 - y * D1 - x ) / (D1 * D2 * D3) ) % D4; 	
			*/	
			vector<int> auxDimSizes = dimSizes;
			reverse(auxDimSizes.begin(), auxDimSizes.end());
				
			for(int i=0; i<numCells; ++i){
				
				//Grid[i].pos.push_back(i%dimSizes[0]);
				
				for(int j=0; j<numDims; ++j){
					
					int div = 1;
					for(int k=0; k<j; ++k){
						div *= auxDimSizes[k];			
					}

					int rest=0;
					for(int k=j-1; k>=0; --k){
						int mult=1;
						
						for(int l=0; l<k; ++l){
							mult *=	auxDimSizes[l];			
						}
						
						rest -= grid[i].pos[k]*mult;
					}
					//if(i==5) cout<<"Pos push: "<<((i+rest)/div)<<" MOD "<<auxDimSizes[j]<<endl;
					grid[i].pos.push_back(((i+rest)/div) % auxDimSizes[j]);
				}
				reverse(grid[i].pos.begin(), grid[i].pos.end());
				
			}

			for(int i=0; i<numCells; ++i){
				fillNeighbours(&grid[i], dimSizes);
				/*cout<<"id: "<<i<<" val: ";
				for(int k=0; k<grid[i].neightbours.size(); ++k){
					cout<<"(";
					for(int j=0; j<grid[i].neightbours[k]->pos.size(); ++j){
						cout<<grid[i].neightbours[k]->pos[j]<<",";
					}
					cout<<")";
				}
				cout<<endl;*/
			}	
			cout<<"acabe de generar la grilla\n";		
		}

		void setObstacle(vector<int> pos){

			getCell(pos)->obstacle=true;
		}

		double getVelocity(cCell c){

			//cout<<"entro"<<endl;
			/*if (grid[c.i][c.j].vel==-1){
				
				return this->maxVelocity;
				
			}
			return grid[c.i][c.j].vel;*/
		}

		cCell* getCell( vector<int> pos){
			
			//cout<<"OJO: "<<grid[98].cost<<endl;
			/*
			You can create an n-dimensional array of dimensions D1, D2, D3, ..., Dn by creating 
			a one dimensional array of size (D1 * D2 * D3 * ... * Dn). Then you access an element
			at position (x1, x2, x3, ... ,xn) like this - array[x1 * (D2 * D3 * ... Dn) + 
			x2 (D3 * D4 * ... Dn) + ... + xn].
			*/
			if(pos.size()==numDims){
				
				int allocator = 0;
				for (int i=0; i<numDims; ++i){
					
					int mult = 1;
					for (int j=i+1; j<numDims; ++j){
						mult *= dimSizes[j];
					}

					allocator += pos[i]*mult;
				}
				//cout<<"A: "<<allocator;//<<" "<<grid[allocator].cost<<endl;
				//if(pos[0]==1 && pos[1]==0) cout<<"Allocator: "<<allocator<<endl;
				return &grid[allocator];
			}
			
			return nullptr;
		}

		void fillNeighbours(cCell *c, vector<int> dimSizes){
	
			for(int i=0; i<numDims; ++i){

				if(c->pos[i]-1>=0){
					vector<int> nePos = c->pos;
					nePos[i] -= 1;
					c->neightbours.push_back(getCell(nePos));
					//if(c->pos[0]==0 && c->pos[1]==0) cout<<"Vecino: ";//<<getCell(nePos)->pos[0]<<","<<getCell(nePos)->pos[1]<<endl;
				}

				if(c->pos[i]+1<dimSizes[i]){
					vector<int> nePos = c->pos;
					nePos[i] += 1;
					c->neightbours.push_back(getCell(nePos));
					//if(c->pos[0]==0 && c->pos[1]==0) cout<<"Vecino: "<<nePos[i];//<<getCell(nePos)->pos[0]<<","<<getCell(nePos)->pos[1]<<endl;
				}
			}
		}

		double euclideanDistance(cCell* a, cCell* b){
			
			float dist = 0;
			//cout<<b->pos.size();
			for(int i=0; i<a->pos.size(); ++i){
				dist += pow(a->pos[i]-b->pos[i], 2);
			}
            return sqrt(dist);
        }


		void print(){

			/*for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					for (int k = 0; k < d; k++) {
						printf("(%.1f,%.4f)", grid[i][j][k].vel, grid[i][j][k].t);
						//cout<<grid[i][j].obstacle;
					}
				}
				printf("\n");
			}*/
			for(int i=0; i<numCells; ++i){
				cout<<"(id: "<<i<<" val: "<<grid[i].obstacle<<"),";
			}

		}
};

#endif