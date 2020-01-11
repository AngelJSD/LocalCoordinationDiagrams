#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

#define MIN_PATH_LARGE 4 // unused
#define MAX_PATH_LARGE 8
#define RADIUS_GOAL_LARGE 5

#define SHOW_PATHS_INIT_AND_GOAL 1

void  fillGrid (string namefile,cGrid* mGrid){
    cout<<namefile<<" de obstaculos leido"<<endl;
        ifstream file;
        vector<cCell> p1;
        file.open((namefile));
        string frase, token;
        if (file.fail())
        {
            cout << "Error al abrir el " << namefile << endl;
            return;
        }
        else
        {
            while (!file.eof())
            {
                getline(file, frase);
                istringstream iss(frase);
                int i = 0;
                int x,y;
                vector<int>  cel1;
                while (getline(iss, token, ',') && i < 2)
                {
                    string tok(token.c_str());
                    cel1.push_back(stoi(tok));
                    i++;
                }
                if(cel1.size()>0){
                    //debug
                    /*for(int g =0;g<cel1.size();g++){
                        cout<<"a"<<cel1[g];
                    }*/
                    mGrid->setObstacle(cel1);
                }
                cel1.clear();                
            }
            file.close();
        }
        //mGrid->print();
}

vector<vector<cCell> >  createPath (int n,cGrid mGrid){
    srand((unsigned)time(NULL));
    vector<vector<cCell> > paths;
    vector<cCell> container;
	cCell start;
	cCell goal;

    int factor =  mGrid.dimSizes[0]/n;

    for(int i=0;i<n;i++){
        
        Astar aStar_planner = Astar(&mGrid);             
        int startX = ((i * factor)/2) + rand() % ((i*factor)+1-((i*factor)/2));
        int startY = rand() % mGrid.dimSizes[1];
        start.pos.push_back(startX);//i
        start.pos.push_back(startY);//j
        
        while(mGrid.getCell(start.pos)->obstacle){
            start.pos.clear();
            startX = ((i * factor)/2) + rand() % ((i*factor)+1-((i*factor)/2));
            startY = rand() % mGrid.dimSizes[1];
            start.pos.push_back(startX);//i
            start.pos.push_back(startY);//j
        }

        //variable = limite_inferior + rand() % (limite_superior +1 - limite_inferior) ;

        int goalX = (((i+1) * factor)/2) + rand() % (((i+1)*factor)+1-(((i+1)*factor)/2));
        int goalY = startY+ rand()%((startY+RADIUS_GOAL_LARGE)+1-startY);
        goal.pos.push_back(goalX);//i
        goal.pos.push_back(goalY);//j
        
        while(mGrid.getCell(goal.pos)->obstacle){
            goal.pos.clear();
            goalX = (((i+1) * factor)/2) + rand() % (((i+1)*factor)+1-(((i+1)*factor)/2));
            goalY = rand() % mGrid.dimSizes[1];
            goal.pos.push_back(goalX);//i
            goal.pos.push_back(goalY);//j
        }
        
        //cout<<"dist: "<<euclideanDistance(&start,&goal)<<endl;
        if(euclideanDistance(&start,&goal)>MAX_PATH_LARGE  ){
            i--;
        }else{
            if(euclideanDistance(&start,&goal)<MIN_PATH_LARGE){
                i--;
            }else{
                //cout<<"from ("<<start.pos[0]<<","<<start.pos[1]<<")al("<<goal.pos[0]<<","<<goal.pos[1]<<")\n";
                aStar_planner.AstarSerch(*mGrid.getCell(start.pos),*mGrid.getCell(goal.pos));
                //cout<<"vivo 4 :\n";
                container  = aStar_planner.reconstructionMap(*mGrid.getCell(start.pos), *mGrid.getCell(goal.pos));
                //cout<<"vivo 5 :\n";
                paths.push_back(container);
                container.clear();
                if(SHOW_PATHS_INIT_AND_GOAL == 1){
                    cout<<"camino "<<i<<": de ("<<start.pos[0]<<","<<start.pos[1]<<") a ("<<goal.pos[0]<<","<<goal.pos[1]<<")\n";    
                }
            }  
        }
        start.pos.clear();
        goal.pos.clear();
        }

    return paths;
}

void exportPaths(vector<vector<cCell> > paths, string extension){
    
    int i = 0;
    for (std::vector<vector<cCell>>::iterator it = paths.begin() ; it != paths.end(); ++it){
        string namefile = "gPath";
        string str_i = to_string(i);
        namefile+=str_i;
        namefile+=extension;
        cout<<namefile<<" generado: con "<<(*it).size()<<"celdas"<<endl;
        ofstream file;
        file.open (namefile);
        for(int j = 0 ; j < (*it).size();j++ ){
            file<<paths[i][j].pos[0]<<","<<paths[i][j].pos[1];
            if(j != (*it).size()-1){
                file<<endl;
            }
        }
        file.close();
        i++;
    }
}

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

void exportCollisions (vector<vector<cCell> > paths){
    cout<<"in--exportCollisions\n";
    vector<cCell> collisionCells;
    map <cCell, vector < pair<int,int> > > collisions, result;
    for(int i = 0 ; i < paths.size(); i++){
        for(int j = 0 ; j< paths[i].size();j++){
            if(collisions.find(paths[i][j]) != collisions.end()){
                //collisionCells.push_back(paths[i][j]); 
                collisions[paths[i][j]].push_back(make_pair(i,j));
                result[paths[i][j]]=collisions[paths[i][j]];
            }else{ 
                vector<pair<int,int> > col;
                col.push_back(make_pair(i,j));
                collisions[paths[i][j]] = col;                
            }
        }
    }

    

    string namefile = "dCollisions.csv";
    ofstream file;
    file.open (namefile);
    for(int j = 0 ; j < collisionCells.size();j++ ){
        file<<collisionCells[j].pos[0]<<","<<collisionCells[j].pos[1]<<endl;
    }
    file.close();
    cout<<"out--exportCollisions\n";
}