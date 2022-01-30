#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#include "../include/config.h"
#include "../include/read_heightmap.h"
#include "../include/quadtree.h"
#include "../include/geometrie.h"
#include "../include/texture.h"
#include "../include/draw.h"

#define STEP_ANGLE	M_PI/40.
#define STEP_PROF	M_PI/40.
#define FPS 60
#define TO_RADIANS M_PI/180.0



/*********************************************************/
/* 				INSTRUCTIONS MOUVEMENT				     */
/*														 */
/* alt + F4 : ferme la fenêtre                           */
/* Z : on avance                                         */
/* S : on recule									     */
/* Q : translation vers la gauche					     */
/* D : translation vers la droite					     */
/* R : rotation autour du point visé				     */
/* position de la souris : on change le point visé       */
/*														 */
/*********************************************************/



struct Motion
{
    int Forward,Backward,Left,Right;
}Motion;

struct Motion motion = {0,0,0,0};
const int width = 1000;
const int height = 1000;

/* variables globales pour la gestion de la caméra */

float theta = M_PI/2.; 
float phi = 0.8;

float camera_pos_x;
float camera_pos_y;
float camera_pos_z;

float camX=0; //permet la translation
float camZ=0;

float profondeur = 13;
float obj_rot = 0.0;

float pitch = -2.2, yaw= -1.;

/* variables globales pour la gestion du quadtree */
struct QuadTree* quadtree;
struct Config_struct config;
struct Config_struct *ptr_config;
struct HeightMap* heightmap;
struct Node **leaves;
GLfloat  h;
int countLeaves;
struct Frustum frustum;
int *ptr_countLeaves;

/* variable pour la texture */
GLuint herbe;
GLuint rock;
GLuint snow;
GLuint back; 
GLuint bottom;
GLuint front; 
GLuint left; 
GLuint right; 
GLuint top;
GLuint arbre;

int countarbre ;
int randarbre;


/*********************************************************/
/* fonction de dessin de la scène à l'écran              */
static void drawFunc(void) { 
	
	//Coordonnées sphériques caméra
	camera_pos_x = cos(phi)*sin(theta);
	camera_pos_y = cos(phi);
	camera_pos_z = sin(theta)* sin(phi);

	Vect camera_pos = createVect(camera_pos_x, camera_pos_y, camera_pos_z);

	//Point visé = position de la caméra + direction de regard
	float vise_x = camera_pos.x+cos(theta)*sin(phi);
	float vise_y = camera_pos.z+sin(theta)*sin(phi);
	float vise_z = camera_pos.y+cos(phi);

	Vect pt_vise = createVect(vise_x, vise_y, vise_z);

	//Vecteur Left
	float left_x=cos(theta+M_PI/2.0);
	float left_y=0.0;
	float left_z=sin(theta+M_PI/2.0);

	Vect left = createVect(left_x, left_y, left_z);

	//Vecteur Up perpendiculaire au Left et Regard
	Vect up = cross(pt_vise,left);

	/* reinitialisation des buffers : couleur et ZBuffer */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	/* modification de la matrice de la scène */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera();
	/* Debut du dessin de la scène */
	glPushMatrix();


	/* Placement de la caméra */

	gluLookAt(profondeur*camera_pos.x,profondeur*camera_pos.y,profondeur*camera_pos.z, vise_x, vise_y, vise_z, 0., 0, 1.);
	
		
	//glDrawRepere(20.0);


	glPushMatrix();
			glTranslatef(-config.xsize/2,-config.ysize/2,-config.zmax);
			glPushMatrix();
	 			glScalef(config.xsize,config.xsize,config.zmax*2);
	 			drawCube();
		 	glPopMatrix();	
	glPopMatrix();
	

	//draw terrain avec arbres 
	glPushMatrix();
	 	glTranslatef(-config.xsize/2,-config.ysize/2,-config.zmax);
		drawQuadtree(quadtree->root);
		for (int i = 1; i <countarbre; i++)
		{
		
			glPushMatrix();
				glTranslatef(leaves[i]->pos1.x*config.xsize/heightmap->h,leaves[i]->pos1.y*config.ysize/heightmap->w,leaves[i]->pos1.z);
				drawArbre();
			glPopMatrix();
	
		}
	glPopMatrix();

	

	float position[4] = {5.0,5.0,5.0,1.0};
	float black[3] = {0.0,0.0,0.0};
	float intensite[3] = {1000.0,1000.0,1000.0};
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_POSITION,position);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,intensite);
	glLightfv(GL_LIGHT0,GL_SPECULAR,black);
	glDisable(GL_LIGHTING);


	
	/* Fin du dessin */
	glPopMatrix();

	/* fin de la définition de la scène */
	glFinish();

	/* changement de buffer d'affichage */
	glutSwapBuffers();
}





