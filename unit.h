
class Unit : public Entity {
public:
    Unit( char* name, int max_life, char* info, Texture* text,int x,int y,int speed,int id );
};

Unit::Unit(char* name, int max_life, char* info, Texture* text,int x,int y,int speed,int id):Entity(name, max_life, info, text, x, y, speed, id) {

return;}
