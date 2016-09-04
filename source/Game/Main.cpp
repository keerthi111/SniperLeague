#include <GL/glut.h>
#include <iostream>
#include "arena.cpp"
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <vector>
#include "shapes.cpp"
#include "SDL/SDL.h"
#include <SDL/SDL_image.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Net.h"
#ifdef debugmain
  //for debugging main part
#define mdebug(x) std::cout << x
#else
#define mdebug(x)
#endif

#ifndef debugmain
          #define debugmain
#endif
//for debugging network part
#ifdef debugnet  
#define netdebug(x) std::cout << x
#else
#define netdebug(x)
#endif

#ifndef debugnet
          #define debugnet
#endif
using namespace std;
using namespace net;

const float SnipeTime = 1.0f / 120.0f;
const float SendingSpeed = 1.0f / 120.0f;
const float TimeOut = 10.0f;
const int PacketSize = 256;
int BulletsSelected;
//health of the players that is represented with a bar
#define MAX_health 20
//maximum players
#define MAX_players_allowed 4

int c;
int players;
int Bot_number;

float playerRadius = 0.25;

class co_ordinate {
	public:
	    co_ordinate(int x1, int y1 ) {
	        x = x1; y = y1;}
	    int x;
	    int y; };

bool isMouseMoved;
bool Bot_Thread[MAX_players_allowed];
bool display_player[MAX_players_allowed];
int gamestyle;
char** v;

const int N = 20;
float edgeLength = 2.5;
float wallThickness = 1.0;
float arenaHeight = 2.0;

myarena thearena;
wall** arena = thearena.Arena;

float angle_of_view = 0.0f;
GLuint texture;

int myID;
int otherPlayers;

float bulletRadius = 0.05;
float my_x=1;
float my_y=0;
float angle = 0.0f;
float changeInangle;

float prevx,prevy, prevmy_x,prevmy_y, prevBoundx, prevBoundy;

enum Mode{Player,Host};
Mode mode;

class pos {
    public:    
        int my_id;
        bool Bot;
        float x;
        float y;
        int Boundx;
        int Boundy;};

bool visit[MAX_players_allowed][N][N];
pos allPlayers[MAX_players_allowed];

class Bullets {
    public:
        float x;
        float y;
        int Boundx;
        int Boundy;
        float my_x;
        float my_y;
        int exist;
        int my_shooter; };
Bullets bullBulletsRec;
vector<Bullets*> Bullets_shown_on_screen;
struct playerdetails{ int health;  int hit;};
vector<Bullets> PendingBulletsSend;
playerdetails mydetail[MAX_players_allowed];

void reshape(GLint w, GLint h) {
    glViewport(0, 0, w, h);
    double aspect = (double)w/ (double)h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, (double)w / (double)h, 1.0, 1000.0);
    int cam1 = edgeLength/2 + allPlayers[myID].Boundx*edgeLength + allPlayers[myID].x ;
    int cam2 = edgeLength/2 + allPlayers[myID].Boundy*edgeLength + allPlayers[myID].y;
    int cam3 = -5.0;
    int origin1= edgeLength/2 + allPlayers[myID].Boundx*edgeLength + allPlayers[myID].x;
    int origin2 = edgeLength/2 + allPlayers[myID].Boundy*edgeLength + allPlayers[myID].y;
    int origin3 = 1.75;
   gluLookAt (  cam1, cam2, cam3,origin1, origin2, origin3, 0, 0, 1 );}
