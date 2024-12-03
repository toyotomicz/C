#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "postscript.h"

// Error codes
#define ERROR_INVALID_PARAMS -1
#define ERROR_FILE_OPERATION -2
#define ERROR_MEMORY_ALLOCATION -3

// Helper function for generating precise axis labels
void generate_axis_label(char* buffer, size_t buffer_size, double value) {
    if (fabs(value) < 0.001) {
        snprintf(buffer, buffer_size, "0.00");
    } else if (fabs(value) >= 1000 || fabs(value) < 0.001) {
        snprintf(buffer, buffer_size, "%.2e", value);
    } else {
        snprintf(buffer, buffer_size, "%.2f", value);
    }
}

// Validate graph parameters
int validate_graph_params(const GraphParams* params) {
    if (!params) return 0;
    
    // Check for valid ranges and dimensions
    if (params->max_x <= params->min_x ||
        params->max_y <= params->min_y ||
        params->width <= 0 ||
        params->height <= 0 ||
        params->x_divisions <= 0 ||
        params->y_divisions <= 0 ||
        !params->points ||
        params->num_points <= 0) {
        return 0;
    }
    
    return 1;
}

// Write PostScript file header
void write_ps_header(FILE* ps_file, const GraphParams* params) {
    fprintf(ps_file, "%%!PS-Adobe-3.0\n");
    fprintf(ps_file, "%%Creator: Jiri Joska\n");
    fprintf(ps_file, "%%Title: Graph of f(x)\n");
    fprintf(ps_file, "%%BoundingBox: 0 0 %d %d\n", 
            params->width + 100,  // Add margins
            params->height + 100);
    fprintf(ps_file, "%%EndComments\n\n");
}

// Set up coordinate system in PostScript
void setup_coordinate_system(FILE* ps_file, const GraphParams* params) {
    fprintf(ps_file, "/margin 50 def\n");
    fprintf(ps_file, "/graphWidth %d def\n", params->width);
    fprintf(ps_file, "/graphHeight %d def\n", params->height);
    
    // Calculate scales with protection against division by zero
    double x_range = params->max_x - params->min_x;
    double y_range = params->max_y - params->min_y;
    
    fprintf(ps_file, "/xScale %g def\n", 
            (x_range != 0) ? params->width / x_range : 1.0);
    fprintf(ps_file, "/yScale %g def\n", 
            (y_range != 0) ? params->height / y_range : 1.0);
}

// Draw the function curve
void draw_function(FILE* ps_file, const GraphParams* params) {
    fprintf(ps_file, "%% Draw Function\n");
    fprintf(ps_file, "0 0 1 setrgbcolor\n");  // Blue color
    fprintf(ps_file, "1 setlinewidth\n");
    fprintf(ps_file, "newpath\n");

    int first_valid_point = 1;  // Flag to track the first valid point

    for (int i = 0; i < params->num_points; i++) {
        double x = params->min_x + (params->max_x - params->min_x) * ((double)i / (params->num_points - 1));
        double y = params->points[i];

        // Check if y is within the specified range
        if (y >= params->min_y && y <= params->max_y) {
            // Convert x and y to graph coordinates
            double graph_x = ((x - params->min_x) / (params->max_x - params->min_x)) * params->width;
            double graph_y = ((y - params->min_y) / (params->max_y - params->min_y)) * params->height;

            if (first_valid_point) {
                fprintf(ps_file, "%g %g moveto\n", graph_x, graph_y);
                first_valid_point = 0;
            } else {
                fprintf(ps_file, "%g %g lineto\n", graph_x, graph_y);
            }
        } else {
            // Reset first_valid_point when encountering an out-of-range point
            first_valid_point = 1;
        }
    }

    fprintf(ps_file, "stroke\n\n");
}

