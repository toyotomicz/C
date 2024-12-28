/* ____________________________________________________________________________
    PostScript Graph Generator Implementation
    Version 1.0
    Module: postscript.c

    Implementation of the PostScript graph generation system providing
    functionality for creating publication-quality function plots.
    
    Technical Notes:
    - PostScript Level 2 compatibility
    - Uses standard PostScript coordinate system
    - Implements IEEE 754 floating-point operations
    - Employs error detection and reporting
    
    Implementation Features:
    - Automatic scale calculations
    - Intelligent label formatting
    - Grid line generation
    - Function curve plotting
    - Complete memory management
____________________________________________________________________________ */

#include "postscript.h"

/* Error codes for internal use */
#define ERROR_INVALID_PARAMS     -1  /* Invalid parameter values provided */
#define ERROR_FILE_OPERATION     -2  /* File access or write operation failed */
#define ERROR_MEMORY_ALLOCATION  -3  /* Dynamic memory allocation failed */

/* ____________________________________________________________________________
    Function: generate_axis_label
    
    Implementation Notes:
    - Handles special case for near-zero values (|x| < 0.001)
    - Uses scientific notation for large values (|x| >= 1000)
    - Maintains consistent decimal places for readability
    - Prevents buffer overflow by validating buffer size manually

    The function implements smart formatting logic:
    - Values close to zero are simplified to "0.00" to avoid scientific notation
    - Large values use scientific notation for better readability
    - All other values use fixed-point notation with 2 decimal places
    This ensures consistent and human-readable axis labels across different scales
____________________________________________________________________________ */
void generate_axis_label(char* buffer, size_t buffer_size, double value) {
    char temp[32]; /* Temporary buffer for formatting */
    
    /* Near-zero values are rounded to zero to avoid confusing labels */
    if (fabs(value) < 0.001) {
        sprintf(temp, "0.00");
    } 
    /* Large or very small values use scientific notation for clarity */
    else if (fabs(value) >= 1000) {
        sprintf(temp, "%.2e", value);
    } 
    /* Normal range values use fixed-point notation */
    else {
        sprintf(temp, "%.2f", value);
    }

    /* Copy to the output buffer safely */
    if (strlen(temp) < buffer_size) {
        strcpy(buffer, temp);
    } else if (buffer_size > 0) {
        /* Truncate if buffer_size is too small */
        strncpy(buffer, temp, buffer_size - 1);
        buffer[buffer_size - 1] = '\0';
    }
}

/* ____________________________________________________________________________
    Function: validate_graph_params
    
    Implementation Notes:
    - Performs comprehensive parameter validation
    - Checks for NULL pointers
    - Validates numerical ranges and dimensions
    - Ensures data array validity
____________________________________________________________________________ */
int validate_graph_params(const GraphParams* params) {
    if (!params) return 0;
    
    /* Check for invalid ranges and dimensions that would cause rendering issues */
    if (params->max_x <= params->min_x ||    /* X-axis range must be positive */
        params->max_y <= params->min_y ||    /* Y-axis range must be positive */
        params->width <= 0 ||                /* Width must be positive */
        params->height <= 0 ||               /* Height must be positive */
        params->x_divisions <= 0 ||          /* Must have at least one division */
        params->y_divisions <= 0 ||          /* Must have at least one division */
        !params->points ||                   /* Data array must exist */
        params->num_points <= 0) {           /* Must have at least one point */
        return 0;
    }
    
    return 1;
}

/* ____________________________________________________________________________
    Function: write_ps_header
    
    Implementation Notes:
    - Generates PostScript DSC compliant header
    - Includes metadata and creator information
    - Calculates proper bounding box with margins
    - Ensures proper document structure
____________________________________________________________________________ */
void write_ps_header(FILE* ps_file, const GraphParams* params) {
    fprintf(ps_file, "%%!PS-Adobe-3.0\n");
    fprintf(ps_file, "%%Creator: Jiri Joska\n");
    fprintf(ps_file, "%%Title: Graph of f(x)\n");
    fprintf(ps_file, "%%BoundingBox: 0 0 %d %d\n", 
            params->width + 100,  /* Add margins */
            params->height + 100);
    fprintf(ps_file, "%%EndComments\n\n");
}

/* ____________________________________________________________________________
    Function: setup_coordinate_system
    
    Implementation Notes:
    - Defines PostScript variables for graph dimensions
    - Calculates proper scaling factors
    - Handles potential division by zero
    - Sets up margin handling
____________________________________________________________________________ */
void setup_coordinate_system(FILE* ps_file, const GraphParams* params) {
    fprintf(ps_file, "/margin 50 def\n");
    fprintf(ps_file, "/graphWidth %d def\n", params->width);
    fprintf(ps_file, "/graphHeight %d def\n", params->height);
    
    double x_range = params->max_x - params->min_x;
    double y_range = params->max_y - params->min_y;
    
    fprintf(ps_file, "/xScale %g def\n", 
            (x_range != 0) ? params->width / x_range : 1.0);
    fprintf(ps_file, "/yScale %g def\n", 
            (y_range != 0) ? params->height / y_range : 1.0);
}