/*********************************************************/
/* fonction de changement de dimension de la fenetre     */
/* paramètres :                                          */
/* - width : largeur (x) de la zone de visualisation     */
/* - height : hauteur (y) de la zone de visualisation    */
static void reshapeFunc(int width,int height) {
	GLfloat  h = (GLfloat) width / (GLfloat) height ;
	
	/* dimension de l'écran GL */
	glViewport(0, 0, (GLint)width, (GLint)height);
	/* construction de la matrice de projection */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* définition de la camera */
	float fovx_rad = config.fov*M_PI/180; 
	float fovy_rad=2.0*atan(tan(fovx_rad/2.)*h);
	float fovy_deg=fovy_rad*180.0/M_PI;

	gluPerspective( fovy_deg, h, config.zfar, config.znear);

	/* Retour a la pile de matrice Modelview
			et effacement de celle-ci */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Timer pour obtenir la position de la souris à un intervalle de temps
void timer(int t)
{
    glutPostRedisplay();
    glutWarpPointer(width/2,height/2);
    glutTimerFunc(1000/FPS,timer,0);
}



void keyboard(unsigned char key,int x,int y)
{
    switch(key)
    {
	//zoom
    case 'Z':
    case 'z':
        motion.Forward = 1;
        break;
	//dezoom
	case 'S':
    case 's':
        motion.Backward = 1;
        break;
	//gauche
    case 'Q':
    case 'q':
        motion.Left = 1;
        break;
    //droite
    case 'D':
    case 'd':
        motion.Right = 1;
        break;
	case 'r':
	case 'R':
		phi -= STEP_ANGLE;
		break;
    
	case 'F' : 
	case 'f' : 
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		break;
	case 'P' : 
	case 'p' : 
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		break;
	}
	glutPostRedisplay();
}

void keyboard_up(unsigned char key,int x,int y)
{
    switch(key)
    {
    case 'Z':
    case 'z':
        motion.Forward = 0;
        break;
    case 'Q':
    case 'q':
        motion.Left = 0;
        break;
    case 'S':
    case 's':
        motion.Backward = 0;
        break;
    case 'D':
    case 'd':
        motion.Right = 0;
        break;
    }
	
}


void passive_motion(int x,int y)
{
    /* two variables to store X and Y coordinates, as observed from the center
      of the window
    */
    int dev_x,dev_y;
    dev_x = (width/2)-x;
    dev_y = (width/2)-y;

    /* apply the changes to pitch and yaw*/
    yaw+=(float)dev_x/10.0;
    pitch+=(float)dev_y/10.0;
}

void camera()
{

    if(motion.Forward)
    {
        camX += cos((yaw+90)*TO_RADIANS)/10.0;
        camZ -= sin((yaw+90)*TO_RADIANS)/10.0;
    }
    if(motion.Backward)
    {
        camX += cos((yaw+90+180)*TO_RADIANS)/10.0;
        camZ -= sin((yaw+90+180)*TO_RADIANS)/10.0;
    }
    if(motion.Left)
    {
        camX += cos((yaw+90+90)*TO_RADIANS)/10.0;
        camZ -= sin((yaw+90+90)*TO_RADIANS)/10.0;
    }
    if(motion.Right)
    {
        camX += cos((yaw+90-90)*TO_RADIANS)/10.0;
        camZ -= sin((yaw+90-90)*TO_RADIANS)/10.0;
    }

    /*limit the values of pitch
      between -60 and 70
    */
    if(pitch>=70)
        pitch = 70;
    if(pitch<=-60)
        pitch=-60;

    glRotatef(-pitch,1.0,0.0,0.0); // Along X axis
    glRotatef(-yaw,0.0,1.0,0.0);    //Along Y axis

    glTranslatef(-camX,0.0,-camZ);
}



/*********************************************************/
/* fonction d'initialisation des paramètres de rendu et  */
/* des objets de la scènes.                              */
static void init() {


	/* INITIALISATION DES PARAMETRES GL */
	/* couleur du fond (gris sombre) */
	glClearColor(0.1,0.1,0.1,0.0);
	/* activation du ZBuffer */
	glEnable( GL_DEPTH_TEST);

	/* lissage des couleurs sur les facettes */
	glShadeModel(GL_SMOOTH);

	/* INITIALISATION DE LA SCENE */
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glDepthFunc(GL_LEQUAL);
    glutWarpPointer(width/2,height/2);
	glutSetCursor(GLUT_CURSOR_NONE);

	leaves = (struct Node **) malloc(config.xsize*sizeof(struct Node));
	countarbre =0;
	randarbre= 0;
	// ptr_countLeaves =&countLeaves;
    // countLeaves = 0;
	// allLeaves(leaves,ptr_countLeaves,quadtree->root);


	//Initialisation des textures

	herbe =  load_texture("./texture/grass.bmp");
	rock =  load_texture("./texture/rock.bmp");
	snow =  load_texture("./texture/snow.bmp");
	top = load_texture("./texture/skybox/top.bmp");
	back = load_texture("./texture/skybox/back.bmp");
	bottom = load_texture("./texture/skybox/bottom.bmp");
	front = load_texture("./texture/skybox/front.bmp");
	left = load_texture("./texture/skybox/left.bmp");
	right = load_texture("./texture/skybox/right.bmp");
	arbre = ReadPNGFromFile("./texture/arbre.png");

	//position des arbres à dessiner
	drawArbreTerrain(quadtree->root,leaves);
   
}

void idle(void) {
	obj_rot+=STEP_ANGLE;
	glutPostRedisplay();
}





int main(int argc, char** argv) 
{
    
	
    /*initialisation Quadtree*/
    quadtree = (struct QuadTree *) malloc(sizeof(struct QuadTree));
    quadtree->root = (struct Node *) malloc(sizeof(struct Node));
    ptr_config = &config;
    read_config_file("config.timac", ptr_config);
    heightmap= read_heightmap(config.heightmap_file);

    quadtree->root->pos1 = createPoint(0,0,0.);
    quadtree->root->pos2 = createPoint(heightmap->w-1,0,0.);
    quadtree->root->pos3 = createPoint(0,heightmap->h-1,0.);
    quadtree->root->pos4 = createPoint(heightmap->w-1,heightmap->h-1,0.);

   
    addMap(heightmap->data_r, quadtree->root,quadtree->root->pos1,quadtree->root->pos2,quadtree->root->pos3, quadtree->root->pos4,ptr_config, heightmap->w);
    
	Vect CA = createVect(-6.,0.,6.);
	Point_float A = createVect(-14.,0.,10.);
	Point_float P = createVect(-13.,0.,8.);
	Point_float O = createVect(0.,0.,0.);
	Vect OP = createVectFromPoints(O,P);
	Vect n = getNormale(CA);
	float d = getD(A,n);
	int test = isInside(OP, CA, A);
	//Point_float H = distancePointPlan(n,d);

	
	

	

    /* traitement des paramètres du programme propres à GL */
	glutInit(&argc, argv);
	/* initialisation du mode d'affichage :                */
	/* RVB + ZBuffer + Double buffer.                      */
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
	/* placement et dimentions originales de la fenêtre */
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	/* ouverture de la fenêtre */
	if (glutCreateWindow("TerreIMAC") == GL_FALSE) {
		return 1;
	}
    init();
    //printf("test2");
    

	/* association de la fonction callback d'affichage */
	glutDisplayFunc(drawFunc);

	/* association de la fonction callback de redimensionnement */
	glutReshapeFunc(reshapeFunc);
	
	/* association de la fonction callback d'événement souris */
	glutPassiveMotionFunc(passive_motion);
	glutTimerFunc(0,timer,0); 
    /* association de la fonction callback d'événement du clavier */

	glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboard_up);

	/* boucle principale de gestion des événements */
	glutMainLoop();
	/* Cette partie du code n'est jamais atteinte */
	return 0;    
}
