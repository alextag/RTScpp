#define gui_size 7
#define buttons_size 1

class GEngine {
public:
	GEngine( int width, int height, Texture *gui );
	void SetCameraAbs( int camerax, int cameray );
	void SetCamera( int changex, int changey );
	void LoadMap( vector< vector<Texture*> > *map );
	void LoadMinimap( );
	void LoadTileSizes( int sizex, int sizey );

private:
	void blit( Texture* text, GLfloat x, GLfloat y, int sizex, int sizey );
	void draw_rect( GLfloat tlx, GLfloat tly, GLfloat brx, GLfloat bry );
	GLfloat camerax,cameray;
	vector< vector<Texture*> > *map;
	int tilesizex,tilesizey;
	int mapsizex,mapsizey,screen_w,screen_h;
	int mmap_x, mmap_y, mmapgui_x, mmapgui_y;
	int maptominimap, minimaptomap, minimapsize;
	Texture* mmap_tex, mmapguib, mmapguir, mmapguit, mmapguil, gui_bot;
	Button* menubutton;
	
};

GEngine::GEngine ( int width=600, int height=480, vector< Texture* > gui, vector< Button* > buttons ) {
	this->screen_w = width;
	this->screem_h = height;
	this->camerax = 100.f;
	this->cameray = 100.f;
	this->tilesizex = 32;
	this->tilesizey = 32;
	
	this->mapsizex = 0;
	this->mapsizey = 0;
	this->mmap_x = 0;
	this->mmap_y = 0;
	this->mmapgui_x = 0;
	this->mmapgui_y = 0;
	
	if (gui.size() != gui_size) {return NULL;} // Raise error.
	this->map = gui.at(0);
	this->mmap_tex = gui.at(1);
	this->mmapguib = gui.at(2);
	this->mmapguir = gui.at(3);
	this->mmapguit = gui.at(4);
	this->mmapguil = gui.at(5);
	this->gui_bot = gui.at(6);
	if (buttons.size() != buttons_size) {return NULL;} // Raise error.
	this->menubutton = buttons.at(0)
}

void GEngine::SetCameraAbs( int camerax, int cameray ) {
	this->camerax = camerax;
	this->cameray = cameray;
	return;
}

void GEngine::SetCamera( int changex, int changey ) {
	this->camerax = this->camerax + changex;
	this->cameray = this->cameray + changey;
	return;
}

void GEngine::LoadMap( vector< vector<Texture*> > *map ) {
	this->map = map;
	this->mapsizex = this->tilesizex * map->size();
	this->mapsizey = this->tilesizey * map->size();
	return;
}

void GEngine::LoadMinimap() { // Do we even want a minimap?
	this->maptominimap = (this->mapsizey) / (this->screen_h/4);
	this->minimaptomap = (double)1 / this->maptominimap;
	this->minimapsize = this->mapsizey*this->minimaptomap;
	
	int tw = ( this->mmapguit->height*this->minimapsize ) / this->mmapguit->width;
	int th = ( tw*this->mmapguit->height ) / this->mmapguit->width;

	//int guih = this->minimapsize + ((this->mmapguit->height*this->minimapsize)/this->mmapguit->width) + ((this->minimapsize*this->mmapguib->height)/this->mmapguib->width);
}

void GEngine::LoadTileSizes( int sizex, int sizey ) {
	this->tilesizex = sizex;
	this->tilesizey = sizey;
	if (map!=NULL) {
		this->mapsizex = this->tilesizex * map->size();
		this->mapsizey = this->tilesizey * map->size();
	}
	return;
}

void GEngine::blit( Texture* text, GLfloat x, GLfloat y, int sizex = -1, int sizey = -1 ) {
	if ( text->id != 0) {
	
		if (sizex==-1) { sizex = text->width; }
		if (sizey==-1) { sizey = text->height;}
		glLoadIdentity();
		glTranslatef( x, y, 0.f );
		glBindTexture( GL_TEXTURE_2D, text->id );
		glBegin( GL_QUADS );
			glTexCoord2f( 0.f, 0.f ); glVertex2f(   0.f,   0.f );
            glTexCoord2f( 1.f, 0.f ); glVertex2f( sizex,   0.f );
            glTexCoord2f( 1.f, 1.f ); glVertex2f( sizex, sizey );
            glTexCoord2f( 0.f, 1.f ); glVertex2f(   0.f, sizey );
        glEnd();
	}
	return;
}

void GEngine::draw_rect( GLfloat tlx, GLfloat tly, GLfloat brx, GLfloat bry ) {
	glLoadIdentity();
    glTranslatef( this->camerax, this->cameray, 0.f );
    glColor3ub( 0, 0, 255 ); //Make colour a variable too?
    glBegin(GL_LINE_LOOP);
    glVertex2f( tlx, tly );
    glVertex2f( brx, tly );
    glVertex2f( brx, bry );
    glVertex2f( tlx, bry );
    glEnd();
    glColor3ub( 255, 255, 255 );
	return;
}
