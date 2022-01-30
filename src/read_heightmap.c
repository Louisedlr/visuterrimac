#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/read_heightmap.h"



int read_int_from_line(char* config_line) {   
    int val;
    sscanf(config_line,"%d\n", &val);
    return val;
}

void read_twoint_from_line(char* config_line, int* str_heigth, int *str_width ) {    
    sscanf(config_line, "%d %d\n", str_heigth, str_width);
}

struct HeightMap* read_heightmap(char* config_filename) 
{
    struct HeightMap *heightmap;
    FILE *heightmap_file;
    char buf[100];
    if ((heightmap_file=fopen(config_filename, "r")) == NULL) 
    {
        fprintf(stderr, "Failed to open config file %s", config_filename);
        exit(EXIT_FAILURE);
    }

    fgets(buf, 100, heightmap_file);
    fgets(buf, 100, heightmap_file);

    int heigth, weigth;
    fgets(buf, 100, heightmap_file);  
    read_twoint_from_line(buf,&heigth,&weigth);

    heightmap = (struct HeightMap *) malloc(sizeof(struct HeightMap));
    heightmap->data_r=(int *) malloc((heigth*weigth)*sizeof(int));
    heightmap->data_g=(int *) malloc((heigth*weigth)*sizeof(int));
    heightmap->data_b=(int *) malloc((heigth*weigth)*sizeof(int));

    heightmap->h= heigth;
    heightmap->w= weigth;
    int count = 0;

    fgets(buf, 100, heightmap_file);

    while(! feof(heightmap_file)) 
    {
        fgets(buf, 100, heightmap_file);
        heightmap->data_r[count]=read_int_from_line(buf);
        fgets(buf, 100, heightmap_file);
        heightmap->data_g[count]=read_int_from_line(buf);
        fgets(buf, 100, heightmap_file);
        heightmap->data_b[count]=read_int_from_line(buf);
        count ++;
    }
    return heightmap;
}
