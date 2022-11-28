#include "headers/calc.h"

int main() {
    size_t err = 0;
    char *buffer = (char *) calloc(1000, sizeof(char));
    fread(buffer, sizeof(char), 900, fopen("in.txt", "r"));
    fprintf(stderr, "%lg", getG(&buffer, &err));
    // fprintf(stderr, "%s", buffer);
    return EXIT_SUCCESS;
}