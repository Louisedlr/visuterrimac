#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "../include/quadtree.h"
#include "../include/geometrie.h"

#include "../include/draw.h"


struct Frustum frustum;

//Produit vectoriel
Vect cross(Vect u1, Vect u2)
{
	Vect v;
	v.x = u1.y * u2.z - u1.z * u2.y ;
	v.y = u2.x * u1.z - u2.z * u1.x ;
	v.z = u1.x * u2.y - u1.y * u2.x ;
	
	return v;
}


int dot(Vect u1, Vect u2)
{
    return (u1.x * u2.x + u1.y * u2.y + u1.z * u2.z);
}

float distance(Point_float A, Point_float B)
{
    return sqrt(pow(B.x-A.x,2)+pow(B.y-A.y,2)+pow(B.z-A.z,2));
}

Vect createVect(float vect_x, float vect_y, float vect_z){
    Vect vect;
    vect.x=vect_x;
    vect.y=vect_y;
    vect.z=vect_z;
    return vect;
}


Vect createVectFromPoints(Point_float A, Point_float B)
{
    Vect vector;
    vector.x = B.x - A.x;
    vector.y = B.y - A.y;
    vector.z = B.z - A.z;
    return vector;
}





float norm(Vect vect)
{
    return sqrt(vect.x*vect.x + vect.y*vect.y + vect.z*vect.z);
}

// Permet d'obtenir la normale d'une droite en prenant le vecteur up à 0,1,0
Vect getNormale(Vect vecteur)
{
    Vect vect_up = createVect(0.,1.,0.);
    return(normalize(cross(vect_up, vecteur)));
}

Vect normalize(Vect v)
{
    float n = norm(v);
    if (n == 0.)
        return v;
    v.x /= n;
    v.y /= n;
    v.z /= n;
    return v;
}

//permet d'obtenir d et donc l'équatino ax+by+cz+d=0 d'un plan

float getD(Point_float P, Vect normalePlan) //P appartenant au plan dont on connaît la normale
{
    float a = normalePlan.x;
    float b = normalePlan.y;
    float c = normalePlan.z;

    // Le point P appartient au plan donc vérifie l'équation
    float d = - a * P.x - b * P.y - c * P.z;

    return d;

}

//Recherche d'un projeté orthogonal
/*Point_float distancePointPlan(Vect normale, float d)
{

    //Point_float O = createVect(0., 0., 0.); // l'origine 

    //On nomme H le projeté orthogonal de l'origine sur le plan dont on donne la normale en paramètre
    Point_float H;
    H.x = -normale.x * d / (normale.x * normale.x + normale.z * normale.z);
    H.y = 0.;
    H.x = -normale.z * d / (normale.x * normale.x + normale.z * normale.z);

    return H;

}*/

// regarde si le point est du côté interne d'un plan défini par le frustum
int isInside(Vect position, Vect cote, Point_float ptDuPlan)
{
    if (dot(position,getNormale(cote))+ getD(ptDuPlan, getNormale(cote)) <0){
            return 0;
        }
    return 1;
}


int insideFrustum(Point_float P, float fovx_rad, float zfar, Vect camera_pos)
{
    // On définit le frustum comme un triangle vue du dessus, on ne tient pas compte du znear
    // AB sont les points contenus dans le plan far et C est la caméra
    Point_float A ;
    A.x = camera_pos.x - tan(fovx_rad / 2.) * zfar ;
    A.y = 0.;
    A.z = camera_pos.z + zfar ;

    Point_float B ;
    B.x = camera_pos.x + tan(fovx_rad / 2.) * zfar;
    B.y = 0.;
    B.z = zfar;

    Point_float C ;
    C.x = camera_pos.x ;
    C.y = camera_pos.y ;
    C.z = camera_pos.z ;

    Vect AB = createVectFromPoints(A,B);
    Vect BC = createVectFromPoints(B,C);
    Vect CA = createVectFromPoints(C,A);
    Vect toCheckPlan[3]={AB, BC, CA};
    Point_float toCheckPoint[3]={A,B,C};

    Vect OP = createVect(P.x, P.y, P.z);

    int test = 1 ;
    int i = 0;


    while(test && i<3) {
        Vect n = getNormale(toCheckPlan[i]);
        float d = getD(toCheckPoint[i], n);
        test *= isInside(OP, toCheckPlan[i], toCheckPoint[i]);
        i++ ;
    }

    return test;

}

void printPoint(Point_float p)
{
    printf("(%f, %f, %f)", p.x, p.y, p.z);
}

void printVector(Vect v)
{
    printf("(%f, %f, %f)", v.x, v.y, v.z);
}


void drawQuadtreeF(struct Node* node,  float fovx_rad, float zfar, Vect camera_pos)
{

    // A MODIFIER POUR PRENDRE EN COMPTE LE QUADTREE

    if(node==NULL)
    {
        return; 
    }

    Point_float position1 ;
    position1.x = (float) node->pos1.x; 
    position1.y = (float) node->pos1.y;
    position1.z = (float) node->pos1.z ;


    Point_float position2 ;
    position2.x = (float) node->pos2.x; 
    position2.y = (float) node->pos2.y;
    position2.z = (float) node->pos2.z ;

    Point_float position3 ;
    position3.x = (float) node->pos3.x; 
    position3.y = (float) node->pos3.y;
    position3.z = (float) node->pos3.z ;

    Point_float position4 ;
    position4.x = (float) node->pos4.x; 
    position4.y = (float) node->pos4.y;
    position4.z = (float) node->pos4.z ;

    if(isLeaf(node))
    {
        if (insideFrustum (position1, fovx_rad, zfar,  camera_pos) || 
            insideFrustum (position2, fovx_rad, zfar,  camera_pos) ||
            insideFrustum (position3, fovx_rad, zfar,  camera_pos) ||
            insideFrustum (position4, fovx_rad, zfar,  camera_pos) ){
                drawTrianglesFromLeaves(node);
                
            }

    }
    else
    {
        drawQuadtree(node->node1);
        drawQuadtree(node->node2);
        drawQuadtree(node->node3);
        drawQuadtree(node->node4);
        if (insideFrustum (position1, fovx_rad, zfar,  camera_pos) ||
            insideFrustum (position2, fovx_rad, zfar,  camera_pos) ||
            insideFrustum (position3, fovx_rad, zfar,  camera_pos) ||
            insideFrustum (position4, fovx_rad, zfar,  camera_pos) ){
                drawQuadtreeF(node->node1, fovx_rad, zfar,  camera_pos);
                drawQuadtreeF(node->node2, fovx_rad, zfar,  camera_pos);
                drawQuadtreeF(node->node3, fovx_rad, zfar,  camera_pos);
                drawQuadtreeF(node->node4, fovx_rad, zfar,  camera_pos);
            }
    }
}