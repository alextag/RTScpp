#define base_size 16

class Entity{
public:
    Entity(char*,int,char*,Texture*,int,int,int,int,int);
    ~Entity(){};
    char *name,*info;
    int x,y,max_life,id,cur_cmd,max_cmd,sizex,sizey,truex,truey,speed,direction,time;
    double fixx,fixy;
    Texture* texture;
    vector<Command*> cmds;
    Command** currcmd;
    void tick(int** world,int,int);
    void clear_curr_cmd();
    void clear_cmds();
private:
};

Entity::Entity(char* name, int max_life, char* info, Texture* text,int x,int y,int speed,int id,int time=-1){
    this->max_life = max_life;
    this->name = name;
    this->info = info;
    this->truex = x;
    this->truey = y;
    this->x = x/base_size;
    this->y = y/base_size;
    this->texture = text;
    this->id = id;
    this->cur_cmd = -1;
    this->max_cmd = 0;
    this->sizex = this->texture->width;
    this->sizey = this->texture->height;
    this->speed = speed;
    this->direction = 0;
    this->fixx = 0;
    this->fixy = 0;
    this->time = time;
    return;
}

void Entity::tick(int** world,int x, int y) {
    if (this->time >0) {return;}
    Command* next;
    if (cmds.empty()) {return;}

    // If there is a new command to process...
    if ( cur_cmd < 0 ) {
        next = cmds.at(0);


        if (next->get()==1) { // If that command is a movement command.
            if ((next->getx()==this->x) && (next->gety()==this->y)) { cur_cmd = -1; cmds.erase(cmds.begin());return;}
            int A_start[]={this->x,this->y},A_end[]={next->getx(),next->gety()},A_size[] = {x,y,0};
            currcmd = Astar(A_start,A_end,world,A_size,this->id);
            max_cmd = A_size[2];
            cur_cmd = max_cmd;
        } else if (next->get()==2) { //If the command is a build command.

        }
    }


    if ((currcmd==NULL)||(cur_cmd<0)||(cur_cmd>5000)||(max_cmd>5000)) {
        this->clear_curr_cmd();
        cmds.erase(cmds.begin());

        return;
    }

    next = currcmd[cur_cmd];
    bool done=true;
   if (next->get()==1) { // If command == "move"
        this->direction = next->getdir();
        if (this->direction == 0) { this->fixx=0;this->fixy=0;}
        else if (this->direction == 44) {this->fixx=this->texture->width/2;this->fixy=0;}
        else if (this->direction == 90) {this->fixx=this->texture->width;this->fixy=0;}
        else if (this->direction == 135) {this->fixx=this->texture->width;this->fixy=this->texture->height/2;}
        else if (this->direction == 180) {this->fixx=this->texture->width;this->fixy=this->texture->height;}
        else if (this->direction == 225){ this->fixx=this->texture->width/2;this->fixy=this->texture->height;}
        else if (this->direction == -90){ this->fixx=0;this->fixy=this->texture->height;}
        else if (this->direction == 314){ this->fixx=0;this->fixy=this->texture->height/2;}

        int ux=this->truex,uy=this->truey;
        if (this->x > next->getx()) { ux = ux - this->speed;done=false;}
        else if (this->x < next->getx()) { ux = ux + this->speed;done=false;}

        if (this->y > next->gety()) { uy = uy - this->speed;done=false;}
        else if (this->y < next->gety()) { uy = uy + this->speed;done=false;}

        int newx=ux/16,newy=uy/16;


        if ((this->x != newx) || (this->y != newy)) {
        bool collision = false;

        int i=0,j;
        while ((i<3) && (!collision)){
            j=0;
            if(newx+i<0){i++;continue;}
            if(newx+i>=x){i++;continue;}
            while(j<3){
              if(newy+j<0){j++;continue;}
              if(newy+j>=y){j++;continue;}
              if ((world[newx+i][newy+j] != 0) && (world[newx+i][newy+j] != this->id)) {collision=true; break;}
              j++;
            }
            i++;
          }

        if (collision) {
            if (cur_cmd <= 1) {
                this->clear_curr_cmd();
                cmds.erase(cmds.begin());

                return;
            }

            this->clear_curr_cmd();
            int A_start[]={this->x,this->y},A_end[]={cmds.at(0)->getx(),cmds.at(0)->gety()},A_size[] = {x,y,0};
            currcmd = Astar(A_start,A_end,world,A_size,this->id);
            max_cmd=A_size[2];
            cur_cmd = max_cmd;

            return;
            }

            world[this->x][this->y] = 0;
            world[this->x+1][this->y] = 0;
            world[this->x][this->y+1] = 0;
            world[this->x+1][this->y+1] = 0;
            this->x = newx;
            this->y = newy;
            this->truex = ux;
            this->truey = uy;
            world[this->x][this->y] = this->id;
            world[this->x+1][this->y] = this->id;
            world[this->x][this->y+1] = this->id;
            world[this->x+1][this->y+1] = this->id;
        } else {
            this->truex = ux;
            this->truey = uy;
        }

        if (done) {
            cur_cmd--;
            if (cur_cmd<0) {
            cmds.erase(cmds.begin());
            }
        return;
        }
   }
   return;
}

void Entity::clear_curr_cmd() {
    for (int i=0;i<this->max_cmd;i++)
    {
        delete(currcmd[i]);
    }
    delete(currcmd);
    this->cur_cmd = -1;
}

void Entity::clear_cmds() {
    this->cmds.clear();
}
