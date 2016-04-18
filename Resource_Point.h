#define max_level 3
class Player;

class ResourcePoint{
    int type,quantity,pos[2],max_quantity,level;
    Player* owner;
  public:
    ResourcePoint(Texture*,int,int,int,int);
    bool levelup() {
        if (this->level < max_level) {
            this->level++;
            return true;
        }
        return false;
    };
    int getlevel() {return this->level;};
    int gettype(){return this->type;};
    int getres(){return this->quantity;};
    Player* getowner(){return this->owner;};
    Texture* texture;
    void free(){ this->owner = NULL; };
    void capture(Player* a){ this->level=0; this->owner=a;};
};

ResourcePoint::ResourcePoint(Texture* text,int type,int quantity, int x, int y){
  this->owner=NULL;
  this->type=type;
  this->quantity=quantity;
  this->max_quantity=quantity;
  this->texture = text;
  this->pos[0] = x;
  this->pos[1] = y;

  return;
  }