/* ____________________________________________________________________________
    Function: draw_function
    
    Implementation Notes:
    - Implements path drawing for function visualization
    - Handles discontinuities in the function
    - Performs range checking for each point
    - Uses efficient PostScript path commands
    - Maintains proper scaling and positioning

    The function implements several important features:
    - Automatically breaks the path when points go out of range
    - Scales points to match the graph dimensions
    - Creates a continuous line for connected points
    - Handles potential gaps in the data
____________________________________________________________________________ */
void draw_function(FILE* ps_file, const GraphParams* params) {
    fprintf(ps_file, "%% Draw Function\n");
    fprintf(ps_file, "0 0 1 setrgbcolor\n");  /* Blue color of the pen */
    fprintf(ps_file, "1 setlinewidth\n");
    fprintf(ps_file, "newpath\n");

    int first_valid_point = 1;  /* Track when to start a new path segment */

    for (int i = 0; i < params->num_points; i++) {
        /* Calculate x coordinate based on point index */
        double x = params->min_x + (params->max_x - params->min_x) * 
                  ((double)i / (params->num_points - 1));
        double y = params->points[i];

        /* Only plot points within the valid y-range to avoid artifacts */
        if (y >= params->min_y && y <= params->max_y) {
            /* Convert from data coordinates to graph coordinates */
            double graph_x = ((x - params->min_x) / 
                           (params->max_x - params->min_x)) * params->width;
            double graph_y = ((y - params->min_y) / 
                           (params->max_y - params->min_y)) * params->height;

            /* Start new path segment for first point or after discontinuity */
            if (first_valid_point) {
                fprintf(ps_file, "%g %g moveto\n", graph_x, graph_y);
                first_valid_point = 0;
            } else {
                /* Continue current path segment */
                fprintf(ps_file, "%g %g lineto\n", graph_x, graph_y);
            }
        } else {
            /* Point out of range - mark next valid point as start of new segment */
            first_valid_point = 1;
        }
    }

    fprintf(ps_file, "stroke\n\n");
}


