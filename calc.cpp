#include "headers/DSL.h"
#include "headers/calc.h"
#include <math.h>
#include <ctype.h>
#include <string.h>

static Type_t getCos(char **buffer, size_t *err);

static Type_t getSin(char **buffer, size_t *err);

static Type_t getLog(char **buffer, size_t *err);

static Type_t getNeg(char **buffer, size_t *err);

Type_t getG(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    Type_t val = getE(buffer, err);

    if (CUR_SYM != '\n') ERR_EXE(calcEndOfProgramErr);
    
    return val;
}

Type_t getE(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    Type_t val = getT(buffer, err);

    while (CUR_SYM == '+' || CUR_SYM == '-') {
        char curOp = CUR_SYM;
        NEXT_SYM;
        Type_t val2 = getT(buffer, err);
        if (curOp == '+')
            val += val2;
        else
            val -= val2;
    }

    return val;
}

Type_t getT(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    Type_t val = getD(buffer, err);

    while (CUR_SYM == '*' || CUR_SYM == '/') {
        char curOp = CUR_SYM;
        NEXT_SYM;

        Type_t val2 = getD(buffer, err);
        if (val2 == POISON) ERR_EXE(calcGetT_Error);

        if (curOp == '*')
            val *= val2;
        else {
            if (val2 == 0) ERR_EXE(calcDevisionByZero);
            val /= val2;
        }
    }

    return val;
}

Type_t getD(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    Type_t val = getU(buffer, err);

    while (CUR_SYM == '^') {
        NEXT_SYM;

        Type_t val2 = getU(buffer, err);
        if (val2 == POISON) ERR_EXE(calcGetD_Error);

        val = pow(val, val2);
    }

    return val;
}

Type_t getU(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    if (!strncmp("cos(", CUR_STR, 4)) {
        Type_t val = getCos(buffer, err);
        if (*err) ERR_EXE(calcGetU_Error);

        return val;
    }

    if (!strncmp("sin(", CUR_STR, 4)) {
        Type_t val = getSin(buffer, err);
        if (*err) ERR_EXE(calcGetU_Error);
        
        return val;
    }

    if (!strncmp("ln(", CUR_STR, 3)) {
        Type_t val = getLog(buffer, err);
        if (*err) ERR_EXE(calcGetU_Error);
        
        return val;
    }

    if (CUR_SYM == '-') {
        Type_t val = getNeg(buffer, err);
        if (*err) ERR_EXE(calcGetU_Error);

        return val;
    }

    ERR_EXE(calcGetU_Error);
}

Type_t getP(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    if (CUR_SYM == '(') {
        NEXT_SYM;
        Type_t val = getE(buffer, err);
        if (CUR_SYM != ')') return calcGetP_Error;
        NEXT_SYM;
        
        return val;
    }
    Type_t val = getN(buffer, err);

    return val;
}

Type_t getN(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    Type_t val = 0;

    if (CUR_SYM < '0' && CUR_SYM > '9') ERR_EXE(calcGetN_Error);

    while (CUR_SYM >= '0' && CUR_SYM <= '9') {
        val = val * 10 + CUR_SYM - '0';
        NEXT_SYM;
    }

    return val;
}

char* getV(char **buffer, size_t *err) {
    catchNullptr(buffer, nullptr, *err |= calcNullCaught;);

    char *newVar = (char *) calloc(MAX_VAR_SIZE, sizeof(char));
    *newVar = '\0';

    if (!isalpha(CUR_SYM)) {
        *err |= calcGetV_Error;
        return nullptr;
    }

    while (isalpha(CUR_SYM)) {
        *newVar = CUR_SYM;
        NEXT_SYM;
    }
    
    return newVar;
}

static Type_t getCos(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    CUR_STR += 4;

    Type_t val = getP(buffer, err);

    if (val == POISON || CUR_SYM != ')') ERR_EXE(calcGetCosError);

    NEXT_SYM;

    return cos(val);
}

static Type_t getSin(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    CUR_STR += 4;

    Type_t val = getP(buffer, err);

    if (val == POISON || CUR_SYM != ')') ERR_EXE(calcGetSinError);

    NEXT_SYM;

    return sin(val);
}

static Type_t getLog(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    CUR_STR += 4;

    Type_t val = getP(buffer, err);

    if (val == POISON || CUR_SYM != ')') ERR_EXE(calcGetLogError);

    NEXT_SYM;

    return log(val);
}

static Type_t getNeg(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    CUR_STR++;

    Type_t val = getP(buffer, err);

    if (val == POISON) ERR_EXE(calcGetNegError);

    return -val;
}

//TODO: strncmp