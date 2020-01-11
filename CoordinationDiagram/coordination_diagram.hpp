#include <iostream>
#include <vector>
#include <map> 
#include <utility>      // std::pair, std::make_pair
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>
#include "nd_grilla.hpp"
#include "Astar.hpp"

//Class Definitions
typedef  vector < cCell > cPath;
typedef  map< cCell, vector < pair < int, int > > > cCollision;
typedef  pair < vector < int >, cCollision > cCollision_zone;

class cCoordinationDiagram{
    
    private:
        vector<cPath > mPaths;
        map<int, vector< pair<cCell, int> > > mPathsMap;
        vector<int> mDimSizes;
        cGrid mGrid;
    
    public:
        cCoordinationDiagram();
        void generateGrid(vector< cPath >);
        cCollision detectCollisions(vector< cPath >);
        vector < cCollision_zone > classifyCollisions(cCollision);
        void fillObstacles(cCollision);
        cPath getVelocityPlanning();
        cPath calculateVelocityPath(vector<int> dimSizes, cCollision );
        vector < pair<cCell, vector < pair<int,int> > > > sortCollisions(cCollision, vector <int>);
        cPath solveSortedCollisions(vector < pair<cCell, vector < pair<int,int> > > >, vector<int> dimSizes, vector< vector< int > >&);
        cPath purifyPath(cPath, vector < vector <int> > &);
        static bool mySortFunction(pair<cCell, int> a, pair<cCell, int> b);

};

cCoordinationDiagram::cCoordinationDiagram(){

    
    //fillObstacles(collisions);
}

bool cCoordinationDiagram::mySortFunction(pair<cCell, int> a, pair<cCell, int> b){

    return a.second < b.second;
}

void cCoordinationDiagram::generateGrid(vector< cPath > paths){

    vector<int> dimSizes;

    for(int i=0; i<paths.size(); ++i){
        dimSizes.push_back(paths[i].size());
    }
    mGrid = cGrid(dimSizes); 
}

cCollision cCoordinationDiagram:: detectCollisions (vector< cPath > paths){
    map <cCell, vector < pair<int,int> > > collisions, result;
    for(int i = 0 ; i < paths.size(); i++){
        for(int j = 0 ; j< paths[i].size();j++){
            if(collisions.find(paths[i][j]) != collisions.end()){
                collisions[paths[i][j]].push_back(make_pair(i,j));
                result[paths[i][j]]=collisions[paths[i][j]];
            }else{
                vector<pair<int,int> > col;
                col.push_back(make_pair(i,j));
                collisions[paths[i][j]] = col;
                
            }
        }
    }

    return result;
}

typedef  map< cCell, vector < pair < int, int > > > cCollision;
typedef  pair < vector < int >, cCollision > cCollision_zone;

