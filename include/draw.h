#ifndef _DRAW_H
#define _DRAW_H


void glDrawRepere(float length);
void drawTrianglesFromLeaves(struct Node *leaves);
void drawQuadtree(struct Node* node);
void drawCube();
drawArbreTerrain(struct Node* node,struct Node **leaves);

#endif