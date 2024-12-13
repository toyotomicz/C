#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "imgio.h"

size_t load_image(const char *path, char **image_data) {
    FILE *file = NULL;
    char *data_temp = NULL;
    size_t file_size;

    #define EXIT_IF(condition)                  \
        if (condition) {                        \
            if (file) { fclose(file); }         \
            if (data_temp) { free(data_temp); } \
            return 0;                           \
        }

    EXIT_IF(!path)

    file = fopen(path, "rb");
    EXIT_IF(!file)

    fseek(file, 0L, SEEK_END);
    file_size = ftell(file);
    EXIT_IF(file_size == 0)

    data_temp = malloc(file_size);
    EXIT_IF(!data_temp)

    rewind(file);
    EXIT_IF(fread(data_temp, 1, file_size, file) != file_size)

    fclose(file);
    *image_data = data_temp;
    return file_size;

    #undef EXIT_IF
}

int save_image(const char *path, const char *image_data, const size_t size) {
    FILE *file = NULL;

    #define EXIT_IF(condition)              \
        if (condition) {                    \
            if (file) { fclose(file); }     \
            return 0;                       \
        }

    EXIT_IF(!path || !image_data || size == 0)

    file = fopen(path, "wb");
    EXIT_IF(!file)

    EXIT_IF(fwrite(image_data, 1, size, file) != size);

    fclose(file);
    return 1;

    #undef EXIT_IF
}