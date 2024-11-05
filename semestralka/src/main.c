#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "parser.h"
#include "postscript.h"

// Error codes
#define SUCCESS 0
#define ERR_MISSING_ARGS 1
#define ERR_INVALID_FUNC 2
#define ERR_FILE_CREATION 3
#define ERR_INVALID_LIMITS 4

// Default limits if not provided
#define DEFAULT_XMIN -10
#define DEFAULT_XMAX 10
#define DEFAULT_YMIN -10
#define DEFAULT_YMAX 10

// Function prototypes
void print_usage();
int parse_limits(const char *limit_str, double *xmin, double *xmax, double *ymin, double *ymax);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_usage();
        return ERR_MISSING_ARGS;
    }

    const char *func_str = argv[1];
    const char *output_file = argv[2];

    double xmin = DEFAULT_XMIN, xmax = DEFAULT_XMAX;
    double ymin = DEFAULT_YMIN, ymax = DEFAULT_YMAX;

    // Parse limits if provided
    if (argc == 4) {
        if (parse_limits(argv[3], &xmin, &xmax, &ymin, &ymax) != 0) {
            fprintf(stderr, "Error: Invalid limits format.\n");
            return ERR_INVALID_LIMITS;
        }
    }

    // Validate and parse the function
    if (!is_valid_function(func_str)) {
        fprintf(stderr, "Error: Invalid function format.\n");
        return ERR_INVALID_FUNC;
    }

    // Open the PostScript output file
    FILE *fp = fopen(output_file, "w");
    if (!fp) {
        fprintf(stderr, "Error: Cannot create output file '%s'.\n", output_file);
        return ERR_FILE_CREATION;
    }

    // Initialize PostScript output
    init_postscript(fp, xmin, xmax, ymin, ymax);

    // Evaluate function and generate graph
    for (double x = xmin; x <= xmax; x += 0.1) {
        double y = evaluate_function(func_str, x);
        if (y >= ymin && y <= ymax) {
            plot_point(fp, x, y);
        }
    }

    // Finalize PostScript file
    close_postscript(fp);
    fclose(fp);

    return SUCCESS;
}

void print_usage() {
    fprintf(stderr, "Usage: graph.exe <func> <out-file> [<limits>]\n");
    fprintf(stderr, "<func>     : Mathematical function, e.g., 'sin(x^2)'\n");
    fprintf(stderr, "<out-file> : Output PostScript file name\n");
    fprintf(stderr, "[<limits>] : Optional limits as 'xmin:xmax:ymin:ymax'\n");
}

int parse_limits(const char *limit_str, double *xmin, double *xmax, double *ymin, double *ymax) {
    return sscanf(limit_str, "%lf:%lf:%lf:%lf", xmin, xmax, ymin, ymax) == 4 ? 0 : -1;
}
