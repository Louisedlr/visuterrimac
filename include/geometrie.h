#ifndef _GEOMETRIE_H
#define _GEOMETRIE_H



typedef struct Vect
{
    float x;
    float y;
    float z;
} Vect, Point_float;


typedef struct Triangle
{
    Point_float A;
    Point_float B;
    Point_float C;

}Triangle;

typedef struct Frustum
{
    float zfar;
    struct Vect* camera;
    float fovx_rad;
}Frustum;

int dot(Vect u1, Vect u2);
float distance(Vect u1, Vect u2);
Vect createVectFromPoints(Point_float A, Point_float B);
Vect createVect(float vect_x, float vect_y, float vect_z);
int pointInTriangle(Vect point, Triangle triangle);
Vect getNormale(Vect vecteur);
float norm(Vect vect);
Vect normalize(Vect v);
float getD(Point_float P, Vect normalePlan);
int insideFrustum(Point_float P, float fovx_rad, float zfar, Vect camera_pos);
//Point_float distancePointPlan(Vect normale, float d);

void printPoint(Point_float p);
void printVector(Vect v);


Vect createVect(float x,float y,float z);
Vect cross(Vect u1, Vect u2);
void drawQuadtreeF(struct Node* node, float fovx_rad, float zfar, Vect camera_pos);

#endif