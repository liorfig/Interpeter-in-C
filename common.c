#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "errorDefs.h"
#include "common.h"
short isValidNum(double num,int type){
    short error = NUMTOOBIG;
    switch (type)
    {
        case SCHAR_TYPE:
            if( num >= SCHAR_MIN  && num <= SCHAR_MAX)
                error = VALID;
            break;
        case SHRT_TYPE:
            if( num >= SHRT_MIN  && num <= SHRT_MAX)
                error = VALID;
            break;
        case INT_TYPE:
            if( num >= INT_MIN  && num <= INT_MAX)
                error = VALID;
            break;
        case UINT_TYPE:
            if( num >= 0  && num <= UINT_MAX)
                error = VALID;
            break;
        default:
            break;
    }
    return error;
}

/* get immed number */
short convertNum(char *oper,short type,int *num){
    char *ptr;
    short error;

    double db;
    db = strtod (oper,&ptr);
    if (ptr){
        if (*ptr != '\0')
            return ILLEGALOPERAND;
    }
    error = isValidNum(db, type);
    if(error != VALID)
        return error;

    *num = (int)db;
    return VALID;
}

/* get register */
unsigned convertRegister(char *oper){
    char *ptr;
    unsigned num;
    if (*oper != '$') /* if doesn't begin with '$' */
        return REGERROR;
    oper++; /* skip '$' */
    num = strtol(oper,&ptr,10);
    if (strlen(ptr) > 0){
        return REGERROR;
    }
    if (num > 32){
        return REGERROR;
    }
    return num;
}