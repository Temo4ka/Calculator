#include <stdlib.h>
#include <stdio.h>
#include "config.h"

struct Buffer {
    const char *fileName = nullptr;

    FILE *stream = nullptr;
    
    char *buffer = nullptr;
    size_t size  =   0    ;

};

enum CalcErrors {
    calcOk              =       0,
    calcNullCaught      = 1 <<  0,
    calcGetT_Error      = 1 <<  1,
    calcGetD_Error      = 1 <<  2,
    calcGetP_Error      = 1 <<  3,
    calcGetU_Error      = 1 <<  4,
    calcGetN_Error      = 1 <<  5,
    calcGetV_Error      = 1 <<  6,
    calcBufferError     = 1 <<  7,
    calcGetCosError     = 1 <<  8,
    calcGetSinError     = 1 <<  9,
    calcGetLogError     = 1 << 10,
    calcGetNegError     = 1 << 11,
    calcDevisionByZero  = 1 << 12,
    calcEndOfProgramErr = 1 << 13,
};

int bufferCtor(Buffer *buf, const char *fileName);

int bufferDtor(Buffer *buf);

Type_t getG(char **buffer, size_t *err);

Type_t getE(char **buffer, size_t *err);

Type_t getT(char **buffer, size_t *err);

Type_t getP(char **buffer, size_t *err);

Type_t getN(char **buffer, size_t *err);

Type_t getD(char **buffer, size_t *err);

Type_t getU(char **buffer, size_t *err);