// Draw graph grid and axes
void draw_grid_and_axes(FILE* ps_file, const GraphParams* params) {
    // Draw graph background
    fprintf(ps_file, "%% Draw graph background\n");
    fprintf(ps_file, "gsave\n");
    fprintf(ps_file, "0.95 setgray\n");
    fprintf(ps_file, "newpath\n");
    fprintf(ps_file, "0 0 moveto\n");
    fprintf(ps_file, "graphWidth 0 lineto\n");
    fprintf(ps_file, "graphWidth graphHeight lineto\n");
    fprintf(ps_file, "0 graphHeight lineto\n");
    fprintf(ps_file, "closepath fill\n");
    fprintf(ps_file, "grestore\n\n");

    // Grid lines
    fprintf(ps_file, "%% Draw grid lines\n");
    fprintf(ps_file, "0.8 setgray\n");
    fprintf(ps_file, "0.3 setlinewidth\n");
    
    // X-axis grid lines
    for (int i = 0; i <= params->x_divisions; i++) {
        double x_pos = (double)i * params->width / params->x_divisions;
        fprintf(ps_file, "newpath\n");
        fprintf(ps_file, "%g 0 moveto\n", x_pos);
        fprintf(ps_file, "%g graphHeight lineto\n", x_pos);
        fprintf(ps_file, "stroke\n");
    }

    // Y-axis grid lines
    for (int i = 0; i <= params->y_divisions; i++) {
        double y_pos = (double)i * params->height / params->y_divisions;
        fprintf(ps_file, "newpath\n");
        fprintf(ps_file, "0 %g moveto\n", y_pos);
        fprintf(ps_file, "graphWidth %g lineto\n", y_pos);
        fprintf(ps_file, "stroke\n");
    }

    // Main axes
    fprintf(ps_file, "%% Draw main axes\n");
    fprintf(ps_file, "0 setgray\n");
    fprintf(ps_file, "1 setlinewidth\n");
    fprintf(ps_file, "newpath\n");
    fprintf(ps_file, "0 0 moveto\n");
    fprintf(ps_file, "graphWidth 0 lineto\n");
    fprintf(ps_file, "0 0 moveto\n");
    fprintf(ps_file, "0 graphHeight lineto\n");
    fprintf(ps_file, "stroke\n\n");
}

// Label the axes
void label_axes(FILE* ps_file, const GraphParams* params) {
    char label_buffer[32];
    
    fprintf(ps_file, "%% Draw axis labels\n");
    fprintf(ps_file, "/Helvetica findfont 10 scalefont setfont\n");

    // X-axis labels
    for (int i = 0; i <= params->x_divisions; i++) {
        double x_pos = (double)i * params->width / params->x_divisions;
        double x_value = params->min_x + (params->max_x - params->min_x) * ((double)i / params->x_divisions);
        generate_axis_label(label_buffer, sizeof(label_buffer), x_value);
        
        fprintf(ps_file, "%g -15 moveto\n", x_pos);
        fprintf(ps_file, "(%s) dup stringwidth pop 2 div neg 0 rmoveto show\n", label_buffer);
    }

    // Y-axis labels
    for (int i = 0; i <= params->y_divisions; i++) {
        double y_pos = (double)i * params->height / params->y_divisions;
        double y_value = params->min_y + (params->max_y - params->min_y) * ((double)i / params->y_divisions);
        generate_axis_label(label_buffer, sizeof(label_buffer), y_value);
        
        fprintf(ps_file, "-30 %g moveto\n", y_pos);
        fprintf(ps_file, "(%s) dup stringwidth pop neg 0 rmoveto show\n", label_buffer);
    }

    // Axis titles
    fprintf(ps_file, "/Helvetica-Bold findfont 12 scalefont setfont\n");
    fprintf(ps_file, "graphWidth 2 div -35 moveto\n");
    fprintf(ps_file, "(x) dup stringwidth pop 2 div neg 0 rmoveto show\n");
    
    fprintf(ps_file, "-35 graphHeight 2 div moveto\n");
    fprintf(ps_file, "90 rotate\n");
    fprintf(ps_file, "(f(x)) dup stringwidth pop 2 div neg 0 rmoveto show\n");
    fprintf(ps_file, "-90 rotate\n");
}

// Main function to generate PostScript graph
int generate_postscript_graph(const GraphParams* params, const char* output_file) {
    if (!validate_graph_params(params) || !output_file) {
        return ERROR_INVALID_PARAMS;
    }

    FILE *ps_file = fopen(output_file, "w");
    if (!ps_file) {
        return ERROR_FILE_OPERATION;
    }

    // Write PostScript document
    write_ps_header(ps_file, params);
    setup_coordinate_system(ps_file, params);

    // Begin graphics state
    fprintf(ps_file, "gsave\n");
    fprintf(ps_file, "margin margin translate\n");
    fprintf(ps_file, "/Helvetica-Bold findfont 12 scalefont setfont\n\n");

    // Draw graph components
    draw_grid_and_axes(ps_file, params);
    label_axes(ps_file, params);
    draw_function(ps_file, params);

    // End document
    fprintf(ps_file, "grestore\n");
    fprintf(ps_file, "showpage\n");
    fprintf(ps_file, "%%EOF\n");

    fclose(ps_file);
    return 0;
}

// Error reporting function
void print_graph_error(int error_code) {
    const char* error_messages[] = {
        "Success",
        "Invalid parameters provided",
        "File operation failed",
        "Memory allocation failed"
    };
    
    if (error_code >= 0 || error_code < -3) {
        fprintf(stderr, "Unknown error occurred\n");
    } else {
        fprintf(stderr, "Error: %s\n", error_messages[-error_code]);
    }
}