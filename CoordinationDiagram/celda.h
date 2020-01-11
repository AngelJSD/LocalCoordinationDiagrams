#include <cstdlib> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <stdio.h> 
#include <vector>
#include <numeric>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
using namespace std;

class Cell
{
  public:
    float s, t;       // Coordenadas de texturas
    float nx, ny, nz; // Coordenadas de la normal
    float x, y, z;    // posición del vértice
    float width, height, sizeZ ; //dimensiones
    float red, green, blue; // color de la celda

    Cell(int x, int y, int z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->width  = 1;
        this->height = 1;
        this->sizeZ  = 1;
    }

    void setColor(float red, float green, float blue){
        this -> red = red;
        this -> green = green;
        this -> blue = blue;
    }


    void printCel()
    {
        cout << " ( " << x << " , " << y << " , " << z << " ) ";
    }

    void drawCel() {
    glColor3f(red, green, blue);
      
      // Front
      glBegin(GL_POLYGON);
      glVertex3f(x, y, z);
      glVertex3f(x + width, y , z);
      glVertex3f(x + width, y + height, z);
      glVertex3f(x, y + height, z);
      glEnd();
      
      // Bottom
      glBegin(GL_POLYGON);
      glVertex3f(x, y, z);
      glVertex3f(x + width, y, z);
      glVertex3f(x + width, y, z + sizeZ);
      glVertex3f(x, y, z + sizeZ);
      glEnd();
      
      // Left
      glBegin(GL_POLYGON);
      glVertex3f(x, y, z);
      glVertex3f(x, y, z + sizeZ);
      glVertex3f(x, y + height, z + sizeZ);
      glVertex3f(x, y + height, z);
      glEnd();
      
      // Back
      glBegin(GL_POLYGON);
      glVertex3f(x, y, z + sizeZ);
      glVertex3f(x + width, y, z + sizeZ);
      glVertex3f(x + width, y + height, z + sizeZ);
      glVertex3f(x, y + height, z + sizeZ);
      glEnd();
      
      // Top
      glBegin(GL_POLYGON);
      glVertex3f(x, y + height, z);
      glVertex3f(x + width, y + height, z);
      glVertex3f(x + width, y + height, z + sizeZ);
      glVertex3f(x, y + height, z + sizeZ);
      glEnd();
      
      // Right
      glBegin(GL_POLYGON);
      glVertex3f(x + width, y, z);
      glVertex3f(x + width, y, z + sizeZ);
      glVertex3f(x + width, y + height, z + sizeZ);
      glVertex3f(x + width, y + height, z);
      glEnd();

      //Uncomment when textures what needed
      /*
      setTexCoords({{0, 1}, {1, 1}, {1, 0}, {0, 0} , {0, 0}, {1, 0}, {1, 1}, {0, 1} ,
                      {1, 1}, {0, 1}, {0, 0}, {1, 0} , {0, 1}, {1, 1}, {1, 0}, {0, 0} ,
                      {0, 1}, {1, 1}, {1, 0}, {0, 0} , {0, 1}, {1, 1}, {1, 0}, {0, 0} });
      */
      return;
    }

  void drawRefCel(float left, float bottom,  float z, float red, float green, float blue) {
  float width = 1;
  float height = 1;
  float sizeZ = 1;
  glColor3f(red, green, blue);
  
  // Front
  glBegin(GL_LINE_LOOP);
  glVertex3f(left, bottom, z);
  glVertex3f(left + width, bottom , z);
  glVertex3f(left + width, bottom + height, z);
  glVertex3f(left, bottom + height, z);
  glEnd();
  
  // Bottom
  glBegin(GL_LINE_LOOP);
  glVertex3f(left, bottom, z);
  glVertex3f(left + width, bottom, z);
  glVertex3f(left + width, bottom, z + sizeZ);
  glVertex3f(left, bottom, z + sizeZ);
  glEnd();
  
  // Left
  glBegin(GL_LINE_LOOP);
  glVertex3f(left, bottom, z);
  glVertex3f(left, bottom, z + sizeZ);
  glVertex3f(left, bottom + height, z + sizeZ);
  glVertex3f(left, bottom + height, z);
  glEnd();
  
  // Back
  glBegin(GL_LINE_LOOP);
  glVertex3f(left, bottom, z + sizeZ);
  glVertex3f(left + width, bottom, z + sizeZ);
  glVertex3f(left + width, bottom + height, z + sizeZ);
  glVertex3f(left, bottom + height, z + sizeZ);
  glEnd();
  
  // Top
  glBegin(GL_LINE_LOOP);
  glVertex3f(left, bottom + height, z);
  glVertex3f(left + width, bottom + height, z);
  glVertex3f(left + width, bottom + height, z + sizeZ);
  glVertex3f(left, bottom + height, z + sizeZ);
  glEnd();
  
  // Right
  glBegin(GL_LINE_LOOP);
  glVertex3f(left + width, bottom, z);
  glVertex3f(left + width, bottom, z + sizeZ);
  glVertex3f(left + width, bottom + height, z + sizeZ);
  glVertex3f(left + width, bottom + height, z);
  glEnd();

  //Uncomment when textures what needed
  /*
  setTexCoords({{0, 1}, {1, 1}, {1, 0}, {0, 0} , {0, 0}, {1, 0}, {1, 1}, {0, 1} ,
                  {1, 1}, {0, 1}, {0, 0}, {1, 0} , {0, 1}, {1, 1}, {1, 0}, {0, 0} ,
                  {0, 1}, {1, 1}, {1, 0}, {0, 0} , {0, 1}, {1, 1}, {1, 0}, {0, 0} });
  */  
  return;
}
};



