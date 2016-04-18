#include <stdio.h>
using namespace std;
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <stdlib.h>
#include<algorithm>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include "command.h"
#include "Texture.h"
#include "Astar.h"
#include "Entity.h"
#include "Resource_Point.h"
#include "Player.h"
#include "Button.h"
#include "GEngine.h"



int SCREEN_WIDTH = 1920;
int SCREEN_HEIGHT = 1080;
const int FRAMES_PER_SECOND = 60;
void runMainLoop( int val );
bool initGL();
void update();
void render();
void freespace();
void update_mouse( int x, int y);
void mouse_click( int button, int state, int x, int y );
void renderTexture( GLfloat, GLfloat, Texture* );
void keyhandler(unsigned char key, int x, int y);
void loadSettings( );
void create_map( );
void make_map( );
Texture *mapTexture,*minimapTexture,*testunittexture;
GEngine *GE;
vector<Entity*> selections;
vector<Player*> players; //Player at 0 should be the user.
bool quit=false;
int mouse_x=0,mouse_y=0;
int **world;
Entity*** cosmos;

int main (int argc, char* args[]) {
    loadSettings();

    glutInit( &argc, args );
    glutInitContextVersion( 2, 1 );
    glutInitDisplayMode( GLUT_DOUBLE );
    glutInitWindowSize( SCREEN_WIDTH, SCREEN_HEIGHT );
    glutCreateWindow( "RTSc++" );
    glutFullScreen();
    if ( !initGL() )
    {
        printf( "Unable to initialize graphics library!\n" );
        return 1;
    }

    mapTexture = new Texture();
    mapTexture->loadfile( "resources/map.png" );
    minimapTexture = new Texture();
    minimapTexture->loadfile( "resources/minimap.png" );
    testunittexture  = new Texture();
    testunittexture->loadfile( "resources/unit.png" );

    players.push_back( new Player( "Bob", 0, 1));
    players.at(0)->createu(new Entity("TestUnit", 1000, NULL, testunittexture, 320, 320, 2, 1));
    players.at(0)->createu(new Entity("TestUnit2", 1000, NULL, testunittexture, 320, 384, 2, 2));
    //players.at(0)->createu(new Entity("TestUnit2", 1000, NULL, testunittexture, 180, 180, 2, 3));
    //players.at(0)->createu(new Entity("TestUnit2", 1000, NULL, testunittexture, 64, 250, 2, 4));
    //players.at(0)->createu(new Entity("TestUnit2", 1000, NULL, testunittexture, 250, 64, 2, 5));
   // players.at(0)->createu(new Entity("TestUnit2", 1000, NULL, testunittexture, 128, 250, 2, 6));
    //players.at(0)->createu(new Entity("TestUnit2", 1000, NULL, testunittexture, 250, 128, 2, 7));


    string gui[5];
    gui[0] = "resources/gui.png";
    gui[1] = "resources/minimap_gui.png";
    gui[2] = "resources/filler_gui.png";
    gui[3] = "resources/res_name_gui.png";
    gui[4] = "resources/res_gui.png";
    gui[5] = "resources/ingame_menu_button.png";
    gui[6] = "resources/ingame_menu_button_pressed.png";

    GE = new GEngine(SCREEN_WIDTH,SCREEN_HEIGHT,gui);
    GE->create_map(mapTexture,minimapTexture);
    create_map();
    glutMotionFunc(update_mouse);
    glutPassiveMotionFunc(update_mouse);
    glutMouseFunc(mouse_click);
    glutKeyboardFunc(keyhandler);
    glutDisplayFunc( render );
    glutTimerFunc( 1000/FRAMES_PER_SECOND, runMainLoop, 0 );
    glutMainLoop();

    return 0;
}

void runMainLoop( int val ) {
    if (quit) {glutDestroyWindow(1);return;}
    update();
    render();
    glutTimerFunc( 1000/FRAMES_PER_SECOND, runMainLoop, val );
}

void update() {
    make_map();
    for (unsigned int i=0;i<players.size();i++)
    {
        Player* p = players.at(i);
        Entity** u = p->get_units();
        for (int j=0;j<p->get_count(0);j++)
        {
            u[j]->tick(world,(GE->map_dimensions_x)/16,(GE->map_dimensions_y)/16);
        }
    }
}

void render() {
    glClear( GL_COLOR_BUFFER_BIT );
    GE->display(mouse_x,mouse_y,players,selections);
    glutSwapBuffers();
}

void update_mouse( int x, int y ){
    mouse_x = x;
    mouse_y = y;
    return;
}

