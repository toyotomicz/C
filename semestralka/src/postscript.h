/* ____________________________________________________________________________
    PostScript Graph Generator
    Version 1.0
    
    A comprehensive system for generating publication-quality function graphs
    in PostScript format with support for customizable axes, grid lines,
    and function plotting.
    
    Key Features:
    - Automatic axis scaling and labeling
    - Customizable graph dimensions and divisions
    - Support for arbitrary function data points
    - Professional-grade PostScript output
    - Comprehensive error handling
    
    Implementation Details:
    - ANSI C compatible
    - IEEE 754 floating-point arithmetic
    - PostScript Level 2 compatible output
    
    Usage Requirements:
    - Input data must be pre-calculated
    - Requires write access to output directory
    - Memory requirements scale with number of points
____________________________________________________________________________ */

#ifndef POSTSCRIPT_GRAPH_H
#define POSTSCRIPT_GRAPH_H

#include <stdio.h>   /* Input and output functions */
#include <stdlib.h>  /* Standard library utilities */
#include <string.h>  /* String manipulation functions */
#include <math.h>    /* Mathematical functions */
#include <float.h>   /* Defines limits for floating-point numbers */



/* Error codes for diagnostic purposes */
#define ERROR_INVALID_PARAMS     -1  /* Invalid parameter values provided */
#define ERROR_FILE_OPERATION     -2  /* File access or write operation failed */
#define ERROR_MEMORY_ALLOCATION  -3  /* Dynamic memory allocation failed */

/* 
    Graph Parameters Structure
    
    Contains all necessary parameters for graph generation:
    - Axis ranges and dimensions
    - Grid division specifications
    - Function data points
*/
typedef struct {
    double min_x, max_x;        /* X-axis range */
    double min_y, max_y;        /* Y-axis range */
    int width, height;          /* Graph dimensions in points */
    int x_divisions;            /* Number of x-axis grid divisions */
    int y_divisions;            /* Number of y-axis grid divisions */
    double *points;             /* Array of function values */
    int num_points;             /* Number of data points */
} GraphParams;

/* ____________________________________________________________________________
    Function: generate_axis_label
    
    Generates formatted string labels for axis values with appropriate
    precision and scientific notation when necessary.
    
    Parameters:
    - buffer:      Output string buffer
    - buffer_size: Size of the output buffer
    - value:       Numeric value to format
    
    Format Rules:
    - Uses scientific notation for very large/small numbers
    - Fixed precision for normal range values
    - Special case handling for near-zero values
____________________________________________________________________________ */
void generate_axis_label(char* buffer, size_t buffer_size, double value);

/* ____________________________________________________________________________
    Function: validate_graph_params
    
    Validates all graph parameters for correctness and consistency.
    
    Parameters:
    - params: Pointer to graph parameters structure
    
    Returns:
    - 1 if parameters are valid
    - 0 if any parameter is invalid
    
    Validation Checks:
    - Range validity
    - Dimension validity
    - Division counts
    - Data point array validity
____________________________________________________________________________ */
int validate_graph_params(const GraphParams* params);

/* ____________________________________________________________________________
    Function: write_ps_header
    
    Writes the PostScript file header with appropriate document structure
    and metadata.
    
    Parameters:
    - ps_file: Output file pointer
    - params:  Graph parameters for dimension information
    
    PostScript Elements:
    - Adobe DSC comments
    - Creator information
    - BoundingBox specification
____________________________________________________________________________ */
void write_ps_header(FILE* ps_file, const GraphParams* params);

/* ____________________________________________________________________________
    Function: setup_coordinate_system
    
    Establishes the PostScript coordinate system and scaling factors
    for the graph.
    
    Parameters:
    - ps_file: Output file pointer
    - params:  Graph parameters for scaling calculations
    
    Coordinate System:
    - Origin at lower-left corner
    - Scales adjusted for specified ranges
    - Margins included in calculations
____________________________________________________________________________ */
void setup_coordinate_system(FILE* ps_file, const GraphParams* params);

/* ____________________________________________________________________________
    Function: draw_function
    
    Renders the function curve using the provided data points.
    
    Parameters:
    - ps_file: Output file pointer
    - params:  Graph parameters including function data
    
    Drawing Features:
    - Automatic point connection
    - Range checking
    - Discontinuity handling
____________________________________________________________________________ */
void draw_function(FILE* ps_file, const GraphParams* params);

/* ____________________________________________________________________________
    Function: draw_grid_and_axes
    
    Draws the graph grid lines and main coordinate axes.
    
    Parameters:
    - ps_file: Output file pointer
    - params:  Graph parameters for grid specifications
    
    Visual Elements:
    - Background fill
    - Grid lines at specified divisions
    - Main coordinate axes
____________________________________________________________________________ */
void draw_grid_and_axes(FILE* ps_file, const GraphParams* params);

/* ____________________________________________________________________________
    Function: label_axes
    
    Adds formatted labels to both axes including tick marks and titles.
    
    Parameters:
    - ps_file: Output file pointer
    - params:  Graph parameters for label positioning
    
    Label Features:
    - Automatic value formatting
    - Centered placement
    - Axis titles
____________________________________________________________________________ */
void label_axes(FILE* ps_file, const GraphParams* params);

/* ____________________________________________________________________________
    Function: generate_postscript_graph
    
    Main function for generating complete PostScript graph file.
    
    Parameters:
    - params:      Graph specification and data
    - output_file: Path to output file
    
    Returns:
    - 0 on success
    - Negative error code on failure
    
    Process Steps:
    - Parameter validation
    - File creation
    - Graph element generation
    - Resource cleanup
____________________________________________________________________________ */
int generate_postscript_graph(const GraphParams* params, const char* output_file);

#endif /* POSTSCRIPT_GRAPH_H */