#ifndef GRID_H
#define GRID_H

#include <stdio.h>
#include <limits>
#include <iostream>
#include <vector>
#include <math.h>
#include <tuple>

using namespace std;

class cCell {

    private:

        float h, g, cost;
        string state;
        bool obstacle = false;
        cCell* father = nullptr;
    
    public:
        
        int i, j, k;
        vector <cCell*> neightbours;
        
        cCell(){};
        cCell(int i, int j, int k=0){
            this->i=i;
            this->j=j;
            this->k=k;
            h = g = cost = numeric_limits<double>::infinity();
        	cost=1;
        }

        void setValueH(float value){
            this->h = value;
        }

        void setValueG(float value){
            this->g = value;
        }

        void setCost(float value){
            this->cost = value;
        }

        void setState(string state){
            this->state = state;
        }

        void setFather(cCell* c){
            this->father = c;
        }

        void setObstacle(){
            this->obstacle = true;
        }

        float getValueG(){
            return g;
        }

        float getValueH(){
            return h;
        }

        float getCost(){
            return cost;
        }

        string getState(){
            return state;
        }

        cCell* getFather(){
            return father;
        }

        vector <cCell*> getNeightbours(){
            return neightbours;
        }        

        bool isObstacle(){
            return obstacle;
        }

        void printNeighbours(){

            cout<<"\nNeighbours of ("<<i<<","<<j<<"):\n";
            
            for(int i=0; i<neightbours.size(); ++i){
                cout<<"("<<neightbours[i]->i<<","<<neightbours[i]->j<<")\n";
            }
        }

};

bool operator ==(cCell a, cCell b ){
	return a.i==b.i && a.j==b.j; }
    
bool operator !=(cCell a, cCell b){
	return !(a==b);	}
		
bool operator <(cCell a, cCell b){
	return tie(a.i,a.j) < tie(b.i,b.j);}

basic_iostream<char>::basic_ostream& operator<<(basic_iostream<char>::basic_ostream& out, const cCell & loc) 
	{
	  out << '(' << loc.i << ',' << loc.j << ')';
	  return out;
	}
	
class cGrid {

	public:
		vector< vector< vector<cCell> > > grid;
		int h, w, d;
		
		cGrid (){};
		cGrid (int h, int w, int d = 1){

			this->h = h;
			this->w = w;
			this->d = d;
			
			init();
		}

		void init(){

			grid.resize(h);

			for (int i = 0; i < h; i++) {

				grid[i].resize(w);
                
				for (int j = 0; j < w; j++) {

                    //cout<<"entro "<<i<<" "<<j<<endl;
					grid[i][j].resize(d);
					for (int k = 0; k < d; k++) {
						
                        cCell cell = cCell(i,j,k);
                        cell.setState("UNKNOWN");
						grid[i][j][k] = cell;
					}
				}
			}
            
			cout<<"Filling neighbours..."<<endl;
			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					for (int k = 0; k < d; k++) {
						if (!grid[i][j][k].isObstacle()) 
							fillNeightbours(&grid[i][j][k]);
					}
				}
			}

            cout<<"Grid construction complete!"<<endl;
		}

		void fillNeightbours(cCell *c){

			c->neightbours.clear();
			
			if(c->i-1>=0) c->neightbours.push_back(&grid[c->i-1][c->j][c->k]);

			if(c->i+1<h) c->neightbours.push_back(&grid[c->i+1][c->j][c->k]);

			if(c->j-1>=0) c->neightbours.push_back(&grid[c->i][c->j-1][c->k]);

			if(c->j+1<w) c->neightbours.push_back(&grid[c->i][c->j+1][c->k]);

			if(c->k-1>=0) c->neightbours.push_back(&grid[c->i][c->j][c->k-1]);

			if(c->k+1<d) c->neightbours.push_back(&grid[c->i][c->j][c->k+1]);

            // 2D diagonals
            
            if(c->i-1>=0 && c->j-1>=0) c->neightbours.push_back(&grid[c->i-1][c->j-1][c->k]);

            if(c->i-1>=0 && c->j+1<w) c->neightbours.push_back(&grid[c->i-1][c->j+1][c->k]);

            if(c->i+1<h && c->j-1>=0) c->neightbours.push_back(&grid[c->i+1][c->j-1][c->k]);

            if(c->i+1<h && c->j+1<w) c->neightbours.push_back(&grid[c->i+1][c->j+1][c->k]);

            // TODO: Add 3D diagonals

		}

        double euclideanDistance(cCell* a, cCell* b){

            return sqrt(pow(a->i-b->i,2)+pow(a->j-b->j,2)+pow(a->k-b->k,2));
        }

        double manhattanDistance(cCell* a, cCell* b){

            return abs(a->i-b->i)+abs(a->j-b->j)+abs(a->k-b->k);
        }

        void setObtacleAt(int i, int j, int k=0){

            getCell(i, j, k)->setObstacle();
        }

		double heuristic(cCell a, cCell b) { 
			double dx = abs(a.i - b.i);
			double dy = abs(a.j - b.j);
			double D=1;
			double D2=sqrt(2);
			return D * (dx + dy) + (D2 - 2 * D) * min(dx, dy);
		}
	
	
        cCell* getCell(int i, int j, int k=0){
            return &grid[i][j][k];
        }

        int getHeight(){
            return h;
        }

        int getWidth(){
            return w;
        }

        int getDepth(){
            return d;
        }

		void printValues(){

			cout<<"\nValues:\n";
            for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					for (int k = 0; k < d; k++) {
						printf("|h: %.2f g: %.2f c: %.2f|", grid[i][j][k].getValueH(), grid[i][j][k].getValueG(), grid[i][j][k].getCost());
					}
				}
				printf("\n");
			}
            cout<<"\n";
		}

        void printStates(){

			cout<<"\nStates:\n";
            for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					for (int k = 0; k < d; k++) {
                        cout<<"|"<<grid[i][j][k].getState()<<"|";
					}
				}
				printf("\n");
			}
            cout<<"\n";
		}

        void printPath(vector<cCell> path){

            vector<vector<char> > output;
            output.resize(h);
            
            for (int i = 0; i < h; i++) {
                output[i].resize(w);
				for (int j = 0; j < w; j++) {
					if(grid[i][j][0].isObstacle()){
                        output[i][j] = '1';
                    }
                    else{
                        output[i][j] = '0';
                    }
				}
			}

            for (int i=0; i<path.size(); ++i){
                output[path[i].i][path[i].j] = '*';
            }

            cout<<"\nPath (*):\n";
            for (int i = 0; i < h; i++) {
                
				for (int j = 0; j < w; j++) {
                    cout<<output[i][j];
                }
                cout<<endl;
            }
            cout<<"\n";
        }
};

#endif
