#ifndef POSTSCRIPT_GRAPH_H
#define POSTSCRIPT_GRAPH_H

#include <stdio.h>

// Error codes
#define ERROR_INVALID_PARAMS -1
#define ERROR_FILE_OPERATION -2
#define ERROR_MEMORY_ALLOCATION -3

// Struktura pro definici parametrů grafu
typedef struct {
    double min_x;    // Minimální hodnota na ose X
    double max_x;    // Maximální hodnota na ose X
    double min_y;    // Minimální hodnota na ose Y
    double max_y;    // Maximální hodnota na ose Y
    int x_divisions; // Počet dělení na ose X (doporučeno 100)
    int y_divisions; // Počet dělení na ose Y (doporučeno 10)
    int width;       // Šířka grafu v bodech
    int height;      // Výška grafu v bodech
} GraphParams;

// Výchozí parametry grafu
#define DEFAULT_GRAPH_PARAMS { \
    -10.0,   /* min_x */ \
    10.0,    /* max_x */ \
    -10.0,   /* min_y */ \
    10.0,    /* max_y */ \
    10,     /* x_divisions */ \
    10,      /* y_divisions */ \
    512,     /* width */ \
    512      /* height */ \
}

/**
 * Vygeneruje PostScript graf podle zadaných parametrů
 * 
 * @param params Struktura s parametry grafu
 * @param output_file Cesta k výstupnímu souboru
 * @return 0 při úspěchu, nebo jeden z následujících chybových kódů:
 *         ERROR_INVALID_PARAMS: Neplatné parametry
 *         ERROR_FILE_OPERATION: Chyba při práci se souborem
 *         ERROR_MEMORY_ALLOCATION: Chyba alokace paměti
 */
int generate_postscript_graph(const GraphParams* params, const char* output_file);

/**
 * Vytiskne chybovou hlášku odpovídající danému chybovému kódu
 * @param error_code Chybový kód vrácený funkcí generate_postscript_graph
 */
void print_graph_error(int error_code);
void generate_axis_label(char* buffer, size_t buffer_size, double value);
int validate_graph_params(const GraphParams* params);
void write_ps_header(FILE* ps_file, const GraphParams* params);
void setup_coordinate_system(FILE* ps_file, const GraphParams* params);

#endif // POSTSCRIPT_GRAPH_H