#include <stdlib.h>
#include <math.h>
#include <stdio.h>


#include <SDL2/SDL.h>
#include<SDL2/SDL_image.h>

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>




#include "../include/texture.h"


/*pour charger les images bmp*/
texture load_texture(char* fname){
    SDL_Surface *tex_surf = SDL_LoadBMP(fname);
    if(!tex_surf){
            return 0;
    }
    texture ret;
    glGenTextures(1, &ret);
    glBindTexture(GL_TEXTURE_2D, ret);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex_surf->w, tex_surf->h, 0, GL_BGR,GL_UNSIGNED_BYTE, tex_surf->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(tex_surf);
    return ret;
}


/*pour charger les images png (ne fonctionne pas)*/
texture ReadPNGFromFile (const char *filename)
{
    SDL_Surface *surface = IMG_Load(filename);
    if(!surface) {
        printf(" IMG_Load error \n");
        return;
    }
    texture texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    return texture;
}
