//To compile: g++ testCD.cpp  -o testCD.out -std=c++11

#include <iostream>
#include <vector>
#include <map> 
#include <utility>      // std::pair, std::make_pair
#include "Astar.hpp"
#include "nd_grilla.hpp"
#include "coordination_diagram.hpp"
#include "common.hpp"
#include <fstream>
#include <sstream>

using namespace std;

vector<vector<cCell> > importPaths(int maxPath,string extension){
    vector< vector<cCell> > paths;  
    for (int i = 0; i< maxPath;i++ ){
        string namefile = "gPath";
        string str_i = to_string(i);
        namefile+=str_i;
        namefile+=extension;
        cout<<namefile<<" leido"<<endl;
        ifstream file;
        vector<cCell> p1;
        file.open((namefile));
        string frase, token;
        if (file.fail())
        {
            cout << "Error al abrir el " << namefile << endl;
            break;
        }
        else
        {
            while (!file.eof())
            {
                getline(file, frase);
                istringstream iss(frase);
                int i = 0;
                int x,y;
                cCell cel1;
                while (getline(iss, token, ',') && i < 2)
                {
                    string tok(token.c_str());
                    cel1.pos.push_back(stoi(tok));
                    i++;
                }
                if(cel1.pos.size()>0){
                    p1.push_back(cel1);    
                }                
            }
            file.close();
            paths.push_back(p1);
            p1.clear();
        } 
    }
    return paths;
}


void exportDelays(vector<vector<int> > paths, string head, string extension){
    int i = 0;
    for (std::vector<vector<int>>::iterator it = paths.begin() ; it != paths.end(); ++it){
        string namefile = head;
        string str_i = to_string(i);
        namefile+=str_i;
        namefile+=extension;
        cout<<namefile<<" generado: con "<<(*it).size()<<"celdas"<<endl;
        ofstream file;
        file.open (namefile);
        for(int j = 0 ; j < (*it).size();j++ ){
            file<<paths[i][j];
            if(j != (*it).size()-1){
            	file<<endl;
            }
        }
        file.close();
        i++;
    }
}

