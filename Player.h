#define res_count 2

class Player{
    Entity **units,**buildings;
	ResourcePoint **resource_pnts;
	char* name;
    int color,ID,unit_count,resources[res_count],table_size[3];//0: units, 1: building, 2: res points
  public:
    Player(char* c, int clr, int id);
    ~Player(){kill();};
    void gather();
    void kill();
    void killu(int);//kill unit in units[a]
    void killb(int);//kill building in buildings[a]
    void createu(Entity*);//add unit a in units
    void createb(Entity*);//add building a in buildings
    void res_capture(ResourcePoint*);
    void res_free(int);
    ResourcePoint** get_res_points();
    Entity **get_units();
    Entity **get_builds();
    int get_count(int index );
    };

Player::Player(char* c, int clr, int id){
  color=clr;
  name=c;
  ID=id;
  unit_count=table_size[0]=table_size[1]=table_size[2]=0;
  int i=0;
  while(i<res_count){resources[i]=0;i++;}
  }

void Player::gather(){
  int i=0;
  while(i<table_size[2]){
    resources[resource_pnts[i]->gettype()]+=resource_pnts[i]->getres();
    i++;
  }
}

void Player::kill(){
  int i=0;
  while((i<table_size[0])&&(i<table_size[1])&&(i<table_size[2])){
    delete(units[i]);
    delete(buildings[i]);
    resource_pnts[i]->free();
    i++;
  }
  while((i<table_size[1])&&(i<table_size[2])){
    delete(buildings[i]);
    resource_pnts[i]->free();
    i++;
  }
  while((i<table_size[0])&&(i<table_size[2])){
    delete(units[i]);
    resource_pnts[i]->free();
    i++;
  }
  while((i<table_size[0])&&(i<table_size[1])){
    delete(buildings[i]);
    delete(units[i]);
    i++;
  }
  while(i<table_size[1]){
    delete(buildings[i]);
    i++;
  }
  while(i<table_size[2]){
    resource_pnts[i]->free();
    i++;
  }
  while(i<table_size[0]){
    delete(units[i]);
    i++;
  }
  free(units);
  free(buildings);
  free(resource_pnts);
}


void Player::killu(int a){
  table_size[0]--;
  units[a]=units[table_size[0]];
  if(table_size[0]==0){
    free(units);}
  else{
    units=(Entity **)realloc(units,(table_size[0])*sizeof(Entity*));}
}

void Player::killb(int a){
  table_size[1]--;
  buildings[a]=buildings[table_size[1]];
  if(table_size[1]==0){
    free(buildings);}
  else{
    buildings=(Entity **)realloc(buildings,(table_size[1])*sizeof(Entity*));}
}

void Player::createu(Entity* a){
  table_size[0]++;
  if(table_size[0]==1){
    units=(Entity **)malloc(sizeof(Entity*));}
  else{
    units=(Entity **)realloc(units,(table_size[0])*sizeof(Entity*));}
  units[table_size[0]-1]=a;
}

void Player::createb(Entity* a){
    table_size[1]++;
  if(table_size[1]==1){
    buildings=(Entity **)malloc(sizeof(Entity*));}
  else{
    buildings=(Entity **)realloc(buildings,(table_size[1])*sizeof(Entity*));}
  buildings[table_size[1]-1]=a;
}

void Player::res_capture( ResourcePoint* point ) {
  if (!(point->getowner())){
    point->capture(this);
    table_size[2]++;
    if(table_size[2]==1){
      resource_pnts=(ResourcePoint **)malloc(sizeof(ResourcePoint*));}
    else{
      resource_pnts=(ResourcePoint **)realloc(resource_pnts,(table_size[2])*sizeof(ResourcePoint*));}
    resource_pnts[table_size[2]-1]=point;
  }
}

void Player::res_free(int a) {
  table_size[2]--;
  resource_pnts[a]->free();
  resource_pnts[a]=resource_pnts[table_size[2]];
  if(table_size[2]==0){
    free(resource_pnts);}
  else{
    resource_pnts=(ResourcePoint **)realloc(resource_pnts,(table_size[2])*sizeof(ResourcePoint*));}
}

ResourcePoint** Player::get_res_points() { return this->resource_pnts;}
Entity** Player::get_units() { return this->units;}
Entity** Player::get_builds() { return this->buildings;}
int Player::get_count( int index ) { return this->table_size[index];}
