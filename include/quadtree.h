#ifndef _QUADTREE_H
#define _QUADTREE_H
#include "../include/config.h"


typedef struct Point
{
    int x;
    int y;
    float z;
} Point;

typedef struct Node
{
    Point pos1; 
    Point pos2;
    Point pos3;
    Point pos4; 
    struct Node *node1;
    struct Node *node2;
    struct Node *node3;
    struct Node *node4;
    
}Nodde;

typedef struct QuadTree
{
    struct Node *root;
}QuadTree;


Point createPoint(int x,int y,float z);
void addMap(const int* data, struct Node *node,Point A,Point B,Point C, Point D,struct Config_struct *config, int w);
int isLeaf(struct Node* node);
void allLeaves(struct Node **leaves,int *countLeaves, struct Node* node);




#endif