vector < cCollision_zone > cCoordinationDiagram:: classifyCollisions(cCollision collisions){
    mPathsMap.clear();
    map<cCell, bool> visited;
    map <cCell, vector < pair<int,int> > >::iterator it;
    for ( it = collisions.begin(); it != collisions.end(); it++ ){
        for(int i=0; i<it->second.size(); ++i){
            mPathsMap[it->second[i].first].push_back(make_pair(it->first,it->second[i].second));
        }
        visited[it->first] = false;
    }
    // Uncomment to print pathsMap
    /* cout<<"First step:"<<endl;
    map<int, vector< pair<cCell, int> > >::iterator it1;
    for ( it1 = pathsMap.begin(); it1 != pathsMap.end(); it1++ ){
        sort(it1->second.begin(), it1->second.end(), mySortFunction);
        std::cout <<"("<<it1->first<<"): ";
        for(int i=0; i<it1->second.size(); ++i){
            cout<<"<cell: ("<<it1->second[i].first.pos[0]<<","<<it1->second[i].first.pos[1]<<"), pos: "<<it1->second[i].second<<">,";
        }
        cout<<endl;
    } */

    vector < cCollision_zone > result;
    map <cCell, vector < pair<int,int> > >::iterator it2;
    
    for ( it2 = collisions.begin(); it2  != collisions.end(); it2++ ) {
        pair < vector < int >, vector < cCell > > newGroup;
        int id_paths_it = 0; //current_group
        if (!visited[it2->first]) {
            // Paso 1:
            vector<cCell>::iterator exists;
            exists = find(newGroup.second.begin(), newGroup.second.end(), it2->first);
            if (exists == newGroup.second.end()) {
                newGroup.second.push_back(it2->first);
            }
            // Paso 2:
            for (int actual_cell = 0; actual_cell < newGroup.second.size(); ++actual_cell) {
                visited[newGroup.second[actual_cell]] = true;
                bool inserted = false;
                // Paso 3:
                for ( int i = 0; i< collisions[newGroup.second[actual_cell]].size(); i++){
                    vector<int>::iterator exists;
                    //cout<<"Busco: "<<it2->second[i].first;
                    exists = find(newGroup.first.begin(), newGroup.first.end(), collisions[newGroup.second[actual_cell]][i].first);
                    if (exists == newGroup.first.end()){
                        newGroup.first.push_back(collisions[newGroup.second[actual_cell]][i].first);
                        // cout<<"agrego"<<it2->second[i].first<<endl;
                        inserted = true;  
                    }
                }
                if(inserted){
                    // Pasos 4 y 5:
                    for (int j = id_paths_it; j<newGroup.first.size(); ++j) {
                        for (int k = 0; k<mPathsMap[newGroup.first[j]].size(); ++k) {
                            vector<cCell>::iterator exists;
                            exists = find(newGroup.second.begin(), newGroup.second.end(), mPathsMap[newGroup.first[j]][k].first);
                            if (exists == newGroup.second.end()) newGroup.second.push_back(mPathsMap[newGroup.first[j]][k].first);
                        }
                    }
                    id_paths_it = newGroup.first.size() - 1;
                }
            }
        }
        if (newGroup.second.size() > 0) {
            cCollision_zone group_to_insert;
            group_to_insert.first = newGroup.first;
            cCollision aux_map;
            for (int i=0; i < newGroup.second.size(); ++i) {
                aux_map[newGroup.second[i]] = collisions[newGroup.second[i]];
            }
            group_to_insert.second = aux_map;
            result.push_back(group_to_insert);
            newGroup.first.clear();
            newGroup.second.clear();
            aux_map.clear();
        }
    }

    return result;
}

void cCoordinationDiagram::fillObstacles(map <cCell, vector < pair<int,int> > > collisions){
    
    vector<int> pos;
    cCell *position;
    map <cCell, vector < pair<int,int> > >::iterator it;
    for ( it = collisions.begin(); it != collisions.end(); it++ ){
		
        if(it->second.size() == mDimSizes.size()){

            for(int i=0; i<it->second.size(); ++i){
                pos.push_back(it->second[i].second);
            }
            position=mGrid.getCell(pos);
            position->obstacle=true;
            pos.clear();   
        }
        else{
            
            int numDims = abs((int)it->second.size() - (int)mDimSizes.size());
            vector<int> dimSizes;
            //bool missing [mDimSizes.size()] = {true};
            bool missing [mDimSizes.size()];
            for(int i = 0; i <mDimSizes.size();i++){
                missing[i]=true;
            }
            for(int i=0; i<it->second.size(); ++i){
                missing[it->second[i].first] = false;
                
            }
            int numCells = 1;
            for(int i=0; i<mDimSizes.size(); ++i){
                if(missing[i]){
                    numCells *= mDimSizes[i];
                    dimSizes.push_back(mDimSizes[i]);
                }
            }
            
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
						
						rest -= pos[k]*mult;
					}
					
					pos.push_back(((i+rest)/div) % auxDimSizes[j]);
				}
                
                vector<int> posFinal(mDimSizes.size());
                int a, b;
                a=b=0;
                
                for(int j=0; j<mDimSizes.size(); ++j){
                    if(missing[j]){
                        posFinal[j]=pos[a];
                        ++a;
                    }
                    else{
                        posFinal[j]=it->second[b].second;
                        ++b;
                    }
                }
                
				mGrid.getCell(posFinal)->obstacle = true;
                pos.clear();
			}
        }
        
    }

    //mGrid.print();

    //mGrid.getCell(pos)//*cCell... ->obstacle 
    /*
    
    1) Iterar por el mapa
    2) Por cada llave (celda) observar su valor (vector) para ver
        si su tamaño corresponde al numero de caminos.
        2.1) Caso facil: su tamaño si es el mismo, obtengo la grilla
                con esas coordenadas y la seteo como obstáculo
        2.2) Caso dificil: su tamaño no es el mismo
            2.2.1) Obtener los ids de los caminos faltantes y guardar en
                    la variable numDims la cantidad de caminos faltantes 
            2.2.2) Obtener el tamaño de cada uno de esos caminos
            2.2.3) Crear una variable numCells que sea la multiplicación
                    de esos tamaños
            2.2.4) Ejecutar un procedimiento parecido al de mGrid.init()
                    para obtener la posición de cada una de las celdas
                    que deben ser seteadas como obstáculo y setearlas

    */  
}

