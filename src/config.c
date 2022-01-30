#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/config.h"


int read_int_from_config_line(char* config_line) {    
    char prm_name[64];
    int val;
    sscanf(config_line, "%s %d\n", prm_name, &val);
    return val;
}

float read_float_from_config_line(char* config_line) {    
    char prm_name[64];
    float val;
    sscanf(config_line, "%s %f\n", prm_name, &val);
    return val;
}

void read_str_from_config_line(char* config_line, char* val) {    
    char prm_name[64];
    sscanf(config_line, "%s %s\n", prm_name, val);
}


void read_config_file(char* config_filename, struct Config_struct *config) 
{
    FILE *timac;
    char buf[100];
    if ((timac=fopen(config_filename, "r")) == NULL) 
    {
        fprintf(stderr, "Failed to open config file %s", config_filename);
        exit(EXIT_FAILURE);
    }

    while(! feof(timac)) 
    {
        fgets(buf, 100, timac);
    
        if (strstr(buf, "heightmap_file ")) {
            read_str_from_config_line(buf,config->heightmap_file);
        }
        if (strstr(buf, "xsize ")) {
            config->xsize=read_int_from_config_line(buf);
        }
        if (strstr(buf, "ysize ")) {
            config->ysize=read_int_from_config_line(buf);
        }
        if (strstr(buf, "zmin ")) {
            config->zmin=read_float_from_config_line(buf);
        }
        if (strstr(buf, "zmax ")) {
            config->zmax=read_float_from_config_line(buf);
        }
        if (strstr(buf, "znear ")) {
            config->znear=read_int_from_config_line(buf);
        }
        if (strstr(buf, "zfar ")) {
            config->zfar=read_int_from_config_line(buf);
        }
        if (strstr(buf, "fov ")) {
            config->fov=read_int_from_config_line(buf);
        }
    }

}


