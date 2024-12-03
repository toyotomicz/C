#ifndef POSTSCRIPT_GRAPH_H
#define POSTSCRIPT_GRAPH_H

#include <stdio.h>

// Error codes
#define ERROR_INVALID_PARAMS -1
#define ERROR_FILE_OPERATION -2
#define ERROR_MEMORY_ALLOCATION -3

// Struktura pro definici parametrů grafu
typedef struct {
    double min_x, max_x;
    double min_y, max_y;
    int width, height;
    int x_divisions, y_divisions;
    double *points;
    int num_points;
} GraphParams;

void generate_axis_label(char* buffer, size_t buffer_size, double value);
int validate_graph_params(const GraphParams* params);
void write_ps_header(FILE* ps_file, const GraphParams* params);
void setup_coordinate_system(FILE* ps_file, const GraphParams* params);
void draw_function(FILE* ps_file, const GraphParams* params);
void draw_grid_and_axes(FILE* ps_file, const GraphParams* params);
void label_axes(FILE* ps_file, const GraphParams* params);
int generate_postscript_graph(const GraphParams* params, const char* output_file);

#endif // POSTSCRIPT_GRAPH_H