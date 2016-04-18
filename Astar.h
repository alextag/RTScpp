#define none_value 0
#define move_command 1
#define unit_size 3

class Anode {
public:
    int x,y;
    bool in_o,in_c,can_pass;
    float g_score,f_score;
    Anode* came_from;
    Anode( int, int, bool);
};

Anode::Anode( int x, int y, bool can_pass) {
    this->x = x;
    this->y = y;
    this->g_score = 0.f;
    this->f_score = 0.f;
    this->can_pass = can_pass;
    this->came_from = NULL;
    this->in_o = false;
    this->in_c = false;
return;
}

int cornerize(Anode* start, Anode* stop){
  int res;
  if ((start==NULL) || (stop==NULL)) { return 0;}
  int x = stop->x - start->x;
  int y = stop->y - start->y;
  if(x==0){
    if(y>0){res=90;}
    else{res=-90;}
  }
  else if(x>0&&y>=0){res=atan(y/x)*180.0/3.14159265359;}
  else if(x>0&&y<0){res=270-atan(y/x)*180.0/3.14159265359;}
  else if(x<0&&y>=0){res=180+atan(y/x)*180.0/3.14159265359;}
  else if(x<0&&y<0){res=270-atan(y/x)*180.0/3.14159265359;}
  return res;
}

bool compareNodes(Anode* x, Anode* y){return (x->f_score>y->f_score);}

float heuristic(Anode* start, Anode* dest) {
    return (pow(start->x - dest->x,2) + pow(start->y - dest->y,2));
}

bool real_pass( int** world, int x, int y, int* siz, int obj, int fin ) {
    int i=0,j;
    while (i<unit_size) {
        j=0;
        if (x+i<0) {i++;continue;}
        if (x+i>=siz[0]) {return false;}
        while (j<unit_size) {
            if (y+j<0) {j++;continue;}
            if (y+j>=siz[1]) {return false;}
            if ((world[x+i][y+j]!=none_value) && (world[x+i][y+j]!=obj) && (world[x+i][y+j]!=fin)) {
                return false;
            }
        j++;
        }
    i++;
    }
    return true;
}

int real_pass_helper( int** world, int x, int y, int* siz, int obj ) {
    int i=0,j;
    while (i<unit_size) {
        j=0;
        if (x+i<0) {i++;continue;}
        if (x+i>=siz[0]) {return 0;}
        while (j<unit_size) {
            if (y+j<0) {j++;continue;}
            if (y+j>=siz[1]) {return 0;}
            if ((world[x+i][y+j]!=none_value) && (world[x+i][y+j]!=obj)) {
                return world[x+i][y+j];
            }
        j++;
        }
    i++;
    }
    return 0;
}

Command** path(Anode* current, Anode* start, int *siz, int **world, int obj, int fin) {
    Command **result;
    result = (Command **)malloc(sizeof(Command*));
    if (fin!=0) {
        current = current->came_from;
        while (!real_pass(world, current->x,current->y,siz,obj,0)) { current = current->came_from;}
    }
    int counter = 0;
    result[counter] = new Command(1,current->x,current->y,cornerize(current->came_from,current));
    while (current!=start) {
        current = current->came_from;
        counter++;
        result = (Command **)realloc(result,(counter+1)*sizeof(Command*));
        result[counter] = new Command(1,current->x,current->y,cornerize(current->came_from,current));
    }
    siz[2] = counter;
return result;
}

vector<Anode*> get_neighbors( Anode* current, vector<vector<Anode*> > aworld) {
    int x = current->x, y = current->y;
    vector<Anode*> result;
    for (int i=-1;i<2;i++) {
        if ((x+i>=aworld.size()) || (x+i<0)) {continue;}
        for (int j=-1;j<2;j++) {
            if ((y+j<0) || (y+j>=aworld.at(0).size())) {continue;}
            if (aworld.at(x+i).at(y+j)->can_pass) {
                if (i*j!=0) {
                    if ((aworld.at(x).at(y+j)->can_pass) && (aworld.at(x+i).at(y)->can_pass)) {
                        result.push_back( aworld.at(x+i).at(y+j) );
                        }
                    } else {
                        result.push_back( aworld.at(x+i).at(y+j) );
                    }
                }
            }
        }
return result;}

Command** Astar(int* st, int* dest, int** world, int* siz, int obj) {
    vector<vector<Anode*> > aworld;
    int fin = 0; // Make fin an array?/vector
    if (dest[0] >= siz[0]-4) { dest[0] = siz[0]-5;}
    else if (dest[0] <= 1) { dest[0] = 2;}
    if (dest[1] >= siz[1]-4) { dest[1] = siz[1]-5;}
    else if (dest[1]<=1) { dest[1] = 2;}
    fin = real_pass_helper( world,dest[0],dest[1],siz,obj );
    //cout << "Skipping: " << fin << endl;
    if (real_pass(world,dest[0],dest[1],siz,obj,0) && (!real_pass(world,dest[0],dest[1],siz,0,0))) { return NULL;}
    for (int i=0;i<siz[0];i++) {
        vector<Anode*> temp;
        for (int j=0;j<siz[1];j++) {
            if (real_pass(world,i,j,siz,obj,fin)) {
                temp.push_back(new Anode(i,j,true));
            } else {
                temp.push_back(new Anode(i,j,false));
            }
        }
        aworld.push_back(temp);
        temp.clear();
    }
    if (!aworld.at(dest[0]).at(dest[1])->can_pass) {return NULL;}
    float new_g_score;

    vector<Anode*> cset;
    Anode* start = aworld.at(st[0]).at(st[1]);
    Anode* stop = aworld.at(dest[0]).at(dest[1]);
    Anode* current = NULL;
    vector<Anode*> oset;
    vector<Anode*> neighbors;
    oset.push_back( start );
    start->g_score = 0;
    start->f_score = start->g_score + heuristic(start,stop);

    while (oset.size()>0) {
        sort(oset.begin(), oset.end(), compareNodes);
        current = oset.at(oset.size()-1);
        if (current==stop) {
            aworld.clear();
            cset.clear();
            oset.clear();
            neighbors.clear();
            return path(stop,start,siz,world,obj,fin);
        }
        neighbors = get_neighbors( current, aworld );
        cset.push_back(oset.at(oset.size()-1));
        oset.pop_back();
        current->in_c = true;
        for (unsigned int i=0;i<neighbors.size();i++) {
            new_g_score = 0.f;
            new_g_score = current->g_score + heuristic(current,neighbors.at(i));
            if ((!neighbors.at(i)->in_o) || (new_g_score<neighbors.at(i)->g_score)) {
                neighbors.at(i)->came_from = current;
                neighbors.at(i)->g_score = new_g_score;
                neighbors.at(i)->f_score = new_g_score + heuristic(neighbors.at(i),stop);
                if (!neighbors.at(i)->in_o) {
                    oset.push_back(neighbors.at(i));
                    neighbors.at(i)->in_o = true;
                }
            }
        }
    }
aworld.clear();
cset.clear();
oset.clear();
neighbors.clear();
return NULL;}