vector < pair<cCell, vector < pair<int,int> > > > cCoordinationDiagram::sortCollisions(cCollision collisions, vector <int> idPaths){

    map<int, vector< pair<cCell, int> > > pathsMap;
    map <cCell, vector < pair<int,int> > >::iterator it;
    for (int i=0; i < idPaths.size(); ++i) {
        pathsMap[idPaths[i]] = mPathsMap[idPaths[i]];
    }

    cout<<"First step:"<<endl;
    map<int, vector< pair<cCell, int> > >::iterator it1;
    for ( it1 = pathsMap.begin(); it1 != pathsMap.end(); it1++ ){
        sort(it1->second.begin(), it1->second.end(), mySortFunction);
        std::cout <<"("<<it1->first<<"): ";
		for(int i=0; i<it1->second.size(); ++i){
			cout<<"<cell: ("<<it1->second[i].first.pos[0]<<","<<it1->second[i].first.pos[1]<<"), pos: "<<it1->second[i].second<<">,";
		}
		cout<<endl;
    }

    vector<pair<cCell,int> > positions(collisions.size());
    vector < pair<cCell, vector < pair<int,int> > > > result (collisions.size());
    int pos=0;
    for ( it = collisions.begin(); it != collisions.end(); it++ ){
        positions[pos].first = it->first;
        positions[pos].second = 0;
        for(int i=0; i<it->second.size(); ++i){
            for(int j=0; j<pathsMap[it->second[i].first].size(); ++j){
                if(pathsMap[it->second[i].first][j].first==it->first && positions[pos].second<j){
                    positions[pos].second = j;
                }
            }
        }
        ++pos;
    }

    sort(positions.begin(), positions.end(), mySortFunction);

    for(int i=0; i<positions.size(); ++i){
        result[i] = make_pair(positions[i].first, collisions[positions[i].first]);
    }

    // 1. Iterar pathsMap por cada camino:
    map<int, vector< pair<cCell, int> > >::iterator it3;
    for ( it3 = pathsMap.begin(); it3 != pathsMap.end(); it3++ ){
        // 1.1 Buscar la primera celda en result
        // 1.2 Buscar el resto de celdas a partir de la posición en que fue encontrada la celda anterior
        int resultIt = 0;
        for(int i=0; i<it3->second.size(); ++i){
            while(resultIt<result.size()){
                if(result[resultIt].first==it3->second[i].first){
                    ++resultIt;
                    break;
                }
                ++resultIt;
            }
            // 1.3 Si la celda no se encuentra entonces break CICLO DETECTADO
            if(resultIt>=result.size() && i!=it3->second.size()-1){
                cout<<"<<<<CICLO DETECTADO>>>>>"<<endl;
                result.clear();
                return result;
            }
        }
    }
    
    // 2. Si se termina de iterar todo pathsMap entonces se retorna result y CICLO NO DETECTADO
    cout<<"Second step:"<<endl;
    vector < pair<cCell, vector < pair<int,int> > > >::iterator it2;
    for ( it2 = result.begin(); it2 != result.end(); it2++ ){
		
		std::cout <<"("<<it2->first.pos[0]<<","<<it2->first.pos[1]<<"): ";
		for(int i=0; i<it2->second.size(); ++i){
			cout<<"(id: "<<it2->second[i].first<<", pos:"<<it2->second[i].second<<"), ";
		}
		cout<<endl;
		
	}

    
    return result;
}

