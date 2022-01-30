#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/quadtree.h"
#include "../include/config.h"
#include "../include/read_heightmap.h"
#include "math.h"



Point createPoint(int x,int y,float z)
{
    Point point; 
    point.x=x;
    point.y=y;
    point.z=z;
    return point;
}


//construction du quadtree
void addMap(const int* data, struct Node *node,Point A,Point B,Point C, Point D,struct Config_struct *config, int w)
{
    float AB = B.x - A.x;
    float AC = C.y - A.y;
    int middle_Ax = ceil(AB/2);
    int middle_Ay = ceil(AC/2);
    int middle_Bx = floor(AB/2);
    int middle_By = floor(AC/2);
   
    if (AB >=2 && AC >=2)
    {
        node->node1 = (struct Node *) malloc(sizeof(struct Node));
        node->node2 = (struct Node *) malloc(sizeof(struct Node));
        node->node3 = (struct Node *) malloc(sizeof(struct Node));
        node->node4 = (struct Node *) malloc(sizeof(struct Node));

        node->node1->pos1=A;
        node->node1->pos2 = createPoint(A.x + middle_Ax,A.y,A.z);
        node->node1->pos3 = createPoint(A.x,A.y +middle_Ay, A.z);
        node->node1->pos4 = createPoint(A.x+middle_Ax, A.y + middle_Ay, A.z);

        node->node2->pos1= createPoint(B.x-middle_Bx,B.y,B.z);
        node->node2->pos2 = B;
        node->node2->pos3 = createPoint(B.x- middle_Bx,B.y+middle_Ay, B.z);
        node->node2->pos4 = createPoint(B.x, B.y +middle_Ay, B.z);

        node->node3->pos1= createPoint(C.x, C.y-middle_By,C.z);
        node->node3->pos2 = createPoint(C.x+middle_Ax,C.y-middle_By,C.z);
        node->node3->pos3 = C;
        node->node3->pos4 = createPoint(C.x+middle_Ax, C.y, C.z);

        node->node4->pos1= createPoint(D.x-middle_Bx,D.y-middle_By,D.z);
        node->node4->pos2 = createPoint(D.x,D.y-middle_By,D.z);
        node->node4->pos3 = createPoint(D.x-middle_Bx,D.y, D.z);
        node->node4->pos4 = D;

        addMap(data,node->node1,node->node1->pos1,node->node1->pos2,node->node1->pos3,node->node1->pos4,config,w);
        addMap(data,node->node2,node->node2->pos1,node->node2->pos2,node->node2->pos3,node->node2->pos4,config,w);
        addMap(data,node->node3,node->node3->pos1,node->node3->pos2,node->node3->pos3,node->node3->pos4,config,w);
        addMap(data,node->node4,node->node4->pos1,node->node4->pos2,node->node4->pos3,node->node4->pos4,config,w);

    }

    else if (AB==2 && AC == 1)
    {
        node->node1 = (struct Node *) malloc(sizeof(struct Node));
        node->node2 = (struct Node *) malloc(sizeof(struct Node));

        node->node1->pos1=A;
        node->node1->pos2 = createPoint(A.x + 1,A.y,A.z);
        node->node1->pos3 = createPoint(A.x,A.y +1, A.z);
        node->node1->pos4 = createPoint(A.x+1, A.y +1, A.z);

        node->node1->pos1.z = config->zmin+data[node->node1->pos1.x + node->node1->pos1.y*w]/255.*(config->zmax-config->zmin);
        node->node1->pos2.z = config->zmin+data[node->node1->pos2.x + node->node1->pos2.y*w]/255.*(config->zmax-config->zmin);
        node->node1->pos3.z = config->zmin+data[node->node1->pos3.x + node->node1->pos3.y*w]/255.*(config->zmax-config->zmin);
        node->node1->pos4.z = config->zmin+data[node->node1->pos4.x + node->node1->pos4.y*w]/255.*(config->zmax-config->zmin);

        node->node2->pos1= createPoint(B.x-1,B.y,B.z);
        node->node2->pos2 = B;
        node->node2->pos3 = createPoint(B.x- 1,B.y+1, B.z);
        node->node2->pos4 = createPoint(B.x, B.y +1, B.z);

        node->node2->pos1.z = config->zmin+data[node->node2->pos1.x + node->node2->pos1.y*w]/255.*(config->zmax-config->zmin);
        node->node2->pos2.z = config->zmin+data[node->node2->pos2.x + node->node2->pos2.y*w]/255.*(config->zmax-config->zmin);
        node->node2->pos3.z = config->zmin+data[node->node2->pos3.x + node->node2->pos3.y*w]/255.*(config->zmax-config->zmin);
        node->node2->pos4.z = config->zmin+data[node->node2->pos4.x + node->node2->pos4.y*w]/255.*(config->zmax-config->zmin);
    }

    else if (AB ==1 && AC==2)
    {
        node->node1 = (struct Node *) malloc(sizeof(struct Node));
        node->node2 = (struct Node *) malloc(sizeof(struct Node));

        node->node1->pos1=A;
        node->node1->pos2 = createPoint(A.x + 1,A.y,A.z);
        node->node1->pos3 = createPoint(A.x,A.y +1, A.z);
        node->node1->pos4 = createPoint(A.x+1, A.y +1, A.z);

        node->node1->pos1.z = config->zmin+data[node->node1->pos1.x + node->node1->pos1.y*w]/255.*(config->zmax-config->zmin);
        node->node1->pos2.z = config->zmin+data[node->node1->pos2.x + node->node1->pos2.y*w]/255.*(config->zmax-config->zmin);
        node->node1->pos3.z = config->zmin+data[node->node1->pos3.x + node->node1->pos3.y*w]/255.*(config->zmax-config->zmin);
        node->node1->pos4.z = config->zmin+data[node->node1->pos4.x + node->node1->pos4.y*w]/255.*(config->zmax-config->zmin);


        node->node2->pos1= createPoint(C.x, C.y-1,C.z);
        node->node2->pos2 = createPoint(C.x+1,C.y-1,C.z);
        node->node2->pos3 = C;
        node->node2->pos4 = createPoint(C.x+1, C.y, C.z);

        node->node2->pos1.z = config->zmin+data[node->node2->pos1.x + node->node2->pos1.y*w]/255.*(config->zmax-config->zmin);
        node->node2->pos2.z = config->zmin+data[node->node2->pos2.x + node->node2->pos2.y*w]/255.*(config->zmax-config->zmin);
        node->node2->pos3.z = config->zmin+data[node->node2->pos3.x + node->node2->pos3.y*w]/255.*(config->zmax-config->zmin);
        node->node2->pos4.z = config->zmin+data[node->node2->pos4.x + node->node2->pos4.y*w]/255.*(config->zmax-config->zmin);

    }
    
    

    else
    {

        node->pos1.z = config->zmin+data[node->pos1.x + node->pos1.y*w]/255.*(config->zmax-config->zmin);
        node->pos2.z = config->zmin+data[node->pos2.x + node->pos2.y*w]/255.*(config->zmax-config->zmin);
        node->pos3.z = config->zmin+data[node->pos3.x + node->pos3.y*w]/255.*(config->zmax-config->zmin);
        node->pos4.z = config->zmin+data[node->pos4.x + node->pos4.y*w]/255.*(config->zmax-config->zmin);

    }


}


int isLeaf(struct Node* node)
{
    if(node->node1 == NULL)
    {
        return 1;
    }
    else{
        return 0;
    }
}



/*fonction qui compte les feuilles (plus utilisé mais nous a servi pour savoir si on avait le bon nombre de feuilles)*/
void allLeaves(struct Node **leaves,int *ptr_countLeaves, struct Node* node)
{
    if(node==NULL)
    {
        return; 
    }
    if(isLeaf(node)==1)
    {
        //leaves[*ptr_countLeaves]= node;
        (*ptr_countLeaves) ++; 
        

        
    }

    else{
        
        allLeaves(leaves, ptr_countLeaves, node->node1);
        allLeaves(leaves, ptr_countLeaves, node->node2);
        allLeaves(leaves, ptr_countLeaves, node->node3);
        allLeaves(leaves, ptr_countLeaves, node->node4);
    }
}

