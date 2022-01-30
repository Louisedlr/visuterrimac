#ifndef _CONFIG_H
#define _CONFIG_H

struct Config_struct { 

    char heightmap_file[64];
    int xsize;
    int ysize;
    float zmin;
    float zmax;
    int znear;
    int zfar;
    int fov;
} ;

int read_int_from_config_line(char* config_line);
float read_float_from_config_line(char* config_line);
void read_str_from_config_line(char* config_line, char* val);
void read_config_file(char* config_filename, struct Config_struct *config);


#endif