cPath cCoordinationDiagram::solveSortedCollisions(vector < pair<cCell, vector < pair<int,int> > > > collisions, vector<int> dimSizes, vector< vector< int > > &finalDelaysPerPath){

    //int delays[dimSizes.size()] = {0};
    int delays[dimSizes.size()]; 
    for(int i = 0; i <dimSizes.size();i++){
                delays[i]=0;
            }

    /*vector< vector< int > > finalDelaysPerPath (dimSizes.size());
    for (int j=0; j<dimSizes.size(); ++j) {
        finalDelaysPerPath[j].resize(dimSizes[j]);
    }*/

    for(int i=0; i<collisions.size(); ++i){

        map <cCell, vector < pair<int,int> > > auxCol;

        auxCol[collisions[i].first] = collisions[i].second;
        vector<int> auxDimSizes, finColPos;
        //cout<<"DimSizes: "<<dimSizes.size()<<endl;
        for(int j=0; j<collisions[i].second.size(); ++j){
            int auxDelay = 0;
            if(delays[collisions[i].second[j].first]){
                auxDelay = delays[collisions[i].second[j].first];
                auxCol[collisions[i].first][j].second += auxDelay;
            }
            cout<<"LLEGO "<<collisions[i].second[j].first<<endl;
            auxDimSizes.push_back(dimSizes[ collisions[i].second[j].first ]+auxDelay);
            // A collision is solved when we arrive to finColPos
            finColPos.push_back(auxCol[collisions[i].first][j].second+1);
        }

        cCell finCol;
        finCol.pos = finColPos;
        cPath localPath = calculateVelocityPath(auxDimSizes, auxCol);
        vector< vector< int > > delaysPerPath (auxDimSizes.size());
        vector< int > delaysPerPathIndexes;
        for (int j=0; j<auxDimSizes.size(); ++j) {
            delaysPerPath[j].resize(auxDimSizes[j]);
            cout<<"ID:"<<collisions[i].second[j].first<<endl;
            delaysPerPathIndexes.push_back(collisions[i].second[j].first);
        }

        purifyPath(localPath, delaysPerPath);

        for (int j=0; j<delaysPerPath.size(); ++j) {
            int acc = 0;
            for (int k=0; k<delaysPerPath[j].size(); ++k) {
                // Only if the collision is not solved
                if (k < finColPos[j]){ //break;
                    // Add the calculated delay to an accumulator for the total sum of delays
                    acc += delaysPerPath[j][k];
                    // Calculate the slipage
                    int diff = delaysPerPath[j].size() - dimSizes[delaysPerPathIndexes[j]];
                    // Store the calculated delay considering the slipage
                    finalDelaysPerPath[delaysPerPathIndexes[j]][k-diff] += delaysPerPath[j][k];
                }
            }
            // Store the sum of delays of the current path
            delays[delaysPerPathIndexes[j]] += acc;
        }


        /*vector <bool> dimsChecked(localPath[0].pos.size());
        int numSuccedDims = 0;
        for (int j=0; j<localPath.size(); ++j) {
            for (int k=0; k<localPath[j].pos.size(); ++k) {
                if (!dimsChecked[k] && localPath[j].pos[k] == finColPos[k]) {
                    dimsChecked[k] = true;
                    numSuccedDims++;
                }
            }
            if (numSuccedDims==dimsChecked.size()-1) {
                cout<<"\nPath is relevant until here ";
                for (int k=0; k<localPath[j].pos.size(); ++k) {
                    cout<<localPath[j].pos[k]<<",";
                }
                cout<<endl;
                break;
            }
        }*/

        /**
        
        for(cPath::iterator j=localPath.begin()+2; j!=localPath.end(); ++j){
            
            if(mGrid.euclideanDistance(&(*j),&(*(j-2))) != 2.0 && mGrid.euclideanDistance(&(*j),&(*(j-1))) == 1.0){
                localPath.erase(j-1);
            }
        }

        for(int i = 0; i < localPath.size(); i++)
        {
            cout<<localPath[i].pos[0]<<" "<<localPath[i].pos[1]<<endl;
        }


        bool stop=false;
        for(int j=1; j<localPath.size(); ++j){
            //TODO: cambiar este if a nD
            //if(mGrid.euclideanDistance(&localPath[j], &localPath[j-1])==1.0 && mGrid.euclideanDistance(&localPath[j], &localPath[j-2])==2.0){
                for(int k=0; k<collisions[i].second.size(); ++k){
                    if(abs(localPath[j].pos[k]-localPath[j-1].pos[k])==0){
                        delays[collisions[i].second[k].first]++;
                        cout<<"Delay en: "<<localPath[j].pos[0]<<" "<<localPath[j].pos[1]<<endl;
                        //TODO: revisar delays
                    }
                    if(localPath[j].pos[k]>collisions[i].second[k].second)
                        stop = true;
                }
            //}
            if(stop) break;
        }
        */


        cout<<"DELAYS: ";

        for(int j=0; j<dimSizes.size(); ++j){

            cout<<delays[j]<<",";
        }
        cout<<endl;

        cout<<"Resolvi"<<endl;
    }

    /*cout<<"##### FINAL DELAYS PER PATH: ######"<<endl;
    for (int i=0; i<finalDelaysPerPath.size(); ++i) {
        for (int j=0; j<finalDelaysPerPath[i].size(); ++j) {
            cout<<finalDelaysPerPath[i][j]<<",";
        }
        cout<<endl;
    }*/

    cPath velocityPath;

    return velocityPath;
}