void mouse_click( int button, int state, int x, int y ){
    // Bind a function in GE that will be called on each button click.
 if (button==0)
 {
     if (!GE->build_mode) {
        bool new_select = false;
        vector<Entity*> temp = GE->translate(state,x,y,NULL,players,&new_select,&quit);
        if (new_select) { selections = temp;}
     } else {
        // Selection.at(0) is the entity that should build/train
        // GE->build_id is the id of the entity to be built.
     }

 }
else if ((button == 2) && (state==1))
{
    if (!GE->build_mode) {
        int newx = ((x-GE->x-8)/16) ,newy = ((y-GE->y-8)/16);
        if (!selections.empty()) {
            if (selections.size()==1) {
                if (newx<=1) { newx = 2;}
                else if (newx>=GE->map_dimensions_x/16) {newx = GE->map_dimensions_x/16 -1;}
                if (newy<=1) { newy = 2;}
                else if (newy>=GE->map_dimensions_y/16) {newx = GE->map_dimensions_y/16 -1;}
                selections.at(0)->clear_cmds();
                selections.at(0)->clear_curr_cmd();
                selections.at(0)->cmds.push_back(new Command(1,newx,newy,0));
            return;
            }

        int temp = selections.size();
        int counter=0;
        int places[3] = {0, -4, 4};
        if (newx<5) { newx = 5;}
        else if (newx>GE->map_dimensions_x/16-4) { newx = GE->map_dimensions_x/16-4;}
        if (y + 4*(temp%3) > GE->map_dimensions_y/16) { y = GE->map_dimensions_y/16 - 4*(temp%3);}


        for (int i=0;i<temp;i++) {
                selections.at(i)->clear_cmds();
                selections.at(i)->clear_curr_cmd();
                selections.at(i)->cmds.push_back(new Command(1,newx+places[counter], newy + 4*(i/3),0));
                counter++;
                if (counter == 3) { counter = 0;}
            }
        }
    } else {
        GE->build_mode = false;
        GE->build_id = -1;
    }

}
return;}

void keyhandler(unsigned char key, int x, int y) {
    if (key=='q')
    {
        quit = true;
        freespace();
    }
    return;
}

void freespace() {
    delete(mapTexture);
    delete(minimapTexture);
    delete(testunittexture);
    delete(GE);
    delete(players.at(0));
    int i=0;
    while(i<(GE->map_dimensions_x)/16){ delete(world[i]);i++;}
    delete(world);
return;
}

void create_map() {
    world=new int*[(GE->map_dimensions_x)/16];
    int i=0;
    while(i<(GE->map_dimensions_x)/16){
      world[i]=new int[(GE->map_dimensions_y)/16];
      i++;
    }
}

void make_map(){
    int i=0,j;
    while(i<(GE->map_dimensions_x)/16){
      j=0;
      while(j<(GE->map_dimensions_y)/16){
        world[i][j]=0;
        j++;
      }
      i++;
    }

    for (unsigned int i=0;i<players.size();i++)
    {
        Player* p = players.at(i);
        Entity** u = p->get_units();
        for (int j=0;j<p->get_count(0);j++)
        {
            world[u[j]->x][u[j]->y] = u[j]->id;
            world[u[j]->x+1][u[j]->y] = u[j]->id;
            world[u[j]->x][u[j]->y+1] = u[j]->id;
            world[u[j]->x+1][u[j]->y+1] = u[j]->id;
        }
    }
    return;
}

void loadSettings( ) {
    FILE *fin = fopen("settings.ini","r");
        char s[80];
        fscanf(fin,"%s",s);
        int temp;
        fscanf(fin,"%d",&temp);
        SCREEN_WIDTH = temp;
        fscanf(fin,"%d",&temp);
        SCREEN_HEIGHT = temp;
    fclose(fin);

    if ((SCREEN_WIDTH>6000)||(SCREEN_WIDTH<0)) { SCREEN_WIDTH=1680;SCREEN_HEIGHT=1050; }
    if ((SCREEN_HEIGHT>2200)||(SCREEN_HEIGHT<0)) { SCREEN_WIDTH=1680;SCREEN_HEIGHT=1050; }
    return;
}

bool initGL() {
    glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glClearColor( 0.f, 0.f, 0.f, 1.f );
    glEnable( GL_TEXTURE_2D );

    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }

    ilInit();
    ilClearColour( 255, 255, 255, 000 );

    ILenum ilError = ilGetError();
    if( ilError != IL_NO_ERROR )
    {
        printf( "Error initializing DevIL! %s\n", iluErrorString( ilError ) );
        return false;
    }
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    return true;
}

#define stats 5

bool getinfo(vector<Template*> table){
  string name,path;
  int a[stats],i,q[2],*u,*b;//a stats q quantity of units production capabilities(0) and building bufs(1) u list of units b list of buffs
  ifstream file;
  file.open ("Units.txt");
  if (!file.is_open()){cout<<"Error couldn't open file\n";return false;}
  file.ignore(256,']');
  while(file.peek()!=EOF){
    file.ignore(256,']');
    file>>name;
    file.ignore(256,']');
    file>>path;
    i=0;
    while(i<stats){
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
    table.push_back(new Template(name,path,a,q,b,u));
    delete(u);delete(b);}
  file.close();
  file.open ("Buildings.txt");
  if (!file.is_open()){cout<<"Error couldn't open file\n";return false;}
  file.ignore(256,']');
  while(file.peek()!=EOF){
    file.ignore(256,']');
    file>>name;
    file.ignore(256,']');
    file>>path;
    i=0;
    while(i<stats-1){
      file.ignore(256,']');
      file>>a[i];
      i++;}
    a[i]=0;
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
    table.push_back(new Template(name,path,a,q,b,u));
    delete(u);delete(b);}
  file.close();
  return true;
}