class Grilla
{
  public:
    Grilla(){
      MAX_PATH_SIZE = 0;
      MAX_PATH_ID = 0;  
    }
    ~Grilla() {}
    
    int MAX_PATH_SIZE; // size del camino mas largo leido
    int MAX_PATH_ID; // ID del camino mas largo leido

    int MAX_PATH_SIZERef; // size del camino mas largo leido
    int MAX_PATH_IDRef; // ID del camino mas largo leido

    vector<Cell> celdasPath;
    vector<vector<Cell>> Paths;
    vector<vector<Cell>> PathsRef;
    vector<vector<int>> DelaysOriginals;
    vector<vector<int>> Delays;
    vector<int> currentPos;
    vector<int> lastPos;
    vector<Cell> startPath;
    vector<Cell> endPath;

    bool loadPathInfo(string, bool);
    bool loadPathInfoRef();
    void loadPathInfoSet(int, string,string);
    void assignColor(vector<Cell> & Path);
    void drawAllPaths(bool);
    void drawGrillaRef (int, int, int);
    bool drawAnimatedPath(int,int);
    void drawAllAnimatedPath(float);
    void loadDelays(int,string,string);

};

bool Grilla::loadPathInfo(string namefile, bool asg = 1)
{
    ifstream fichero;
    fichero.open((namefile));
    string frase, token;
    int line = 1;
    int linea_abre = 1;
    if (fichero.fail())
    {
        cout << "Error al abrir el " << namefile << endl;
        return false;
    }
    else
    {
        
        while (!fichero.eof())
        {
            getline(fichero, frase);
            istringstream iss(frase);
            int i = 0;
            int x,y,z;
            while (getline(iss, token, ',') && i < 3)
            {
                string tok(token.c_str());
                if(i == 0){y=stoi(tok);} // reflejado en x
                if(i == 1){x=stoi(tok);} // reflejado en x
                if(i == 2){z=0;}//stoi(tok);}
                i++;
            }
            //cout<<"("<<x<<","<<y<<","<<") y "<<"("<<x1<<","<<y1<<","<<")";//debug
            Cell c(x,y,z);
            celdasPath.push_back(c);
        }
        fichero.close();
    }
    cout<< namefile << " Subido con "<<celdasPath.size() <<" celdas "<< endl;
    if(MAX_PATH_SIZE < celdasPath.size()){
      MAX_PATH_SIZE = celdasPath.size(); 
      MAX_PATH_ID = Paths.size();
    } 
    
    Paths.push_back(celdasPath);
    currentPos.push_back(0);
    lastPos.push_back(-1);
    if(asg){
      assignColor(Paths[Paths.size()-1]);  
    }    

    for (int i = 0; i < celdasPath.size();i++)
    {
        //celdasPath[i].printCel();
        //cout<<celdasPath[i].red;
        //cout<<endl;
    }
    celdasPath.clear();
    return true;
}

bool Grilla::loadPathInfoRef()
{
  cout<<"IN loadPathInfoRef";
  for(int j = 0;j < Paths.size(); j++){
    vector <Cell> tempRef ;
    for (int i = 0; i < Paths[j].size(); ++i)
      {
          Cell c(Paths[j][i].x,Paths[j][i].y,Paths[j][i].z);
          tempRef.push_back(c);
      } 
    PathsRef.push_back(tempRef);
  }   
  return true; 
}