int main(){
	cout<<"TEST de coordination_diagram\n";
	

	vector< vector<cCell> > paths;
	paths = importPaths(NUMBER_OF_PATHS,".csv");


	vector<int> dimSizes;

	for(int i=0; i<paths.size(); ++i){
		dimSizes.push_back(paths[i].size());
	}

	vector< vector< int > > finalDelaysPerPath (dimSizes.size());
    for (int j=0; j<dimSizes.size(); ++j) {
        finalDelaysPerPath[j].resize(dimSizes[j]);
    }

	cCoordinationDiagram coordination = cCoordinationDiagram();
	clock_t begin = clock();
	map <cCell, vector < pair<int,int> > > myCollisions = coordination.detectCollisions(paths);
	map <cCell, vector < pair<int,int> > >::iterator it;
	cout<<"colisiones detectadas: "<<myCollisions.size()<<"\n";
	for ( it = myCollisions.begin(); it != myCollisions.end(); it++ ){
		std::cout <<"("<<it->first.pos[0]<<","<<it->first.pos[1]<<"): ";
	}
	cout<<"\n--------------- Clasificando zonas -------------------\n";
	vector<pair<vector<int>, map< cCell, vector < pair < int, int > > > > > collisionsClassified;
	collisionsClassified = coordination.classifyCollisions(myCollisions);
	cout<<collisionsClassified.size()<<endl;
	cout<<"--------- end clasificando zonas -------------------\n";
	cout<<"\n------------ Iterando grupos -------------------\n";
	for(int i=0; i<collisionsClassified.size(); ++i){
		///cout<<"\nGRUPO "<<i+1<<":\n";
		map <cCell, vector < pair<int,int> > >::iterator it;
		for ( it = collisionsClassified[i].second.begin(); it != collisionsClassified[i].second.end(); it++ ){
	
			///std::cout <<"("<<it->first.pos[0]<<","<<it->first.pos[1]<<"): ";
			for(int i=0; i<it->second.size(); ++i){
			///	cout<<"(id: "<<it->second[i].first<<", pos:"<<it->second[i].second<<"), ";
			}
			///cout<<endl;
		}
		cout<<"--------------- Ordenando grupo -------------------\n";
		vector < pair<cCell, vector < pair<int,int> > > > sortedCollisions = coordination.sortCollisions(collisionsClassified[i].second, collisionsClassified[i].first);
		cout<<"-------------- end ordenando grupo -------------------\n";
		cout<<"--------------- Resolviendo grupo -------------------\n";
		if (sortedCollisions.size() > 0) {
			coordination.solveSortedCollisions(sortedCollisions, dimSizes, finalDelaysPerPath);
		} else {
			cout<<"!!!!!!!!!!!!!!!! Ciclo detectado !!!!!!!!!!!!!!!!!!!\n";
			vector<int> auxDimSizes;
			for(int pathsIdsIt = 0; pathsIdsIt < collisionsClassified[i].first.size(); ++pathsIdsIt) {
				auxDimSizes.push_back(dimSizes[collisionsClassified[i].first[pathsIdsIt]]);
			}
			cout<<"--------------- Aplicando CD clasico -------------------\n";
			vector<cCell> coordPath = coordination.calculateVelocityPath(auxDimSizes, collisionsClassified[i].second);
			cout<<"\n------------ End aplicando CD clasico -----------------\n";
		}
		cout<<"-------------- End resolviendo grupo -------------------\n";
	}
	cout<<"\n--------- End iterando grupos -------------------\n";
	clock_t end = clock();
	cout<<"##### FINAL DELAYS PER PATH: ######"<<endl;
    for (int i=0; i<finalDelaysPerPath.size(); ++i) {
        for (int j=0; j<finalDelaysPerPath[i].size(); ++j) {
            cout<<finalDelaysPerPath[i][j]<<",";
        }
        cout<<endl;
    }
	
	exportDelays(finalDelaysPerPath,"local_planning", ".csv");

	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout<<"TIEMPO: "<<elapsed_secs<<endl;
	
	vector< vector< int > > ClassicCD_finalDelaysPerPath (dimSizes.size());
    for (int j=0; j<dimSizes.size(); ++j) {
        ClassicCD_finalDelaysPerPath[j].resize(dimSizes[j]);
    }
    /*
	cout<<"\n<<<<<<<<<< Executing clasic CD >>>>>>>>>>>>>\n";
	begin = clock();
	vector<cCell> coordPath = coordination.calculateVelocityPath(dimSizes, myCollisions);
	end = clock();
	cout<<"\n<<<<<<<< End executing clasic CD >>>>>>>>>>>>>\n";
	*/
	/*for (int i = 0; i < coordPath.size(); ++i) {
		cout<<coordPath[i].pos[0]<<", "<<coordPath[i].pos[1]<<", "<<coordPath[i].pos[2]<<endl;
	}*/
	/*
	coordPath = coordination.purifyPath(coordPath, ClassicCD_finalDelaysPerPath);
	for (int i = 0; i < coordPath.size(); ++i) {
		cout<<coordPath[i].pos[0]<<", "<<coordPath[i].pos[1]<<", "<<coordPath[i].pos[2]<<endl;
	}
	cout<<"\n##### FINAL DELAYS PER PATH: ######"<<endl;
    for (int i=0; i<ClassicCD_finalDelaysPerPath.size(); ++i) {
        for (int j=0; j<ClassicCD_finalDelaysPerPath[i].size(); ++j) {
            cout<<ClassicCD_finalDelaysPerPath[i][j]<<",";
        }
        cout<<endl;
    }
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout<<"TIEMPO: "<<elapsed_secs<<endl;
    */
	/*cout<<"Coordination Path:"<<endl;
	for(int i = 0; i < coordPath.size(); i++)
	{
		cout<<coordPath[i].pos[0]<<" "<<coordPath[i].pos[1]<<" "<<coordPath[i].pos[2]<<" "<<coordPath[i].pos[3]<<endl;
	}*/
	//exportDelays(ClassicCD_finalDelaysPerPath,"classic_planning", ".csv");
	cout<<"ACABO"<<endl;
	return 0;
}