void initRendering() {
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING); //Enable lighting
    glEnable(GL_NORMALIZE); //Automaticalmy_y normalize normals
    glShadeModel(GL_SMOOTH); //Enable smooth shading
    
    SDL_Surface *TextureImage;
    if ((TextureImage = IMG_Load("surface.jpg"))){
            glGenTextures( 1, &texture );
            glBindTexture( GL_TEXTURE_2D, texture );
            glTexImage2D( GL_TEXTURE_2D, 0, 3, TextureImage->w,
                         TextureImage->h, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, TextureImage->pixels );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );}
            else{
               cout<<"no textures"<<endl;
            }
    if( TextureImage ){SDL_FreeSurface( TextureImage);}
    glBindTexture( GL_TEXTURE_2D, texture);

    //Add ambient light
    GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 0.5f}; //Color (0.2, 0.2, 0.2)
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    
    //Add positioned light
    GLfloat lightColor0[] = {1.0f, 1.0f, 1.0f, 0.5f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos0[] = {0.0f, 0.0f, 2*arenaHeight, 0.2f}; //Positioned at (4, 0, 8)
    GLfloat lightColor0spec[] = {0.0f, 0.0f, 0.0f, 0.5f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    
    //Add directed light
    GLfloat lightColor1[] = {1.0f, 1.0f, 1.0f, 0.5f}; //Color (0.5, 0.2, 0.2)
    //Coming from the direction (-1, 0.5, 0.5)
    GLfloat lightPos1[] = {(N-1)*edgeLength, 0.0f, 2*arenaHeight, 0.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1); 
    //Add directed light
    GLfloat lightColor3[] = {1.0f, 1.0f, 1.0f, 0.5f}; //Color (0.5, 0.2, 0.2)
    GLfloat lightColor3amb[] = {0.0f, 0.0f, 0.0f, 0.0f}; //Color (0.5, 0.2, 0.2)
    GLfloat lightPos3[] = {0.0f, (N-1)*edgeLength, 2*arenaHeight, 0.0f};
    GLfloat lightSpot[] = {0.0f, 0.0f, -1.0f};
    //glLightfv(GL_LIGHT3, GL_AMBIENT, lightColor1);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT3, GL_POSITION, lightPos1);
    
    
    glEnable(GL_LIGHT0); //Enable light0
    glEnable(GL_LIGHT1); //Enable light1
    glEnable(GL_LIGHT3); //Enable light3
 }
void RandomPosition(int i){  allPlayers[i].Boundy = rand()%N;  allPlayers[i].Boundx = rand()%N;} 
int check_Bounds_bullet ( Bullets* checkBullets ) {
    if ( arena[checkBullets->Boundx][checkBullets->Boundy].edge[0] && checkBullets->y <= -(edgeLength-wallThickness-2*bulletRadius)/2 )
        return 0;
    if ( arena[checkBullets->Boundx][checkBullets->Boundy].edge[2] && checkBullets->y >= +(edgeLength-wallThickness-2*bulletRadius)/2 )
        return 0;
    if ( arena[checkBullets->Boundx][checkBullets->Boundy].edge[3] && checkBullets->x <= -(edgeLength-wallThickness-2*bulletRadius)/2 )
        return 0;
    if ( arena[checkBullets->Boundx][checkBullets->Boundy].edge[1] && checkBullets->x >= +(edgeLength-wallThickness-2*bulletRadius)/2 )
        return 0;
    for(int i = 0; i < players ; i++ ) {
        if( i == checkBullets-> my_shooter )
            continue;
        if(checkBullets->Boundx == allPlayers[i].Boundx && checkBullets->Boundy == allPlayers[i].Boundy ) {
            if((allPlayers[i].x - checkBullets->x)*(allPlayers[i].x - checkBullets->x) + (allPlayers[i].y - checkBullets->y)*(allPlayers[i].y - checkBullets->y) < playerRadius*playerRadius){
                
                        mydetail[i].hit = 15;
                        mydetail[i].health --;
                        if (mydetail[i].health == 0){
                            if (i == myID){
                                RandomPosition(i);
                           }
                          else if ( mode == Host && allPlayers[i].Bot){
                                // Delete AI using thread no.
                                display_player[i] = false;
                                Bot_Thread[i] = false;
                               // cout << "Sent Message" << endl;
                           
                            }
                            else{
                                allPlayers[i].Boundx = 0 ;
                                allPlayers[i].Boundy = 0 ;
                                }
                           
                        }
                    return 0;
            }
        }
    }
    mdebug(checkBullets->Boundx << " " << checkBullets->Boundy <<endl;)
    if ( checkBullets->y > +edgeLength/2 ) {
        checkBullets->Boundy += 1;
        checkBullets->y += -edgeLength;
    }
    if ( checkBullets->y < -edgeLength/2 ) {
        checkBullets->Boundy += -1;
        checkBullets->y += +edgeLength;
    }
    if ( checkBullets->x > +edgeLength/2 ) {
        checkBullets->Boundx += 1;
        checkBullets->x += -edgeLength;
    }
    if ( checkBullets->x < -edgeLength/2 ) {
        checkBullets->Boundx += -1;
        checkBullets->x += +edgeLength;
    }
    return 1;}
void check_Bounds () {
    if ( arena[allPlayers[myID].Boundx][allPlayers[myID].Boundy].edge[0] && allPlayers[myID].y <= -(edgeLength-wallThickness-2*playerRadius)/2 )
        allPlayers[myID].y = -(edgeLength-wallThickness-2*playerRadius)/2;
    if ( arena[allPlayers[myID].Boundx][allPlayers[myID].Boundy].edge[2] && allPlayers[myID].y >= +(edgeLength-wallThickness-2*playerRadius)/2 )
        allPlayers[myID].y = +(edgeLength-wallThickness-2*playerRadius)/2;
    if ( arena[allPlayers[myID].Boundx][allPlayers[myID].Boundy].edge[3] && allPlayers[myID].x <= -(edgeLength-wallThickness-2*playerRadius)/2 )
        allPlayers[myID].x = -(edgeLength-wallThickness-2*playerRadius)/2;
    if ( arena[allPlayers[myID].Boundx][allPlayers[myID].Boundy].edge[1] && allPlayers[myID].x >= +(edgeLength-wallThickness-2*playerRadius)/2 )
        allPlayers[myID].x = +(edgeLength-wallThickness-2*playerRadius)/2;
     mdebug("allPlayers[myID].Boundx"<<allPlayers[myID].Boundy<<endl);
    if ( allPlayers[myID].y > +edgeLength/2 ) {
        allPlayers[myID].Boundy += 1;
        allPlayers[myID].y += -edgeLength;
    }
    if ( allPlayers[myID].y < -edgeLength/2 ) {
        allPlayers[myID].Boundy += -1;
        allPlayers[myID].y += +edgeLength;
    }
    if ( allPlayers[myID].x > +edgeLength/2 ) {
        allPlayers[myID].Boundx += 1;
        allPlayers[myID].x += -edgeLength;
    }
    if ( allPlayers[myID].x < -edgeLength/2 ) {
        allPlayers[myID].Boundx += -1;
        allPlayers[myID].x += +edgeLength;
    } } //Doesn't let player cross the arena
void *shoot_Bot ( void *id ) {
        Bullets *bull;
        bull = new Bullets;
        bull->x = prevx;
        bull->y = prevy;
        bull->Boundx = prevBoundx;
        bull->Boundy = prevBoundy;
        bull->my_x = prevmy_x;
        bull->my_y = prevmy_y;
        bull->exist = 1;
        bull->my_shooter = (long)id;
    if (gamestyle == 1){
        Bullets bull2;
        bull2.x = prevx;
        bull2.y = prevy;
        bull2.Boundx = prevBoundx;
        bull2.Boundy = prevBoundy;
        bull2.my_x = prevmy_x;
        bull2.my_y = prevmy_y;
        bull2.exist = 1;
        bull2.my_shooter = (long)id;
        PendingBulletsSend.push_back (bull2);
    }
    Bullets_shown_on_screen.push_back ( bull );
    while ( 1 ) {
        bull->exist = check_Bounds_bullet ( bull );
        if ( bull->exist == 0 ) {
            break;
        }
        bull->x += bull->my_x*0.1;
        bull->y += bull->my_y*0.1;
        usleep (10000);
    }}
int line_of_sight ( pos play1, pos play2 ) {
    float netx1 = edgeLength/2 + edgeLength*play1.Boundx + play1.x;
    float nety1 = edgeLength/2 + edgeLength*play1.Boundy + play1.y;
    float netx2 = edgeLength/2 + edgeLength*play2.Boundx + play2.x;
    float nety2 = edgeLength/2 + edgeLength*play2.Boundy + play2.y;
    int num = 1000;
    float dx = (netx2-netx1)/num;
    float dy = (nety2-nety1)/num;
    float los_x = play1.x;
    float los_y = play1.y;
    int los_Boundx = play1.Boundx;
    int los_Boundy = play1.Boundy;
    for ( int i = 0; i < num; i++ ) {
        if ( arena[los_Boundx][los_Boundy].edge[0] && los_y <= -(edgeLength-wallThickness)/2 )
            return 0;
        if ( arena[los_Boundx][los_Boundy].edge[2] && los_y >= +(edgeLength-wallThickness)/2 )
            return 0;
        if ( arena[los_Boundx][los_Boundy].edge[3] && los_x <= -(edgeLength-wallThickness)/2 )
            return 0;
        if ( arena[los_Boundx][los_Boundy].edge[1] && los_x >= +(edgeLength-wallThickness)/2 )
            return 0;
        los_x += dx;
        los_y += dy;
        if ( los_y > +edgeLength/2 ) {
            los_Boundy += 1;
            los_y += -edgeLength;
        }
        if ( los_y < -edgeLength/2 ) {
            los_Boundy += -1;
            los_y += +edgeLength;
        }
        if ( los_x > +edgeLength/2 ) {
            los_Boundx += 1;
            los_x += -edgeLength;
        }
        if ( los_x < -edgeLength/2 ) {
            los_Boundx += -1;
            los_x += +edgeLength;
        }
    }
    return 1;}
void* AI_Path ( void* id ) {

    int newID = (long)id;
    int pre_fetch = N*N;
    visit[newID][allPlayers[newID].Boundx][allPlayers[newID].Boundy] = 1;
    vector<co_ordinate> path;
    path.push_back( co_ordinate ( allPlayers[newID].Boundx, allPlayers[newID].Boundy ) );
    if ( Bot_Thread[newID] ) {
        for ( int p = 0; p < pre_fetch; p++ ) {
            co_ordinate cur = path.back();
            if ( (cur.x >= N/2 -2 && cur.x <= N/2+1) && (cur.y >= N/2 -2 && cur.y <= N/2+1) )
                break;
            vector<co_ordinate> options;
            if ( arena[cur.x][cur.y].edge[3] == 0 ) {
                if ( cur.x > 0 ) {
                    if ( visit[newID][cur.x-1][cur.y] == 0 ) {
                        options.push_back ( co_ordinate (cur.x-1, cur.y) );
                        mdebug("changing visit"<<endl;)
                    }
                }
            }
            if ( arena[cur.x][cur.y].edge[2] == 0 ) {
                if ( cur.y < N-1 ) {
                    if ( visit[newID][cur.x][cur.y+1] == 0 ) {
                        options.push_back ( co_ordinate (cur.x, cur.y+1) );
                        mdebug("changing visit"<<endl;)
                    }
                }
            }
            if ( arena[cur.x][cur.y].edge[1] == 0 ) {
                if ( cur.x < N-1 ) {
                    if ( visit[newID][cur.x+1][cur.y] == 0 ) {
                        options.push_back ( co_ordinate (cur.x+1, cur.y) );
                        mdebug("changing visit"<<endl;)
                    }
                }
            }
            if ( arena[cur.x][cur.y].edge[0] == 0 ) {
                if ( cur.y > 0 ) {
                    if ( visit[newID][cur.x][cur.y-1] == 0 ) {
                        options.push_back ( co_ordinate (cur.x, cur.y-1) );
                        mdebug("changing visit"<<endl;)
                    }
                }
            }
            int deltax;
            int deltay;
            if ( options.size() != 0 ) {
                int select = rand()%(options.size());
                path.push_back ( co_ordinate ( options[select].x, options[select].y ) );
                visit[newID][options[select].x][options[select].y] = 1;
            }
            else {
                path.pop_back();
                p--;
            }
        }
    }
    int deltax;
    int deltay;
    int iter = 0;
    while ( Bot_Thread[newID]  ) {
        mdebug( iter << " " << path.size () << endl;)
        for ( int i = 0; i < players; i++ ) {
            if ( i == newID )
                continue;
            if ( line_of_sight ( allPlayers[newID], allPlayers[i] ) ) {
                float netx1 = edgeLength/2 + edgeLength*allPlayers[newID].Boundx + allPlayers[newID].x;
                float nety1 = edgeLength/2 + edgeLength*allPlayers[newID].Boundy + allPlayers[newID].y;
                float netx2 = edgeLength/2 + edgeLength*allPlayers[i].Boundx + allPlayers[i].x;
                float nety2 = edgeLength/2 + edgeLength*allPlayers[i].Boundy + allPlayers[i].y;
                float sq = sqrt ( pow ( netx2 - netx1, 2) + pow ( nety2 - nety1, 2 ) );
                prevmy_x = (netx2-netx1)/sq;;
                prevmy_y = (nety2-nety1)/sq;
                prevBoundx = allPlayers[newID].Boundx;
                prevBoundy = allPlayers[newID].Boundy;
                prevx = allPlayers[newID].x;
                prevy = allPlayers[newID].y;
                pthread_t t;
                pthread_create ( &t, NULL, shoot_Bot, (void*)newID );
                
            }

        }
        deltax = path[iter+1].x - path[iter].x;
        deltay = path[iter+1].y - path[iter].y;
        int move_num = edgeLength*100;
        for ( int i = 0; i < move_num; i++ ) {
            allPlayers[newID].x += deltax*0.01;
            allPlayers[newID].y += deltay*0.01;
            if ( allPlayers[newID].x > +edgeLength/2 ) {
                allPlayers[newID].x += -edgeLength;
                allPlayers[newID].Boundx++;
            }
            if ( allPlayers[newID].x < -edgeLength/2 ) {
                allPlayers[newID].x += +edgeLength;
                allPlayers[newID].Boundx--;
            }
            if ( allPlayers[newID].y > +edgeLength/2 ) {
                allPlayers[newID].y += -edgeLength;
                allPlayers[newID].Boundy++;
            }
            if ( allPlayers[newID].y < -edgeLength/2 ) {
                allPlayers[newID].y += +edgeLength;
                allPlayers[newID].Boundy--;
            }
            usleep ( 7500 );
        }
        iter++;
        if ( iter == path.size() - 1 )
            break;
    }

    Bot_Thread[newID] = true;
  //  cout << "Exiting Thread" << endl;
  }
void Comp_Bot(int i){
   for(int j =0; j<N; j++){
       for(int k = 0; k<N;k++){
            visit[i][j][k] = 0;
        }
    }
    allPlayers[i].Bot = true;
    RandomPosition(i);
     mdebug("Random position alloted to player"<<endl);
    pthread_t t1;
     mdebug("New Thread Created With" << allPlayers[i].Boundx <<"  " << allPlayers[i].Boundy << endl;)
    pthread_create ( &t1, NULL, AI_Path, (void*)i );
    display_player[i] = true;
    pthread_detach(t1);}
int DidIWon = 0;
ReliableConnection connection( 0x11223344, TimeOut );
int isGameEnded(){
    if(mydetail[myID].health == 0){
         mdebug("You lose \n";)
        cout << "You lose \n";
        connection.Stop();
        exit(0);
        return 0;}
    int k = players;
    for(int i = 0; i < players; i++){
        if( i != myID){ 
            if(mydetail[i].health == 0){k--;}}}
    
    if(DidIWon > 0 || k==1){
        mdebug( "You won\n";)
        cout << "You won \n";
        connection.Stop();
        exit(0);
        return 1;
    }
    return 2;}
void display() {
    isMouseMoved = !isMouseMoved;
    int bull = isGameEnded();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.6, 0.01, 1000.0);
    gluLookAt ( edgeLength/2 + allPlayers[myID].Boundx*edgeLength + allPlayers[myID].x - my_x*2*edgeLength, edgeLength/2 + allPlayers[myID].Boundy*edgeLength + allPlayers[myID].y - my_y*2*edgeLength, 1.95*(arenaHeight-1.7), 
                edgeLength/2 + allPlayers[myID].Boundx*edgeLength + allPlayers[myID].x, edgeLength/2 + allPlayers[myID].Boundy*edgeLength + allPlayers[myID].y, 1.25*(arenaHeight-1.2),
                0, 0, 1 );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable( GL_TEXTURE_2D );
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture( GL_TEXTURE_2D, texture);
    Cube ( -wallThickness/2, -wallThickness/2, (N)*edgeLength + wallThickness/2, (N)*edgeLength + wallThickness/2, -0.1);
    glTranslatef ( (N/2)*edgeLength, (N/2)*edgeLength, 0.0f );
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);                 // Turn Blending On
    glDisable(GL_DEPTH_TEST);           // Turn Depth Testing Off
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glColor3f(0.0f, 1.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glLoadIdentity();
    glDisable(GL_BLEND);                 // Turn Blending On
    glEnable(GL_DEPTH_TEST);           // Turn Depth Testing Off
    glEnable(GL_TEXTURE_2D);
    for ( int i = 0; i < N; i++ ) {
        for ( int j = 0; j < N; j++ ) {
        	glLoadIdentity();
            glTranslatef ( edgeLength/2 + i*edgeLength, edgeLength/2 + j*edgeLength, 0.0 );
            if ( arena[i][j].edge[0] )
                Cube ( -(edgeLength)/2, -(edgeLength)/2, +(edgeLength)/2, -(edgeLength)/2, arenaHeight);
            if ( arena[i][j].edge[3] )
                Cube ( -(edgeLength+0)/2, -(edgeLength-0)/2, -(edgeLength)/2, +(edgeLength-0)/2, arenaHeight);
            if ( arena[i][j].edge[2] && j == N-1 )
                Cube ( -(edgeLength-0)/2, +(edgeLength+0)/2, +(edgeLength)/2, +(edgeLength-0)/2, arenaHeight);
            if ( arena[i][j].edge[1] )
                Cube ( +(edgeLength+0)/2, -(edgeLength-0)/2, +(edgeLength)/2, +(edgeLength-0)/2, arenaHeight);
        }
    }
  for ( int i = 1; i < N-1; i++ ) {
    for ( int j = 1; j < N-1; j++ ) {
    	if( arena[i][j].edge[0] || arena[i][j].edge[3] || arena[i-1][j-1].edge[1] || arena[i-1][j-1].edge[2]){
    		    glLoadIdentity();
      			glTranslatef ( i*edgeLength, j*edgeLength, 0.0 );
      			Cube ( -wallThickness/2, -wallThickness/2, +wallThickness/2, +wallThickness/2, arenaHeight );
    	}
    }
   }
   glDisable( GL_TEXTURE_2D );
    glLoadIdentity();
    check_Bounds();
    glLoadIdentity();
    
    for (int i = 0 ; i < players ; i++){
        if (display_player[i]){
        	if (mydetail[i].hit > 0){
        		mydetail[i].hit--;
        		glColor3f(1.0,0.0,0.0);}
        	    glTranslatef ( edgeLength/2 + allPlayers[i].Boundx*edgeLength, edgeLength/2 + allPlayers[i].Boundy*edgeLength, 2*playerRadius );
    			glTranslatef ( allPlayers[i].x, allPlayers[i].y, 0.0 );
    			glColor3f(1,1,1);
    			glTranslatef (0,0, 0.1);
	    	    glColor3f(0.0,0.0f,0.0f);
    			glutSolidSphere( playerRadius*.4, 40, 40  );
    			glutWireSphere( playerRadius*.5, 40, 40  );
    			glTranslatef (0,0, -(0.45)*playerRadius);
    			
    			Pyramid();
    			glColor3f(1,0,0);
    			glPushMatrix();
			    glTranslatef (0, 0, -100);
			    glutSolidTorus(0,0,100, 100);
			    glTranslatef (0, 0, 100);
			    glPopMatrix();
			    glTranslatef (0, 0, playerRadius);
			    glPushMatrix();
			    glRotatef(90, 0 , 1 , 0);
			    glColor3f(1 , 1, 1);
			    glColor3f(1 , 0,0);
			    glTranslatef (0.3, 0.1, 0.27);
			    Cube((-1)*playerRadius, -0.03, (-1)*playerRadius + (2*mydetail[i].health*playerRadius)/(MAX_health), 0.03, 0.03);
			    glPopMatrix();
			    glPopMatrix();
				glLoadIdentity();
            glColor3f(1.0f, 1.0f, 1.0f);
        }
        else{
            if (Bot_Thread[i]){
             
                mydetail[i].health = 0;
                        //create New AI thread with old ID.
            }


        }}
    int bulletLoop = 0;
    while ( bulletLoop < Bullets_shown_on_screen.size () ) {
        if ( Bullets_shown_on_screen[bulletLoop]->exist == 1 ) {
            glTranslatef ( edgeLength/2 + Bullets_shown_on_screen[bulletLoop]->Boundx*edgeLength, edgeLength/2 + Bullets_shown_on_screen[bulletLoop]->Boundy*edgeLength, 2*playerRadius );
            glTranslatef ( Bullets_shown_on_screen[bulletLoop]->x, Bullets_shown_on_screen[bulletLoop]->y, 0.0 );
            glutSolidSphere ( bulletRadius, 10, 10 );
            glLoadIdentity();
        }
        else {
            Bullets_shown_on_screen.erase( Bullets_shown_on_screen.begin() + bulletLoop );
            bulletLoop--;
        }
        bulletLoop++;}
    glEnable(GL_DEPTH_TEST);            // Turn Depth Testing Off
    glEnable( GL_TEXTURE_2D );
    glEnable(GL_LIGHTING);
    glFlush();
    glutSwapBuffers();}
