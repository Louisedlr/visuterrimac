#ifndef _TEXTURE_H
#define _TEXTURE_H





typedef GLuint texture;

/* OpenGL texture info */
struct gl_texture_t
{
  GLuint width;           /* largeur */
  GLuint height;          /* hauteur */

  GLenum format;          /* RVB, RVBA, Luminance, Luminance Alpha */
  GLint  internalFormat;  /* composantes d'un texel */
  GLuint id;

  GLubyte *texels;         /* données de l'image */
};

texture load_texture(char* fname);
texture ReadPNGFromFile (const char *filename);

#endif