/* ____________________________________________________________________________
    Function: draw_grid_and_axes
    
    Implementation Notes:
    - Creates graph background with proper color
    - Generates grid lines at specified intervals
    - Draws main coordinate axes
    - Uses PostScript graphics state stack
    - Implements proper line styles and colors
____________________________________________________________________________ */
void draw_grid_and_axes(FILE* ps_file, const GraphParams* params) {
    /* Draw graph background */
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

    /* Grid lines */
    fprintf(ps_file, "%% Draw grid lines\n");
    fprintf(ps_file, "0.8 setgray\n");
    fprintf(ps_file, "0.3 setlinewidth\n");
    
    /* X-axis grid lines */
    int i;
    for (i = 0; i <= params->x_divisions; i++) {
        double x_pos = (double)i * params->width / params->x_divisions;
        fprintf(ps_file, "newpath\n");
        fprintf(ps_file, "%g 0 moveto\n", x_pos);
        fprintf(ps_file, "%g graphHeight lineto\n", x_pos);
        fprintf(ps_file, "stroke\n");
    }

    /* Y-axis grid lines */
    for (i = 0; i <= params->y_divisions; i++) {
        double y_pos = (double)i * params->height / params->y_divisions;
        fprintf(ps_file, "newpath\n");
        fprintf(ps_file, "0 %g moveto\n", y_pos);
        fprintf(ps_file, "graphWidth %g lineto\n", y_pos);
        fprintf(ps_file, "stroke\n");
    }

    /* Main axes */
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

/* ____________________________________________________________________________
    Function: label_axes
    
    Implementation Notes:
    - Implements automatic label positioning
    - Centers labels on grid lines
    - Handles font selection and scaling
    - Rotates text for y-axis title
    - Uses PostScript text operators efficiently

    The function implements several text positioning techniques:
    - X-axis labels are centered under their grid lines using string width calculations
    - Y-axis labels are right-aligned using negative string width offsets
    - Axis titles use a larger, bold font and are centered along their axes
    - Y-axis title is rotated 90 degrees using PostScript's coordinate system
    - Labels maintain proper spacing even with varying text lengths
____________________________________________________________________________ */
void label_axes(FILE* ps_file, const GraphParams* params) {
    char label_buffer[32];
    
    fprintf(ps_file, "%% Draw axis labels\n");
    /* Use Helvetica for readability at small sizes */
    fprintf(ps_file, "/Helvetica findfont 10 scalefont setfont\n");

    /* X-axis labels: positioned below x-axis with centered alignment */
    for (int i = 0; i <= params->x_divisions; i++) {
        /* Calculate position and value for this grid line */
        double x_pos = (double)i * params->width / params->x_divisions;
        double x_value = params->min_x + (params->max_x - params->min_x) * 
                        ((double)i / params->x_divisions);
        generate_axis_label(label_buffer, sizeof(label_buffer), x_value);
        
        /* Position text 15 units below axis line */
        fprintf(ps_file, "%g -15 moveto\n", x_pos);
        /* Center text by moving left half the string width */
        fprintf(ps_file, "(%s) dup stringwidth pop 2 div neg 0 rmoveto show\n", 
                label_buffer);
    }

    /* Y-axis labels: positioned left of y-axis with right alignment */
    for (int i = 0; i <= params->y_divisions; i++) {
        /* Calculate position and value for this grid line */
        double y_pos = (double)i * params->height / params->y_divisions;
        double y_value = params->min_y + (params->max_y - params->min_y) * 
                        ((double)i / params->y_divisions);
        generate_axis_label(label_buffer, sizeof(label_buffer), y_value);
        
        /* Position text 10 units left of axis line */
        fprintf(ps_file, "-10 %g moveto\n", y_pos);
        /* Right-align text by moving left the full string width */
        fprintf(ps_file, "(%s) dup stringwidth pop neg 0 rmoveto show\n", 
                label_buffer);
    }

    /* Axis titles: centered along axes with larger bold font */
    fprintf(ps_file, "/Helvetica-Bold findfont 12 scalefont setfont\n");
    
    /* X-axis title: centered below labels */
    fprintf(ps_file, "graphWidth 2 div -35 moveto\n");
    fprintf(ps_file, "(x) dup stringwidth pop 2 div neg 0 rmoveto show\n");
    
    /* Y-axis title: centered vertically, rotated 90 degrees */
    fprintf(ps_file, "-35 graphHeight 2 div moveto\n");
    fprintf(ps_file, "90 rotate\n");                /* Start rotation */
    fprintf(ps_file, "(f(x)) dup stringwidth pop 2 div neg 0 rmoveto show\n");
    fprintf(ps_file, "-90 rotate\n");              /* Restore rotation */
}


/* ____________________________________________________________________________
    Function: generate_postscript_graph
    
    Implementation Notes:
    - Coordinates complete graph generation process
    - Validates all input parameters
    - Manages file operations
    - Sets up proper PostScript environment
    - Ensures proper cleanup and file closure
____________________________________________________________________________ */
int generate_postscript_graph(const GraphParams* params, const char* output_file) {
    if (!validate_graph_params(params) || !output_file) {
        return ERROR_INVALID_PARAMS;
    }

    FILE *ps_file = fopen(output_file, "w");
    if (!ps_file) {
        return ERROR_FILE_OPERATION;
    }

    /* Write PostScript document */
    write_ps_header(ps_file, params);
    setup_coordinate_system(ps_file, params);

    /* Begin graphics state */
    fprintf(ps_file, "gsave\n");
    fprintf(ps_file, "margin margin translate\n");
    fprintf(ps_file, "/Helvetica-Bold findfont 12 scalefont setfont\n\n");

    /* Draw graph components */
    draw_grid_and_axes(ps_file, params);
    label_axes(ps_file, params);
    draw_function(ps_file, params);

    /* End document */
    fprintf(ps_file, "grestore\n");
    fprintf(ps_file, "showpage\n");
    fprintf(ps_file, "%%EOF\n");

    fclose(ps_file);
    return 0;
}

/* ____________________________________________________________________________
    Function: print_graph_error
    
    Implementation Notes:
    - Provides user-friendly error messages
    - Handles all defined error codes
    - Writes to standard error stream
    - Includes fallback for unknown errors
____________________________________________________________________________ */
void print_graph_error(int error_code) {
    /* Error messages array indexed by negative error code
       e.g., error code -1 corresponds to index 1 */
    const char* error_messages[] = {
        "Success",                       /* For error_code 0 */
        "Invalid parameters provided",   /* For error_code -1 */
        "File operation failed",         /* For error_code -2 */
        "Memory allocation failed"       /* For error_code -3 */
    };
    
    /* Check if error code is out of range:
       - error_code >= 0 means not an error
       - error_code < -3 means beyond our defined errors */
    if (error_code >= 0 || error_code < -3) {
        fprintf(stderr, "Unknown error occurred\n");
    } else {
        /* Convert negative error code to positive array index */
        fprintf(stderr, "Error: %s\n", error_messages[-error_code]);
    }
}