void timer(int v) {
    angle_of_view += 0.0f;
    if (angle_of_view > 360) {
        angle_of_view -= 360;
    }
    if (1) {
        glutPostRedisplay();
    }
    glutTimerFunc(30, timer, v);}   
void keyboard(int key, int x, int y){
	switch (key) {
	        case GLUT_KEY_LEFT :
	            allPlayers[myID].x -= 0.1*my_y;
	            allPlayers[myID].y += 0.1*my_x;
	            break;
	        case GLUT_KEY_RIGHT :
	            allPlayers[myID].x += 0.1*my_y;
	            allPlayers[myID].y -= 0.1*my_x;
	            break;
	        case GLUT_KEY_UP :
	            allPlayers[myID].x += 0.1*my_x;
	            allPlayers[myID].y += 0.1*my_y;
	            break;
	        case GLUT_KEY_DOWN :
	            allPlayers[myID].x += -0.1*my_x;
	            allPlayers[myID].y += -0.1*my_y;
	            break;
		}}
void *shoot ( void *id ) {
    Bullets *bull;

    bull = new Bullets;
    bull->x = prevx;
    bull->y = prevy;
    bull->Boundx = prevBoundx;
    bull->Boundy = prevBoundy;
    bull->my_x = prevmy_x;
    bull->my_y = prevmy_y;
    bull->exist = 1;
    bull->my_shooter = myID;
    if (gamestyle == 1){
        Bullets bull2;
        bull2.x = prevx;
        bull2.y = prevy;
        bull2.Boundx = prevBoundx;
        bull2.Boundy = prevBoundy;
        bull2.my_x = prevmy_x;
        bull2.my_y = prevmy_y;
        bull2.exist = 1;
        bull2.my_shooter = myID;
        PendingBulletsSend.push_back (bull2);
    }
    Bullets_shown_on_screen.push_back ( bull );
    while ( 1 ) {
        bull->exist = check_Bounds_bullet ( bull );
        if ( bull->exist == 0 ) {
            break;
        }
        bull->x += bull->my_x*0.1;
        bull->y += bull->my_y*0.1;
        usleep (8000);
    }}