float rndColor ;
float rndColor1 ;
float rndColor2;
void Grilla:: assignColor( vector<Cell> & path){
   rndColor  = (0.1*(rand() % 10 + 1));
   rndColor1 = (0.1*(rand() % 10 + 1));
   rndColor2 = (0.1*(rand() % 10 + 1));
  cout<<"color:"<<rndColor<<" , "<<rndColor1<<" , "<<rndColor2<<" , \n";
  for(int i = 0; i< path.size();i++ ){
    path[i].setColor(rndColor,rndColor1,rndColor2);
  }
}

void Grilla::drawAllPaths(bool ref = 1){
    //cout<<"dibujar"<< Paths.size()<<"caminos\n";  
    if(ref == 1){
      for(int j = 0;j < Paths.size(); j++){
        for (int i = 0; i < Paths[j].size(); i++)
          {
              Paths[j][i].drawCel();
          }
      }          
    }else{
      for(int j = 0;j < Paths.size(); j++){
        for (int i = 0; i < Paths[j].size(); i++)
          {
              PathsRef[j][i].drawCel();
          }
      }          
    }


    
}

void Grilla:: drawGrillaRef (int dim_x, int dim_y, int dim_z){
  for(int z = 0; z< dim_z;z++){ 
    for(int y = 0; y< dim_y;y++){
      for(int x = 0; x< dim_x;x++){
        Cell c(x,y,z);
        c.drawRefCel(x,y,z,1,1,1);
      }
    }
  }
}

void Grilla::loadPathInfoSet(int n, string head,string extension){
  for(int i = 0 ; i< n; i++){
    string namefile = head;
    string str_i = to_string(i);
        namefile+=str_i;
      namefile+=extension;
      loadPathInfo(namefile);
      Cell c(Paths[i][0].x,Paths[i][0].y,Paths[i][0].z);
      startPath.push_back(c);
      startPath[i].setColor(Paths[i][0].red, Paths[i][0].green, Paths[i][0].blue);

      int lastPos = Paths[i].size()-1;
      Cell cellEnd(Paths[i][lastPos].x,Paths[i][lastPos].y,Paths[i][lastPos].z);
      endPath.push_back(cellEnd);
      endPath[i].setColor(Paths[i][lastPos].red, Paths[i][lastPos].green, Paths[i][lastPos].blue);
  }

 /*//only debug
  cout<<"--------Caminos  leidos------\n";
  for (int i = 0; i <  Paths.size(); ++i)
  {
    cout<<"P"<<i<<": ";
    for (int j = 0; j < Paths[i].size(); ++j)
    {
       cout<<Paths[i][j].x<<" , "<<Paths[i][j].y<<")(";
    }
    cout<<endl;
  }*/
}

bool Grilla::drawAnimatedPath(int id_path, int pos_time){
  if(pos_time < Paths[id_path].size()){
    Paths[id_path][currentPos[id_path]].drawCel();
    if(pos_time != lastPos[id_path]){
     // cout<<pos_time<<" in P"<<id_path<<" : currentPos: "<<currentPos[id_path]<<endl;
      if(Delays[id_path][currentPos[id_path]] == 0){
        currentPos[id_path]++;  
      }else{
      //  cout<<"D: "<<Delays[id_path][currentPos[id_path]]<<endl;
        Delays[id_path][currentPos[id_path]]--;
      }         
      lastPos[id_path] = pos_time; 
    }
    return true;
  }else{
    return false;
  }
}

void Grilla::drawAllAnimatedPath(float time){
  int pos_time =time/1.0;
  for(int j = 0;j < Paths.size(); ++j){
    drawAnimatedPath(j, pos_time);
  }
}

void Grilla::loadDelays(int maxPath, string head, string extension){
  for (int i = 0; i< maxPath;i++ ){
        string namefile = head;
        string str_i = to_string(i);
        namefile+=str_i;
        namefile+=extension;
        cout<<namefile<<" leido"<<endl;
        ifstream file;
        vector<int> p1;
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
                int tmp1;
                while (getline(iss, token, ',') && i < 1)
                {
                    string tok(token.c_str());
                    p1.push_back(stoi(tok)); 
                    i++;
                }              
            }
            cout<<endl;
            file.close();
            Delays.push_back(p1);
            DelaysOriginals.push_back(p1);
            p1.clear();
        } 
    }

  cout<<"--------DELAYS  leidos------\n";
  for (int i = 0; i <  Paths.size(); ++i)
  {
    cout<<"P"<<i<<": ";
    for (int j = 0; j < Paths[i].size(); ++j)
    {
       cout<<Delays[i][j]<<" . ";
    }
    cout<<endl;
  }
}

  
    
    