cPath cCoordinationDiagram::calculateVelocityPath(vector<int> dimSizes, cCollision collisions){

    mDimSizes = dimSizes;
    /*cout<<"Vivoooo "<<dimSizes.size()<<endl;   
    mGrid = cGrid(mDimSizes);
    cout<<"GRILLA GENERADA"<<endl;
    fillObstacles(collisions);*/
    map<int, vector< pair<cCell, int> > > pathsMap;
    map <cCell, vector < pair<int,int> > >::iterator it;
    for ( it = collisions.begin(); it != collisions.end(); it++ ){
        for(int i=0; i<it->second.size(); ++i){
            pathsMap[it->second[i].first].push_back(make_pair(it->first,it->second[i].second));
        }
    }

    cPath  sol;
	// Astar aStar_planner = Astar(&mGrid);
    Astar aStar_planner = Astar(pathsMap, dimSizes);
	cCell start;
	cCell goal;
	for (int i = 0 ; i< mDimSizes.size();i++){
		start.pos.push_back(0);
		goal.pos.push_back(mDimSizes[i]-1);
	}
	cout<<"cell Start: (";
	for(int i  = 0; i < start.pos.size();i++){
		cout<<start.pos[i]<<" - ";
	}	
	cout<<")"<<endl;
	cout<<"cell goal: (";
	for(int i  = 0; i < goal.pos.size();i++){
		cout<<goal.pos[i]<<" - ";
	}	
	cout<<")"<<endl;
	//aStar_planner.AstarSerch(*mGrid.getCell(goal.pos),*mGrid.getCell(start.pos));
    aStar_planner.newSearch(goal, start);
	
	//sol  = aStar_planner.reconstructionMap(*mGrid.getCell(goal.pos), *mGrid.getCell(start.pos));
    sol  = aStar_planner.newReconstructionMap(goal, start);
    /*for(int i=0; i<sol.size(); ++i){
        for(int j = 0; j < sol[i].pos.size(); ++j)
        {
            cout<<sol[i].pos[j]<<", ";
        }
        cout<<endl;
    }*/

	return sol;
    /*
    
    1. Crear un Astar que recibe como parametro la direccion de la grilla
    2. Ejecutar Search desde 0,0 hasta largodelcamino1-1,largodelcamino2-1
    3. Retornar el resultado de reconstructionMap
    4. Probar que funcione
    
    */
}

double euclideanDistance(cCell* a, cCell* b){            
    float dist = 0;
    //cout<<b->pos.size();
    for(int i=0; i<a->pos.size(); ++i){
        dist += pow(a->pos[i]-b->pos[i], 2);
    }
    return sqrt(dist);
}

cPath cCoordinationDiagram:: purifyPath(cPath i_path, vector< vector <int> > &originalPathsDelays){
    cPath o_path;
    o_path = i_path;
    cPath::iterator it;

    for (it  = i_path.begin()+2; it != i_path.end(); it++){
        int dif = 0;
        int affectedDim;
        for(int j = 0; j < it->pos.size(); j++){
            dif+=pow(it->pos[j]-(it-2)->pos[j],2);
        }
        /*cout<<it->pos[0]<<","<<it->pos[1]<<","<<it->pos[2]<<"&";
        cout<<(it-1)->pos[0]<<","<<(it-1)->pos[1]<<","<<(it-1)->pos[2];*/
        if(dif <= it->pos.size() &&  euclideanDistance(&(*it),&(*(it-1)))== 1){
            //cout<<"\nentra 1: \n";
            i_path.erase(it-1);
            --it;
        }
    }

    for (int i = 1; i < i_path.size(); ++i) {
        for (int j = 0; j < i_path[i].pos.size(); ++j) {
            if (i_path[i].pos[j] - i_path[i-1].pos[j] == 0 && i_path[i].pos[j] != originalPathsDelays[j].size() - 1) {
                originalPathsDelays[j][i_path[i].pos[j]] += 1;
            }
        }
    }

    return i_path;
}
