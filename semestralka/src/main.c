#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "parser.h"
#include "postscript.h"

/* error codes */
#define SUCCESS 0
#define ERR_MISSING_ARGS 1
#define ERR_INVALID_FUNC 2
#define ERR_FILE_CREATION 3
#define ERR_INVALID_LIMITS 4

/* default limits */
#define DEFAULT_XMIN -10
#define DEFAULT_XMAX 10
#define DEFAULT_YMIN -10
#define DEFAULT_YMAX 10

/* function prototypes */
void print_usage(void);
int parse_limits(const char *limit_str, double *xmin, double *xmax, double *ymin, double *ymax);
char* process_function_string(const char *input);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_usage();
        return ERR_MISSING_ARGS;
    }

    /* Process function string - handle quotes if present */
    char *func_str = process_function_string(argv[1]);
    if (!func_str) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return ERR_INVALID_FUNC;
    }
    
    const char *output_file = argv[2];
    
    double xmin = DEFAULT_XMIN, xmax = DEFAULT_XMAX;
    double ymin = DEFAULT_YMIN, ymax = DEFAULT_YMAX;

    /* Parse limits if provided */
    if (argc == 4) {
        if (parse_limits(argv[3], &xmin, &xmax, &ymin, &ymax) != 0) {
            fprintf(stderr, "Error: Invalid limits format. Expected format: xmin:xmax:ymin:ymax\n");
            free(func_str);
            return ERR_INVALID_LIMITS;
        }
        
        /* Validate limits */
        if (xmin >= xmax || ymin >= ymax) {
            fprintf(stderr, "Error: Invalid limits. Ensure xmin < xmax and ymin < ymax\n");
            free(func_str);
            return ERR_INVALID_LIMITS;
        }
    }

    /* Validate and parse the function */
    if (!is_valid_function(func_str)) {
        fprintf(stderr, "Error: Invalid function format: %s\n", func_str);
        free(func_str);
        return ERR_INVALID_FUNC;
    }

    /* Open the PostScript output file */
    FILE *fp = fopen(output_file, "w");
    if (!fp) {
        fprintf(stderr, "Error: Cannot create output file '%s'.\n", output_file);
        free(func_str);
        return ERR_FILE_CREATION;
    }

    /* Initialize PostScript output */
    /*init_postscript(fp, xmin, xmax, ymin, ymax);*/

    /* Evaluate function and generate graph */
    const double step = (xmax - xmin) / 1000.0; /* Use 1000 points for smooth curve */
    double x;
    int first_point = 1;
    
    for (x = xmin; x <= xmax; x += step) {
        double y = evaluate_function(func_str, x);
        if (y >= ymin && y <= ymax) {
            if (first_point) {
                /*move_to(fp, x, y);*/
                first_point = 0;
            } else {
                /*line_to(fp, x, y);*/
            }
        }
    }

    /* Finalize PostScript file */
    /*close_postscript(fp);*/
    fclose(fp);
    free(func_str);

    return SUCCESS;
}

char* process_function_string(const char *input) {
    size_t len = strlen(input);
    char *result;
    
    /* Remove quotes if present */
    if (len >= 2 && input[0] == '"' && input[len-1] == '"') {
        result = malloc(len - 1); /* -2 for quotes, +1 for null terminator */
        if (!result) return NULL;
        strncpy(result, input + 1, len - 2);
        result[len - 2] = '\0';
    } else {
        result = strdup(input);
    }
    
    return result;
}

void print_usage(void) {
    fprintf(stderr, "Usage: graph <func> <out-file> [<limits>]\n\n");
    fprintf(stderr, "Parameters:\n");
    fprintf(stderr, "  <func>     Mathematical function (e.g., \"sin(x^2)*cos(x)\")\n");
    fprintf(stderr, "  <out-file> Output PostScript file name\n");
    fprintf(stderr, "  [<limits>] Optional limits in format xmin:xmax:ymin:ymax\n\n");
    fprintf(stderr, "Examples:\n");
    fprintf(stderr, "  graph \"sin(x)\" output.ps\n");
    fprintf(stderr, "  graph \"x^2 + 2*x + 1\" output.ps -2:2:-1:5\n");
}

int parse_limits(const char *limit_str, double *xmin, double *xmax, double *ymin, double *ymax) {
    char *str = strdup(limit_str);
    if (!str) return -1;
    
    char *xmin_str = strtok(str, ":");
    char *xmax_str = strtok(NULL, ":");
    char *ymin_str = strtok(NULL, ":");
    char *ymax_str = strtok(NULL, ":");
    
    if (!xmin_str || !xmax_str || !ymin_str || !ymax_str) {
        free(str);
        return -1;
    }
    
    *xmin = atof(xmin_str);
    *xmax = atof(xmax_str);
    *ymin = atof(ymin_str);
    *ymax = atof(ymax_str);
    
    free(str);
    return 0;
}