void *shoot_ofOtherPlayers ( void *id ) {
    Bullets *bull_rec;
    bull_rec = new Bullets;
    bull_rec = &bullBulletsRec;
    bull_rec->exist = 1;
    Bullets_shown_on_screen.push_back ( bull_rec );
    while ( 1 ) { bull_rec->exist = check_Bounds_bullet ( bull_rec );
                 if ( bull_rec->exist == 0 ) {
                 break;}
        bull_rec->x += bull_rec->my_x*0.1;
        bull_rec->y += bull_rec->my_y*0.1;
        usleep (10000);
    }}
void bullet_is_shot( int button, int state, int x, int y) {
    if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ){
        prevmy_x = my_x;
        prevmy_y = my_y;
        prevBoundx = allPlayers[myID].Boundx;
        prevBoundy = allPlayers[myID].Boundy;
        prevx = allPlayers[myID].x;
        prevy = allPlayers[myID].y;
    	pthread_t t;
	    pthread_create ( &t, NULL, shoot, (void*)1);
        }} 
void mouseMove(int x, int y) {

  // this will onmy_y be true when the left button is down
  // update changeInangle
  if (isMouseMoved){
    changeInangle = -(x - 800) * 0.001f;
    // update camera's direction
    my_x = -sin(angle + changeInangle);
    my_y = cos(angle + changeInangle);
    angle += changeInangle;
      glutWarpPointer(800, 450);
    isMouseMoved = !isMouseMoved;
	}}
