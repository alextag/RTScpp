#include <iostream>
#include <cmath>
#include <stdlib.h>
#include<algorithm>
#include<ctime>
#include "Astar.h"

using namespace std;

int main(){
  int start[2],end[2],size[3];
  cout<<"Enter Starting point:\nX: ";
  cin>>start[0];
  cout<<"Y: ";
  cin>>start[1];
  cout<<"Enter Destination:\nX: ";
  cin>>end[0];
  cout<<"Y: ";
  cin>>end[1];
  size[0]=size[1]=800;
  int **map;
  map = new int*[size[0]];
  int i=0,j;

  while(i<size[0]){
    map[i]=new int[size[1]];
    j=0;
    while(j<size[1]){
      map[i][j]=1;
      j++;}
    i++;}

  //map[2][2]=2;

  int **path;
  clock_t t1, t2;
  t1 = clock();

  path=Astar(start,end,map,size);
  t2 = clock();
  float diff = (((float)t2 - (float)t1) / 1000000.0F ) * 1000;

  if (path==NULL){cout<<"error";}
  else{
    cout<<"To monopati apotelite apo "<<size[3]<<" bimata\nXronos upologismou "<<diff<<" s\n";
    i=0;
    while(i<size[3]){
      j=0;
      while(j<2){
        cout<<path[i][j]<<" ";
        j++;}
      cout<<endl;
      i++;}
}
  return 0;
  }
