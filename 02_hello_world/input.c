#include <stdio.h>
#include <stdlib.h>

#DEFINE MAX_ARRAY_LENGTH = 0

int main(){
    char input[MAX_ARRAY_LENGTH] = { 0 };

    printf("> ");
    fgets(input, MAX_ARRAY_LENGTH, stdin);

    puts(input);
    printf("%s\n", input);

    return EXIT_SUCCESS;
}