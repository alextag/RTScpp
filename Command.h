class Command {
public:
    Command(int cmd, int posx, int posy,int direction, int id);
    ~Command(){};
    int getx();
    int gety();
    int get();
    int getdir();
    int getid();
private:
    int posx,posy,direction,cmd,id;
};

Command::Command(int cmd, int posx, int posy, int direction, int id=-1) {
    this->cmd = cmd;
    this->posx = posx;
    this->posy = posy;
    this->direction = direction;
    this->id = id;
    return;
}

int Command::getx(){return this->posx;}
int Command::gety(){return this->posy;}
int Command::get(){return this->cmd;}
int Command::getdir() {return this->direction;}
int Command::getid() {return this->id;}

class Template{
  public:
    string texture,name;
    int id,hp,dpf,def,speed,q[2],*bufs,*units;
    Template(string name,string texture,int* stats,int* quantity,int* bufs,int* units);
    ~Template(){};
};



Template::Template(string name,string texture,int* stats,int* quantity,int* bufs,int* units){
  this->name=name;
  this->texture=texture;
  id=stats[0];
  hp=stats[1];
  dpf=stats[2];
  def=stats[3];
  speed=stats[4];
  int i=0;
  if(quantity[0]!=0){
    this->bufs= new int[quantity[0]];
    while(i<quantity[0]){
      this->bufs[i]=bufs[i];
      i++;
    }}
  i=0;
  if(quantity[1]!=0){
    this->units= new int[quantity[1]];
    while(i<quantity[1]){
      this->units[i]=units[i];
      i++;
    }}
}
