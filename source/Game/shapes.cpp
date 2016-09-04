#include <GL/glut.h>
using namespace std;

void Cube( float x1, float y1, float x2, float y2, float h ) {
    
    glBegin(GL_QUADS);
    /* Front Face */
    glTexCoord2f( 0.0, 1 );
    glVertex3f( x1, y1, h );
    glTexCoord2f( 1, 1 );
    glVertex3f(  x2, y1, h );
    glTexCoord2f( 1, 0.0 );
    glVertex3f(  x2,  y2, h );
    glTexCoord2f( 0.0, 0.0 );
    glVertex3f( x1,  y2, h );
    
    /* Back Face */
    glTexCoord2f( 0.0, 0.0 );
    glVertex3f( x1, y1, 0 );
    glTexCoord2f( 0.0, 1 );
    glVertex3f( x1,  y2, 0 );
    glTexCoord2f( 1, 1 );
    glVertex3f(  x2,  y2, 0 );
    glTexCoord2f( 1, 0.0 );
    glVertex3f(  x2, y1, 0 );
    
    /* Top Face */
    glTexCoord2f( 1, 1 );
    glVertex3f( x1, y2, 0 );
    glTexCoord2f( 1, 0.0 );
    glVertex3f( x1,  y2,  h );
    glTexCoord2f( 0.0, 0.0 );
    glVertex3f(  x2,  y2,  h );
    glTexCoord2f( 0.0, 1 );
    glVertex3f(  x2, y2, 0 );
    
    /* Bottom Face */
    glTexCoord2f( 0.0, 1 );
    glVertex3f( x1, y1, 0 );
    glTexCoord2f( 1, 1 );
    glVertex3f(  x2, y1, 0 );
    glTexCoord2f( 1, 0.0 );
    glVertex3f(  x2, y1,  h );
    glTexCoord2f( 0.0, 0.0 );
    glVertex3f( x1, y1, h );
    
    /* Right face */
    glTexCoord2f( 0.0, 0.0 );
    glVertex3f( x2, y1, 0 );
    glTexCoord2f( 0.0, 1 );
    glVertex3f( x2,  y2, 0 );
    glTexCoord2f( 1, 1 );
    glVertex3f( x2,  y2,  h );
    glTexCoord2f( 1, 0.0 );
    glVertex3f( x2, y1,  h );
    
    /* Left Face */
    glTexCoord2f( 1, 0.0 );
    glVertex3f( x1, y1, 0 );
    glTexCoord2f( 0.0, 0.0 );
    glVertex3f( x1, y1,  h );
    glTexCoord2f( 0.0, 1 );
    glVertex3f( x1,  y2,  h );
    glTexCoord2f( 1, 1 );
    glVertex3f( x1,  y2, 0 );
    glEnd( );}

void Pyramid (){
                int AAngle = 270; // angle variable for rotation
                int X = 1, Y = 0, Z = 0;
                const GLfloat red[3] = {1,0,0};
                const GLfloat green[3] = {0,1,0};
                glPushMatrix();
                glColorMaterial(GL_FRONT, GL_DIFFUSE);
                glEnable(GL_COLOR_MATERIAL);
                glColor3fv(red);
                glRotatef(AAngle, X,Y,Z);
                glBegin(GL_TRIANGLES);
                glNormal3f(1,0,1); 
                // first triangle
                glVertex3f(0.2,0,0.2); //
                glNormal3f(0,0.4,0); //
                glVertex3f(0,0.4,0); //
                glNormal3f(-0.2,0,0.2); //
                glVertex3f(-0.2,0,0.2); //
                glNormal3f(-0.2,0,0.2); 
                // second triangle
                glVertex3f(-0.2,0,0.2); //
                glNormal3f(0,0.4,0); //
                glVertex3f(0,0.4,0); //
                glNormal3f(-0.2,0,-0.2); //
                glVertex3f(-0.2,0,-0.2); //
                glNormal3f(-0.2,0,-0.2); 
                // third triangle
                glVertex3f(-0.2,0,-0.2); //
                glNormal3f(0,0.4,0); //
                glVertex3f(0,0.4,0); //
                glNormal3f(0.2,0,-0.2); //
                glVertex3f(0.2,0,-0.2); //
                glNormal3f(0.2,0,-0.2); 
                // last triangle
                glVertex3f(0.2,0,-0.2); // 
                glNormal3f(0,0.4,0); //
                glVertex3f(0,0.4,0); //
                glNormal3f(0.2,0,0.2); //
                glVertex3f(0.2,0,0.2); //
                glEnd();
                
                /* we also have to draw a square for the bottom of the pyramid so that as it rotates
                we wont be able see inside of it but all a square is is two triangle put together*/
                glColor3fv(green);
                glBegin(GL_TRIANGLES);
                glNormal3f(-0.2,0,0.2);
                glVertex3f(-0.2,0,0.2);
                glNormal3f(-0.2,0,-0.2);
                glVertex3f(-0.2,0,-0.2);
                glNormal3f(0.2,0,0.2);
                glVertex3f(0.2,0,00.2);
                glNormal3f(0.2,0,0.2);
                glVertex3f(0.2,0,0.2);
                glNormal3f(-0.2,0,-0.2);
                glVertex3f(-0.2,0,-0.2);
                glNormal3f(0.2,0,-0.2);
                glVertex3f(0.2,0,-0.2);
                glEnd();
                glColor3f(1.0,1.0,1.0);
                glDisable(GL_COLOR_MATERIAL);
                glPopMatrix();}
