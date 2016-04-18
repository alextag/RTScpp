
class GEngine {
public:
    GEngine ( int width, int height, std::string *paths );
    void add_at_rot ( Texture* text, GLfloat x, GLfloat y, int direction, double fixx, double fixy );
    void add_at ( Texture* text, GLfloat x, GLfloat y );
    void add_at( Texture* text, GLfloat x, GLfloat y, int sizex, int sizey);
    void add_at( Button* b );
    void create_map ( Texture* map_texture, Texture* mmap_texture );
    void create_minimap( Texture* mmap_texture );
    void create_gui();
    void display( int x,int y, vector<Player*> players, vector<Entity*> selection );
    void draw_rect(GLfloat tl, GLfloat tr, GLfloat bl, GLfloat br);
    void draw_rect_rot(GLfloat tl, GLfloat tr, GLfloat bl, GLfloat br, int direction, double fixx, double fixy);
    void make_minimap ( vector<Player*> players);
    void make_gui(); //Selection should be added here
    void make_map( vector<Player*> players, vector<Entity*> selections );
    bool on_button( int x, int y, Button* b );

    vector<Entity*> translate( int state, int x, int y, Entity** world, vector<Player*> players, bool* new_select, bool* quit );
    bool collision( int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
    void update_mouse( int, int );
    ~GEngine ();
    int screen_w;
    int screen_h;
    int map_dimensions_x , map_dimensions_y, map_minimap_ratio, build_id;
    bool build_mode;
    GLfloat x,y;
private:
    bool clicked_minimap, start_selection;
    int start_selection_x, start_selection_y;
    int minimap_x        , minimap_y       , gui_x             , gui_y;
    int minimap_size     , minimap_gui_size, minimap_gui_x     , minimap_gui_y;
    int minimap_box_size_x, minimap_box_size_y;
    int select_tlx       , select_tly      , select_brx        , select_bry;
    int select_w         , select_h;
    int filler_height;
    double minimap_map_ratio;
    Texture *gui_texture,*minimap_gui,*map_texture,*minimap_texture;
    Texture *filler,*res_name,*res_box,*ingame_menu_button, *ingame_menu_button_pressed;
    Button *pressed_button;
    Button *ingame_menu_b;
};

GEngine::GEngine (int width, int height, std::string *paths ) {
    this->x = 100.f;
    this->y = 100.f;
    this->clicked_minimap = false;
    this->gui_texture = new Texture();
    this->gui_texture->loadfile(paths[0]);
    this->minimap_gui = new Texture();
    this->minimap_gui->loadfile(paths[1]);
    this->filler = new Texture();
    this->filler->loadfile(paths[2]);
    this->filler_height = this->filler->height;
    this->res_name = new Texture();
    this->res_name->loadfile(paths[3]);
    this->res_box= new Texture();
    this->res_box->loadfile(paths[4]);
    this->ingame_menu_button = new Texture();
    this->ingame_menu_button->loadfile(paths[5]);
    this->ingame_menu_button_pressed = new Texture();
    this->ingame_menu_button_pressed->loadfile(paths[6]);
    this->start_selection = false;
    this->start_selection_x = -1;
    this->start_selection_y = -1;
    this->map_dimensions_x = 0;
    this->map_dimensions_y = 0;
    this->screen_w = width;
    this->screen_h = height;
    this->pressed_button = NULL;
    this->build_mode = false;
    this->build_id = -1;
    return;
}

GEngine::~GEngine() {
    delete(this->gui_texture);
    delete(this->minimap_gui);
    delete(this->filler);
    delete(this->res_name);
    delete(this->res_box);
    delete(this->ingame_menu_button);
    delete(this->ingame_menu_button_pressed);
    delete(this->ingame_menu_b);
    return;
}

void GEngine::create_map( Texture* map_texture, Texture* mmap_texture ) {
    //Map dimensions defined and texture saved
    this->map_dimensions_x = map_texture->width;
    this->map_dimensions_y = map_texture->height;
    this->map_texture = map_texture;

    this->create_minimap( mmap_texture );
    this->create_gui();
}

void GEngine::create_minimap( Texture* mmap_texture ) {
    //Deciding the size of the minimap and thus its connection to the map
    this->map_minimap_ratio = (this->map_dimensions_y)/(this->screen_h/4);
    this->minimap_map_ratio = (double)1 / this->map_minimap_ratio;
    this->minimap_size = this->map_dimensions_y*this->minimap_map_ratio;

    //Deciding the size of the minimap GUI so that the minimap can fit into it
    this->minimap_gui_size = (this->minimap_size*4)/3;
    this->minimap_gui->width = this->minimap_gui_size;
    this->minimap_gui->height = this->minimap_gui_size;
    this->minimap_texture = mmap_texture;

    //Deciding the size of the box representing the screen on the minimap
    this->minimap_box_size_x= this->screen_w*this->minimap_map_ratio;
        //the minimap is taking about 20% of the screen, so assume that the screen sees the map only above that
    this->minimap_box_size_y=(this->screen_h * 0.80)*this->minimap_map_ratio;
    return;
}

void GEngine::create_gui() {
    //Deciding the position for the minimap GUI
    this->minimap_gui_x = this->screen_w-this->minimap_gui_size;
    this->minimap_gui_y = this->screen_h-this->minimap_gui_size;

    //Deciding the position for the minimap
        //Instead of this, break minimap gui into 4 textures, 2 of them printed on bottom-right corner, minimap next to them and then the other 2.
    int temp = (double)(0.12 * (this->minimap_gui_size));
    this->minimap_x = this->screen_w - this->minimap_size - temp;
    this->minimap_y = this->screen_h - this->minimap_size - temp;

    //Deciding the size and position of action bar according to how big the minimap came to be.
    double factor = (double)(this->minimap_gui_x+2)/this->gui_texture->width;
    this->gui_texture->width = this->screen_w - this->minimap_gui_size;
    this->gui_texture->height = this->gui_texture->height*factor;
    this->gui_x = 0;
    this->gui_y = this->screen_h - this->gui_texture->height;

    //Make Buttons
    this->ingame_menu_b = new Button( this->ingame_menu_button_pressed, this->ingame_menu_button, this->screen_w - this->ingame_menu_button->width, 0 );
    return;
}

void GEngine::make_map( vector<Player*> players, vector<Entity*> selections ) {

    this->add_at(this->map_texture,this->x,this->y);

    for (unsigned int j=0;j<players.size();j++)
    {
        Entity** units = players.at(j)->get_units();
        Entity** buildings = players.at(j)->get_builds();

        for (int i=0;i<players.at(j)->get_count(1);i++)
        {
            this->add_at_rot(buildings[i]->texture,(buildings[i]->truex) + this->x,(buildings[i]->truey) + this->y,buildings[i]->direction,buildings[i]->fixx,buildings[i]->fixy);
        }
        for (int i=0;i<players.at(j)->get_count(0);i++)
        {
            this->add_at_rot(units[i]->texture,(units[i]->truex) + this->x,(units[i]->truey) + this->y,units[i]->direction,units[i]->fixx,units[i]->fixy);
        }
    }

    for (unsigned int i=0;i<selections.size();i++) {
        this->draw_rect_rot( selections.at(i)->truex, selections.at(i)->truey, selections.at(i)->truex+selections.at(i)->sizex, selections.at(i)->truey+selections.at(i)->sizey, selections.at(i)->direction, selections.at(i)->fixx, selections.at(i)->fixy );
    }
    this->make_gui();
    this->make_minimap( players );
    return;
}

void GEngine::make_gui() {
    int tempx=0;
    this->add_at(this->minimap_gui,this->minimap_gui_x,this->minimap_gui_y);
    this->add_at(this->minimap_texture,this->minimap_x,this->minimap_y,this->minimap_size,this->minimap_size);
    this->add_at(this->gui_texture,this->gui_x,this->gui_y);
    this->add_at(this->filler, 0,0, this->screen_w, this->filler->height);

    this->add_at(this->res_name, tempx,0); tempx += this->res_name->width;
    this->add_at(this->res_box, tempx,0); tempx+=this->res_box->width;
    tempx += 10;
    this->add_at(this->res_name, tempx,0); tempx += this->res_name->width;
    this->add_at(this->res_box, tempx,0); tempx+=this->res_box->width;

    //Adding buttons
    this->add_at( this->ingame_menu_b );
    return;
}

void GEngine::make_minimap( vector<Player*> players ) {
    // With a static pre-made minimap, only units & buildings should be painted on the fly, not the whole minimap!
    // And not their actual icons either, a simple minimap is enough.
        glBindTexture( GL_TEXTURE_2D, this->minimap_texture->id );

        for (unsigned int j=0;j<players.size();j++)
        {
            Entity** units = players.at(j)->get_units();
            Entity** buildings = players.at(j)->get_builds();

            for (int i=0;i<players.at(j)->get_count(0);i++)
            {
                this->add_at(units[i]->texture, (units[i]->truex)*this->minimap_map_ratio + this->minimap_x, (units[i]->truey)*this->minimap_map_ratio + this->minimap_y, units[i]->texture->width*this->minimap_map_ratio, units[i]->texture->height*this->minimap_map_ratio );
            }

            for (int i=0;i<players.at(j)->get_count(1);i++)
            {
                this->add_at(buildings[i]->texture, (buildings[i]->truex)*this->minimap_map_ratio + this->minimap_x, (buildings[i]->truey)*this->minimap_map_ratio + this->minimap_y, buildings[i]->texture->width*this->minimap_map_ratio, buildings[i]->texture->height*this->minimap_map_ratio );
            }
        }

        int startingx=this->minimap_x,startingy=this->minimap_y;
        int difx=0,dify=0;
        //cout << startingx << " " << startingy << endl;
        startingx = startingx + ((-1)*(this->x)) + ((-1)*(this->x))*this->minimap_map_ratio;
        startingy = startingy + ((-1)*(this->y)) + ((-1)*(this->y))*this->minimap_map_ratio;

        if (startingx<this->minimap_x+ ((-1)*(this->x))) { difx = this->minimap_x + ((-1)*(this->x)) - startingx; startingx=this->minimap_x+ ((-1)*(this->x));}
        if (startingx+this->minimap_box_size_x>this->minimap_x+ ((-1)*(this->x)) + this->minimap_size) { difx = difx + startingx+this->minimap_box_size_x - this->minimap_x - ((-1)*(this->x)) - this->minimap_size;}

        if (startingy<this->minimap_y+ ((-1)*(this->y))) { dify = this->minimap_y + ((-1)*(this->y)) - startingy; startingy=this->minimap_y+ ((-1)*(this->y));}
        if (startingy+this->minimap_box_size_y>this->minimap_y+ ((-1)*(this->y)) + this->minimap_size) { dify = dify + startingy+this->minimap_box_size_y - this->minimap_y - ((-1)*(this->y)) - this->minimap_size;}

        this->draw_rect(startingx,startingy,startingx+this->minimap_box_size_x - difx,startingy+this->minimap_box_size_y - dify);

    return;
}

void GEngine::update_mouse( int x, int y ) {
    if (this->clicked_minimap)
    {
        if ((x>=this->minimap_x) && (x<=this->minimap_size + this->minimap_x) && (y<=this->minimap_size+this->minimap_y) && (y>=this->minimap_y))
        {
            this->x = ((-1)*(x-this->minimap_x)*this->map_minimap_ratio) + this->screen_w/2;
            this->y = ((-1)*(y-this->minimap_y)*this->map_minimap_ratio) + this->screen_h/2;
        }
    }
    else if (this->start_selection)
    {
        int endx = x - this->x;
        int endy = y - this->y;

        if (endx>=this->start_selection_x)
        {
            this->select_tlx  = this->start_selection_x;
            this->select_brx  = endx;
        } else
        {
            this->select_tlx = endx;
            this->select_brx  = this->start_selection_x;
        }
        if (endy>=this->start_selection_y)
        {
            this->select_tly = this->start_selection_y;
            this->select_bry = endy;
        } else
        {
            this->select_tly = endy;
            this->select_bry = this->start_selection_y;
        }
        this->draw_rect(this->select_tlx,this->select_tly,this->select_brx,this->select_bry);
    }

    if (x<=10) {this->x += 10;}
    else if (x>=this->screen_w-10) {this->x -= 10;}

    if (y<=10) {this->y += 10;}
    else if (y>= this->screen_h-10) {this->y -= 10;}

    if (this->x>this->screen_w/2) {this->x = this->screen_w/2;}
    else if (this->x+this->map_dimensions_x<this->screen_w/2) {this->x = this->screen_w/2 - this->map_dimensions_x;}

    if (this->y>this->screen_h/2) {this->y = this->screen_h/2;}
    else if (this->y+this->map_dimensions_y<this->screen_h/2) {this->y = this->screen_h/2 - this->map_dimensions_y;}
}

void GEngine::display(int x,int y, vector<Player*> players, vector<Entity*> selections) {
    this->make_map( players, selections );
    this->update_mouse(x,y);
}

vector<Entity*> GEngine::translate( int state, int x, int y, Entity** world, vector<Player*> players, bool *new_select, bool *quit ) {

vector<Entity*> selections;
Entity** units = players.at(0)->get_units();
Entity** buildings = players.at(0)->get_builds();
    if (state==1) {
        this->clicked_minimap = false;
    }
    if (this->start_selection) {
        this->select_w = this->select_brx - this->select_tlx;
        this->select_h = this->select_bry - this->select_tly;
        *new_select = true;
        this->start_selection = false;
        bool builds = true;
        for (int i=0;i<players.at(0)->get_count(0);i++) {
            if (this->collision( units[i]->truex, units[i]->truey, units[i]->texture->width, units[i]->texture->height, this->select_tlx, this->select_tly, this->select_w, this->select_h))
            {
                selections.push_back(units[i]);
                builds = false;
            }
        }

        if (builds) {
            for (int i=0;i<players.at(0)->get_count(1);i++) {
                if (this->collision(buildings[i]->truex,buildings[i]->truey,buildings[i]->texture->width,buildings[i]->texture->height,this->select_tlx,this->select_tly,this->select_w,this->select_h))
                    {
                    selections.push_back(buildings[i]);
                    }
            }
        }
    }

if ( (state==0) && (x>=this->minimap_x) && (x<=this->minimap_size+this->minimap_x) && (y<=this->minimap_size+this->minimap_y) && (y>=this->minimap_y))
    {
        this->clicked_minimap = true;
        this->x = ((-1)* (x-this->minimap_x)*this->map_minimap_ratio) + this->screen_w/2;
        this->y = ((-1)* (y-this->minimap_y)*this->map_minimap_ratio) + this->screen_h/2;
    }
else if (y>this->gui_y) {
        std::cout << "In action bar" << std::endl;
    }
else if (y<=this->filler_height) {
    if (this->on_button(x,y,this->ingame_menu_b)) {
            if (state == 0) { this->ingame_menu_b->press(); this->pressed_button = this->ingame_menu_b;}
            else if ((state == 1) && (this->ingame_menu_b->isPressed)) { *quit = true; }

    }
}
else if ((y>= this->minimap_gui_y) && (x>= this->minimap_gui_x)) {
    std::cout << "In minimap gui" << std::endl;
}
else if (state == 0)
    {
        this->start_selection = true;
        this->start_selection_x = x - this->x;
        this->start_selection_y = y - this->y;
    }

if ( state == 1){
    if (this->pressed_button!=NULL) {
        this->pressed_button->release();
        this->pressed_button=NULL;
    }
}


return selections;
}

bool GEngine::collision( int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2 ) {
    if (x1>(x2+w2)) {return false;}
    if (x2>(x1+w1)) {return false;}
    if (y2>(y1+h1)) {return false;}
    if (y1>(y2+h2)) {return false;}
    return true;
}

void GEngine::add_at_rot( Texture* text, GLfloat x, GLfloat y, int direction=0, double fixx=0, double fixy=0) {
    if( text->id != 0 )
    {
        //Remove any previous transformations
        glLoadIdentity();

        //Move to rendering point
        //cout << direction << endl;
        glTranslatef( x + fixx, y + fixy, 0.f );
        glRotatef( direction, 0,0,1);
        //Set texture ID
        glBindTexture( GL_TEXTURE_2D, text->id );
        //Render textured quad
        glBegin( GL_QUADS );
            glTexCoord2f( 0.f, 0.f ); glVertex2f(           0.f,            0.f );
            glTexCoord2f( 1.f, 0.f ); glVertex2f( text->width,            0.f );
            glTexCoord2f( 1.f, 1.f ); glVertex2f( text->width, text->height );
            glTexCoord2f( 0.f, 1.f ); glVertex2f(           0.f, text->height );
        glEnd();
    }
}

void GEngine::add_at( Texture* text, GLfloat x, GLfloat y) {
    if( text->id != 0 )
    {
        //Remove any previous transformations
        glLoadIdentity();

        //Move to rendering point
        //cout << direction << endl;
        glTranslatef( x, y, 0.f );
        //Set texture ID
        glBindTexture( GL_TEXTURE_2D, text->id );
        //Render textured quad
        glBegin( GL_QUADS );
            glTexCoord2f( 0.f, 0.f ); glVertex2f(           0.f,            0.f );
            glTexCoord2f( 1.f, 0.f ); glVertex2f( text->width,            0.f );
            glTexCoord2f( 1.f, 1.f ); glVertex2f( text->width, text->height );
            glTexCoord2f( 0.f, 1.f ); glVertex2f(           0.f, text->height );
        glEnd();
    }
}

void GEngine::add_at( Button* b ) {
    if( b->get_texture()->id != 0 )
    {
        //Remove any previous transformations
        glLoadIdentity();

        //Move to rendering point
        //cout << direction << endl;
        glTranslatef( b->x, b->y, 0.f );
        //Set texture ID
        glBindTexture( GL_TEXTURE_2D, b->get_texture()->id );
        //Render textured quad
        glBegin( GL_QUADS );
            glTexCoord2f( 0.f, 0.f ); glVertex2f(                  0.f,               0.f );
            glTexCoord2f( 1.f, 0.f ); glVertex2f( b->get_texture()->width,            0.f );
            glTexCoord2f( 1.f, 1.f ); glVertex2f( b->get_texture()->width, b->get_texture()->height );
            glTexCoord2f( 0.f, 1.f ); glVertex2f(           0.f, b->get_texture()->height );
        glEnd();
    }
}

void GEngine::add_at( Texture* text, GLfloat x, GLfloat y, int sizex, int sizey) {
    if( text->id != 0 )
    {
        //Remove any previous transformations
        glLoadIdentity();

        //Move to rendering point
        glTranslatef( x, y, 0.f );

        //Set texture ID
        glBindTexture( GL_TEXTURE_2D, text->id );

        //Render textured quad
        glBegin( GL_QUADS );
            glTexCoord2f( 0.f, 0.f ); glVertex2f(   0.f,   0.f );
            glTexCoord2f( 1.f, 0.f ); glVertex2f( sizex,   0.f );
            glTexCoord2f( 1.f, 1.f ); glVertex2f( sizex, sizey );
            glTexCoord2f( 0.f, 1.f ); glVertex2f(   0.f, sizey );
        glEnd();
    }
}

void GEngine::draw_rect(GLfloat tlx, GLfloat tly, GLfloat brx, GLfloat bry) {
    glLoadIdentity();
    glTranslatef( this->x, this->y, 0.f );
    glColor3ub( 0, 0, 255 );
    glBegin(GL_LINE_LOOP);
    glVertex2f( tlx, tly );
    glVertex2f( brx, tly );
    glVertex2f( brx, bry );
    glVertex2f( tlx, bry );
    glEnd();
    glColor3ub( 255, 255, 255 );
return;}

void GEngine::draw_rect_rot(GLfloat tlx, GLfloat tly, GLfloat brx, GLfloat bry, int direction, double fixx, double fixy) {
    glLoadIdentity();
    glTranslatef( this->x + tlx + fixx, this->y + tly + fixy, 0.f );
    glRotatef( direction, 0,0,1);
    glColor3ub( 0, 0, 255 );
    glBegin(GL_LINE_LOOP);
    glVertex2f( 0.f - 4 , 0.f - 4 );
    glVertex2f( brx-tlx+4, 0.f - 4 );
    glVertex2f( brx-tlx+4, bry-tly+4 );
    glVertex2f( 0.f - 4, bry-tly+4 );
    glEnd();
    glColor3ub( 255, 255, 255 );
return;}

bool GEngine::on_button( int x, int y, Button* b) {
    return (x>=b->x) && (x<=b->x+b->sizex) && (y>=b->y) && (y<=b->y+b->sizey);
}
