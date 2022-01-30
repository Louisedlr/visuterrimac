#ifndef _READ_HEIGHTMAP_H
#define _READ_HEIGHTMAP_H

struct HeightMap { 

    int h;
    int w; 
    int *data_r;
    int *data_g;
    int *data_b;
} HeightMap;


int read_int_from_line(char* config_line);
void read_twoint_from_line(char* config_line, int* str_heigth, int *str_width );
struct HeightMap* read_heightmap(char* config_filename);

#endif