void *maindisplay ( void *id ) {
    for (int i = 0 ; i < MAX_players_allowed; i++ ){
        for(int j =0; j<N; j++){
            for(int k = 0; k<N;k++){
                visit[i][j][k] = 0;
            }
        }}
    if (gamestyle == 0){  
    	for (int i = 1 ; i < players ; i++){
                Comp_Bot(i);}}
    else if (mode == Host){
        for (int i = otherPlayers ; i < players ; i++ ){
            Comp_Bot(i);
        }}
    glutInit(&c, v);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 200);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("SniperLeague");
    initRendering();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutTimerFunc(100, timer, 0);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutMouseFunc(bullet_is_shot);
    glutPassiveMotionFunc(mouseMove);
    glutSpecialFunc(keyboard);
    glutMainLoop();}

int Main( int argc, char* argv[] ){

    for (int i = 0 ; i < MAX_players_allowed ; i++){
        display_player[i] = true;
        Bot_Thread[i] = true;}
    srand (time(NULL));
    enum GameMode{
        On,
        Off,
        InitializingGame,
        Initialized};
    mode = Host;
    GameMode gamemode = Off;
    Address address[4];
    players = 1;
    int port;
    gamestyle = 1;
    if (argc == 2){
        players += atoi(argv[1]) ;
        gamestyle = 0;}
    else if ( argc == 4 ){ // Player
        int a,b,c,d;
        if ( sscanf( argv[1], "%d.%d.%d.%d", &a, &b, &c, &d ) )
        {
            mode = Player;
            address[0] = Address(a,b,c,d,atoi(argv[2]));
            port = atoi(argv[3]);
        }}
    else if (argc == 5){
        port = atoi(argv[1]);
        address[0] = Address(127,0,0,1,atoi(argv[1]));
        otherPlayers = atoi(argv[2]);
        Bot_number = atoi(argv[3]);}
    
    if (gamestyle == 0) {
        connection.dum_Stop();
            myID = 0;
            otherPlayers = 0 ;
           for (int i = 0 ; i < players ; i++){
                RandomPosition(i);
            }
    	}
    else{
        if ( !connection.Start( port ) ){
           // cout<<"Unable to start connection on port"<< port<<endl; 
            netdebug( "Unable to start connection on port"<< port<<endl; )
            return 1; }
        struct Addrs{ Address add; bool status; };
        struct datasend{ int otherPlayers;  int i; int j; Address address1; Address address2;  Address address3; };
        while (gamemode != On ){
            if(mode == Host){
                myID = 0;
               unsigned char bull[256];
                for ( int i = 0; i < 256; i++ )
                    bull[i] = 0;
               // cout<<"Server : Waiting for Player to Connect"<<endl;
               netdebug("Server : Waiting for Player to Connect"<<endl;)
                while (gamemode == Off){
                    unsigned char packet[256];
                    Address bull;
                    int bytes_read = connection.Receive_Packet( packet, sizeof(packet) ,bull, 0x11223338);
                    if (bytes_read != 0){
                        bool f=false;
                        for (int i = 0 ; i < players ; i++ ){
                            if (i != myID){
                                if (bull == address[i]){
                                    f=true;
                                    break;
                                }
                            }
                        }
                        if (!f){
                            address[players] = bull;
                            players++;
                           // cout<<"New Player is added "<<endl;
                            netdebug("New Player is added "<<endl;)
                        }
                        if (players == otherPlayers){
                            gamemode = InitializingGame;
                        }
                    }}
                Addrs bullorary[3];
                for (int i = 0; i < players-1 ; i++){
                    bullorary[i].add = address[i+1];
                    bullorary[i].status = false;}
                for (int i = 0; i < players-1 ; i++){  bullorary[i].status = false;}
                netdebug("Server : Send addresses of other players"<<endl;)
                while (gamemode == InitializingGame){
                    for (int i = 1 ; i < players ; i++){
                        datasend send;
                        send.i = players + Bot_number;
                        send.j = i ;
                        send.otherPlayers = otherPlayers;
                        send.address1 = address[1];
                        send.address2 = address[2];
                        send.address3 = address[3];
                        
                        connection.Send_Packet( (unsigned char*)&send, sizeof( send ) ,address[i],0x11223341);
                    }
                    unsigned char packet[PacketSize];
                    memset( packet, 0, sizeof( packet ) );
                    Address bull;
                   int bytes_read = connection.Receive_Packet( packet, sizeof(packet), bull,0x11223346 );
                    if ( bytes_read != 0 ){
                        netdebug("Server :Received Codfgggdfon for Player Addresses"<<endl;)
                        for (int i = 0 ; i < players - 1 ; i++ ){
                            if (!bullorary[i].status && bullorary[i].add == bull){
                                netdebug("::"<<i<<endl;)
                                bullorary[i].status = true;
                            }
                        }
                        bool r =true;
                        for (int i = 0 ; i < players - 1 ; i++ ){
                            if (!bullorary[i].status ){
                                r = false;
                                break;
                            }
                        }
                        if (r){
                            gamemode = Initialized;
                            netdebug("Server :Received Confirmation for Player Addresses"<<endl;)
                        }
                    }}
                for (int i = 0; i < players-1 ; i++){ bullorary[i].status = false; }
                while (gamemode == Initialized){
                    for (int j = 1 ; j < players ; j++){
                        for (int i = 0 ; i< 20;i++){
                            unsigned char packet[PacketSize];//send command to start the game to start
                            memset( packet, 0, sizeof( packet ) );
                            connection.Send_Packet( packet, sizeof( packet ),address[j] ,0x11223344); }}
                    netdebug("Server : Start the game"<<endl;)
                    players = otherPlayers + Bot_number;
                    gamemode = On; }}
            else{
	            netdebug("Client :trying to connect"<<endl;)
	            while (gamemode == Off){
	                unsigned char packet[256];
	                connection.Send_Packet( packet, sizeof(packet),address[0],0x11223338 );
	                Address bull ;
	                int bytes_read = connection.Receive_Packet( packet, sizeof(packet) , bull,0x11223341);
	                if ( bytes_read != 0 && bull == address[0] ){
	                    datasend *recData;
	                    recData = new datasend;
	                    recData = (datasend*)&packet;
	                    //delete recData;
	                    players = recData->i;
	                    myID = recData->j;
	                    otherPlayers = recData->otherPlayers;
	                    address[1] = recData->address1;
	                    address[2] = recData->address2;
	                    address[3] = recData->address3;
	                    netdebug( "client is connected to server,received Addresses"<<endl;)
	                    gamemode = InitializingGame;
	                }}
	            netdebug("Client :Send confirmation for Addreses"<<endl;)
	            gamemode = Initialized;
	            netdebug ("Waiting For Confirmation "<<endl;) 
	            while (gamemode == Initialized){

	            	//send confirmation : recieved addresses
	                unsigned char packet1[256];
	                connection.Send_Packet( packet1, sizeof( packet1 ) , address[0],0x11223346);
	               
	                //recieve confirmation for starting the game
	                unsigned char packet[PacketSize];
	                memset( packet, 0, sizeof( packet ) );
	                Address bull;
	                int bytes_read = connection.Receive_Packet( packet, sizeof(packet), bull ,0x11223344);
	                if ( bytes_read != 0 && bull == address[0] ){
	                    netdebug("Client : Start the game"<<endl;)
	                    gamemode = On;
	                }}}}}
    bool connect[4];
    for (int i = 0; i < otherPlayers ; i++){
            connect[i] = true;
            netdebug("Player Number"<<i<< " is port "<<address[i].GetPort()<<endl;)}
   netdebug(otherPlayers << endl;)
   // cout << otherPlayers << endl;
    for (int i = 0 ; i < players ; i++){
        mydetail[i].health = MAX_health;
        mydetail[i].hit=0;
        allPlayers[i].my_id = i;
        allPlayers[i].Bot = false;
        display_player[i] = true;
        Bot_Thread[i] = true;}
    if (gamestyle == 1){
        RandomPosition(myID);
        for (int i = otherPlayers ; i < players ; i++){
            allPlayers[i].Bot = true;
            RandomPosition(i);
        }
        pthread_t disp;
        pthread_create ( &disp, NULL, maindisplay, (void*)1 );
        connection.receive_addresses(address[0],address[1],address[2],address[3]);
        float sendAccumulator = 0.1f;
        while ( true ){
        int detail= 0;
        for (int i = 0; i < otherPlayers; i++){
            if ( i != myID){
            if (!connection.getstate(i)){
                connect[i] = false;
                if (mode == Host){  }
                else if (mode == Player && (myID - i) == 1 ){  mode = Host;  }
                netdebug("Player"<<i<< " disconnected "<<endl; )
            }
            else{
                detail += 1;
            }
            }
        }
        if (!detail){
            netdebug("Everyone is disconnected "<<endl;)
            DidIWon = 1;
            connection.dum_Stop();
            break;
        }
        sendAccumulator += SnipeTime;
        while ( sendAccumulator >  SendingSpeed )
        {
            for ( int k = 0 ; k < PendingBulletsSend.size(); k++){
               	Bullets bull;
                bull = PendingBulletsSend[k];
                for (int i = 0; i < otherPlayers ; i++){
                	if (i!= myID && connect[i]){
        				connection.SendPacket((unsigned char*)&bull, sizeof(bull),address[i],0x11223344,i);
        			}
        		}
        	}
        	PendingBulletsSend.clear();
        	for (int i = 0; i < otherPlayers ; i++){
                	if (i!= myID && connect[i]){
                    	connection.SendPacket( (unsigned char*)&allPlayers[myID], sizeof( allPlayers[myID] ),address[i],0x11223344,i );
                        if (mode == Host){
                        for (int k = 0 ; k < players ; k++){
                            if (allPlayers[k].Bot){
                                connection.SendPacket( (unsigned char*)&allPlayers[k], sizeof( allPlayers[k] ),address[i],0x11223344,i );
                            }
                        }
                        }
                	}
            	}
            sendAccumulator -= SendingSpeed;
        }
        while ( true )
        {
            Address addr;
            unsigned char packet[96];
            int i ;
            int bytes_read = connection.ReceivePacket( packet, sizeof(packet),addr,0x11223344,i);
            if ( bytes_read != 0 ){
            	if (bytes_read == 24){
                pos *recData;
                recData = new pos;
                recData = (pos*)&packet;
                allPlayers[recData->my_id] = *recData;
                //cout<<"received data "<<recData->my_id;
                netdebug("received data "<<recData->my_id;)
            	}
            	else if (bytes_read == 32){
                    netdebug("bullet recieved "<<endl;)
            		Bullets* bull;
            		bull = new Bullets;
            		bull = (Bullets*)&packet;
            		
                    bullBulletsRec.x = bull->x;
            		bullBulletsRec.y = bull->y;
            		bullBulletsRec.Boundx = bull->Boundx;
            		bullBulletsRec.Boundy = bull->Boundy;
            		bullBulletsRec.my_x = bull->my_x;
            		bullBulletsRec.my_y = bull->my_y;
            		bullBulletsRec.my_shooter = bull->my_shooter;
            		
            		pthread_t t1;
        			pthread_create ( &t1, NULL, shoot_ofOtherPlayers, (void*)0);

            	}
            }
            else{
                break;
            }
        }
        
        // update connection
        for (int i = 0; i < otherPlayers ; i++){
            if (i!= myID && connect[i]){
                connection.Update( SnipeTime ,i);
            }
        }
      
        wait_1( SnipeTime );
        }}
    else{ maindisplay((void*)1); }

    return isGameEnded();
}
