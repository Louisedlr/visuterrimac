#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../include/quadtree.h"
#include "../include/config.h"
#include "../include/read_heightmap.h"



struct QuadTree* quadtree;
struct Config_struct config;
struct HeightMap* heightmap;
struct Node **leaves;
int countLeaves;


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
int countarbre;
int randarbre;


void glDrawRepere(float length) {
	// dessin du repère
	glBegin(GL_LINES);
		glColor3f(1.,0.,0.);
		glVertex3f(0.,0.,0.);
		glVertex3f(length,0.,0.);
		glColor3f(0.,1.,0.);
		glVertex3i(0.,0.,0.);
		glVertex3i(0.,length,0.);
		glColor3f(0.,0.,1.);
		glVertex3i(0.,0.,0.);
		glVertex3i(0.,0.,length);
	glEnd();
}


void drawTrianglesFromLeaves(struct Node *leaves)
{
	glDisable( GL_DEPTH_TEST);

	if (leaves->pos1.z >config.zmax-(config.zmax/3)&& leaves->pos2.z >config.zmax-(config.zmax/3) && leaves->pos3.z >config.zmax-(config.zmax/3))
	{		
		
	 	
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, snow);
	}	

	else if (leaves->pos1.z >config.zmax-(config.zmax/1.7) && leaves->pos2.z >config.zmax-(config.zmax/1.7) && leaves->pos3.z >config.zmax-(config.zmax/1.7))
	{

		

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, rock);
	}
			

	
	else{
		
		glEnable(GL_TEXTURE_2D);
	 	glBindTexture(GL_TEXTURE_2D, herbe);
		
	}

	
	//glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA,GL_ONE);

    glBegin(GL_TRIANGLES);
		glColor3f(1.,1.,1.);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(leaves->pos1.x*config.xsize/heightmap->h,leaves->pos1.y*config.ysize/heightmap->w,leaves->pos1.z);
        glTexCoord2f(1.0f, 0.0f);
		glVertex3f(leaves->pos2.x*config.xsize/heightmap->h,leaves->pos2.y*config.ysize/heightmap->w,leaves->pos2.z);
        glTexCoord2f(1.0f, 1.0f);
		glVertex3f(leaves->pos3.x*config.xsize/heightmap->h,leaves->pos3.y*config.ysize/heightmap->w,leaves->pos3.z);

		
		glEnd();
	
		glBegin(GL_TRIANGLES);
			glColor3f(1.,1.,1.);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(leaves->pos2.x*config.xsize/heightmap->h,leaves->pos2.y*config.ysize/heightmap->w,leaves->pos2.z);
        	glTexCoord2f(0.0f, 0.0f);
			glVertex3f(leaves->pos3.x*config.xsize/heightmap->h,leaves->pos3.y*config.ysize/heightmap->w,leaves->pos3.z);
        	glTexCoord2f(1.0f, 0.0f);
			glVertex3f(leaves->pos4.x*config.xsize/heightmap->h,leaves->pos4.y*config.ysize/heightmap->w,leaves->pos4.z);
    
		
		glEnd();

	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_BLEND);
	glEnable( GL_DEPTH_TEST);



	
		
}


void drawQuadtree(struct Node* node)
{
	
    if(node==NULL)
    {
        return; 
    }

    if(isLeaf(node)==1)
    {		
        drawTrianglesFromLeaves(node);

    }
    else
    {
        drawQuadtree(node->node1);
        drawQuadtree(node->node2);
        drawQuadtree(node->node3);
        drawQuadtree(node->node4);
    }
}


void drawCube()
{	
	glDisable( GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, top);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, 0, 1);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(1,0, 1);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1, 1, 1);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0, 1 ,1);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, left);
	glBegin(GL_QUADS);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1, 0, 1);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1,0, 0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(1, 1, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(1, 1 ,1);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, front);
	glBegin(GL_QUADS);
	glColor3f(1,1,1);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1, 1, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1,1, 1);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, 1, 1);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 1 ,0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, right);
	glBegin(GL_QUADS);
		glColor3f(1,1,1);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, 1, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0,1, 1);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, 0, 1);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0 ,0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, back);
	glBegin(GL_QUADS);
	glColor3f(1,1,1);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, 0, 1);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0,0, 0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(1, 0, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(1, 0 ,1);
    glEnd();

    glBindTexture(GL_TEXTURE_2D,bottom);
	glBegin(GL_QUADS);
		glColor3f(1,1,1);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0, 0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(1,0, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1, 1, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, 1 ,0);
	glColor3f(1,1,1);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glEnable( GL_DEPTH_TEST);
}

void drawArbre()
{	
	// glDisable( GL_DEPTH_TEST);

	// glEnable(GL_TEXTURE_2D);
	
	// glBindTexture(GL_TEXTURE_2D, arbre);

	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glBegin(GL_QUADS);
		glColor3f(1,1,1);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, 0, 1);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(1,0, 1);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1, 1, 1);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0, 1 ,1);
	


		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1, 0, 1);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1,0, 0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(1, 1, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(1, 1 ,1);
	
	glColor3f(1,1,1);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1, 1, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1,1, 1);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, 1, 1);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 1 ,0);

		glColor3f(1,1,1);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, 1, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0,1, 1);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, 0, 1);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0 ,0);

	glColor3f(1,1,1);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, 0, 1);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0,0, 0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(1, 0, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(1, 0 ,1);
  
		glColor3f(1,1,1);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0, 0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(1,0, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1, 1, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, 1 ,0);
	glColor3f(1,1,1);
		glEnd();

	// glDisable(GL_TEXTURE_2D);
	// glDisable(GL_BLEND);
	// glEnable( GL_DEPTH_TEST);
}

void drawArbreTerrain(struct Node* node,struct Node **leaves)
{
	
    if(node==NULL)
    {
        return; 
    }

    if(isLeaf(node)==1)
    {		
		randarbre ++;
		if (randarbre%config.xsize == 0  && node->pos1.z <config.zmax-(config.zmax/1.7) && countarbre<config.xsize)
		{

			leaves[countarbre]=node;
			countarbre ++;
			
		}


    }
    else
    {
        drawArbreTerrain(node->node1,leaves);
        drawArbreTerrain(node->node2,leaves);
        drawArbreTerrain(node->node3,leaves);
        drawArbreTerrain(node->node4,leaves);
    }
}