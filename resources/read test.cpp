#include <iostream>
#include <fstream>

using namespace std;

int main(){
  string name,path;
  int a[5],i,q[2],*u,*b;//a stats q quantity of units production capabilities(0) and building bufs(1) u list of units b list of buffs
  ifstream file;
  file.open ("Units.txt");
  if (!file.is_open()){cout<<"Error couldn't open file\n";return -1;}
  file.ignore(256,']');
  while(file.peek()!=EOF){
    file.ignore(256,']');
    file>>name;
    file.ignore(256,']');
    file>>path;
    i=0;
    while(i<5){
    file.ignore(256,']');
    file>>a[i];
    i++;}
    file.ignore(256,']');
    file>>q[0];
    i=0;
    if(q[0]!=0){
      b=new int[q[0]];
      while(i<q[0]){
        file>>b[i];
        i++;
      }
    }
    file.ignore(256,']');
    file>>q[1];
    i=0;
    if(q[1]!=0){
      u=new int[q[1]];
      while(i<q[1]){
        file>>u[i];
        i++;
      }
    }

    cout<<name<<endl<<path<<endl<<a[0]<<endl<<a[1]<<endl<<a[2]<<endl<<a[3]<<endl<<a[4]<<endl<<q[0];
    i=0;
    while(i<q[0]){cout<<" "<<b[i];i++;}
    cout<<endl<<q[1];
    i=0;
    while(i<q[1]){cout<<" "<<u[i];i++;}
    cout<<endl;
    delete(u);delete(b);
  }
  file.close();
  file.open ("Buildings.txt");
  if (!file.is_open()){cout<<"Error couldn't open file\n";return -2;}
  file.ignore(256,']');
  while(file.peek()!=EOF){
    file.ignore(256,']');
    file>>name;
    file.ignore(256,']');
    file>>path;
    i=0;
    while(i<4){
      file.ignore(256,']');
      file>>a[i];
      i++;}
    file.ignore(256,']');
    file>>q[0];
    i=0;
    if(q[0]!=0){
      b=new int[q[0]];
      while(i<q[0]){
        file>>b[i];
        i++;
      }
    }
    file.ignore(256,']');
    file>>q[1];
    i=0;
    if(q[1]!=0){
      u=new int[q[1]];
      while(i<q[1]){
        file>>u[i];
        i++;
      }
    }
    cout<<name<<endl<<path<<endl<<a[0]<<endl<<a[1]<<endl<<a[2]<<endl<<a[3]<<endl<<q[0];
    i=0;
    while(i<q[0]){cout<<" "<<b[i];i++;}
    cout<<endl<<q[1];
    i=0;
    while(i<q[1]){cout<<" "<<u[i];i++;}
    delete(u);delete(b);}
  file.close();
  return 0;
}
