#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "directiveHandlers.h"
#include "errorDefs.h"
#include "parser.h"

/* Directive Table*/
directive directives[DATA_BUF_SIZE] =
        {
                {".db", handleDB},
                {".dh", handleDH},
                {".dw", handleDW},
                {".asciz", handleASCIZ}
        };

short identifyDirective(char* word){
    short i = 0;
    while(strcmp(word,directives[i].name) && i < DATA_BUF_SIZE) /* check if data command */
    {
        i++;
    }
    if (i < DATA_BUF_SIZE)
        return i;
    else
        return ERROR;
}

short handleDirective(short index,char* line,unsigned char** di,unsigned long* dc){
    short error;
    error = directives[index].pfunc(line,di,dc);
    return error;
}

short handleASCIZ(char* line,unsigned char** di,unsigned long* dc){
    short error = VALID;
    int count = 0;
    if (line == NULL)
        return MISSINGOPERANDS;
    while(*line != '\n' && *line != EOF)
    {
        if (*line != '"' && !isspace(*line))
            return ILLEGALOPERAND;
        if (!isspace(*line) && count > 0)
            return ILLEGALOPERAND;
        if(*line == '"')
        {
            line++;
            while(*line != '"'){
                if (*line == '\n' || *line == EOF)
                    return ILLEGALOPERAND;
                error =  verifyAndAllocate(di,*dc, BYTE,DI_BUFFER);
                if (error != VALID)
                    return NOTENOUGHMEMORY;
                memcpy((*di + *dc), line, BYTE);
                *dc += BYTE;
                line++;
                count++; /* count if got data */
            }
        }
        line++;
    }

    error = verifyAndAllocate(di,*dc, BYTE,DI_BUFFER);
    *(*di + *dc) = '\0';
    *dc += BYTE;

    return error;
}

short handleDB(char* line,unsigned char** di,unsigned long* dc){
    char num, *oper;
    short error = VALID;
    int temp; /*  temp for converting */
    oper = getNextWord(&line,OPERDELIM);
    if (oper == NULL)
        return MISSINGOPERANDS;

    else if (*oper == '\0') /* if empty */
        return EMPTYOPERAND;

    error = convertNum(oper, SCHAR_TYPE,&temp);
    if(error != VALID)
        return error;
    num = (char)temp;


    error = verifyAndAllocate(di,*dc, BYTE,DI_BUFFER);
    if (error != VALID)
        return NOTENOUGHMEMORY;
    memcpy((*di + *dc), &num, BYTE);
    *dc += BYTE;

    while (line != NULL) {
        oper = getNextWord(&line,OPERDELIM);
        if (oper == NULL){
            break; /* Line finished. */
        }
        else if (*oper == '\0') /* If empty */
            return EMPTYOPERAND;

        error = convertNum(oper,SCHAR_TYPE,&temp);
        if(error != VALID)
            return error;
        num = (char)temp;

        error = verifyAndAllocate(di,*dc, BYTE,DI_BUFFER);
        if (error != VALID)
            return NOTENOUGHMEMORY;
        memcpy((*di + *dc), &num, BYTE);
        *dc += BYTE;
    }
    return VALID;
}


short handleDH(char* line,unsigned char** di,unsigned long* dc){
    short num,error;
    char *oper;
    int temp; /* For converting */

    oper = getNextWord(&line,OPERDELIM);
    if (oper == NULL){
        return MISSINGOPERANDS;
    }
    else if (*oper == '\0') /* If empty */
        return EMPTYOPERAND;

    error = convertNum(oper,SHRT_TYPE,&temp); /* Convert oper to num */
    if (error != VALID)
        return error;
    num = (short)temp;

    error = verifyAndAllocate(di,*dc, HALFWORD,DI_BUFFER);
    if (error != VALID)
        return NOTENOUGHMEMORY;
    memcpy((*di + *dc), &num, HALFWORD);
    *dc += HALFWORD;

    while (line != NULL) {
        oper = getNextWord(&line,OPERDELIM);
        if (oper == NULL){
            break; /* Line finished. */
        }
        else if (*oper == '\0') /* If empty */
            return EMPTYOPERAND;

        error = convertNum(oper,SHRT_TYPE,&temp);
        if (error != VALID)
            return error;
        num = (short)temp;
        error = verifyAndAllocate(di,*dc, HALFWORD,DI_BUFFER);
        if (error != VALID)
            return NOTENOUGHMEMORY;
        memcpy((*di + *dc), &num, HALFWORD);
        *dc += HALFWORD;
    }
    return VALID;
}


short handleDW(char* line,unsigned char** di,unsigned long* dc){
    int num;
    short error;
    char *oper;

    oper = getNextWord(&line,OPERDELIM);
    if (oper == NULL){
        return MISSINGOPERANDS;
    }
    else if (*oper == '\0') /* If empty */
        return EMPTYOPERAND;

    error = convertNum(oper,INT_TYPE,&num);
    if (error != VALID)
        return error;

    error = verifyAndAllocate(di,*dc, WORD,DI_BUFFER);
    if (error != VALID)
        return NOTENOUGHMEMORY;
    memcpy((*di + *dc), &num, WORD);
    *dc += WORD;


    while (line != NULL) {
        oper = getNextWord(&line,OPERDELIM);
        if (oper == NULL){
            break; /* Line finished. */
        }
        else if (*oper == '\0') /* If empty */
            return EMPTYOPERAND;

        error = convertNum(oper,INT_TYPE,&num);
        if (error != VALID)
            return error;
        error = verifyAndAllocate(di,*dc, WORD,DI_BUFFER);
        if (error != VALID)
            return NOTENOUGHMEMORY;
        memcpy((*di + *dc), &num, WORD);
        *dc += WORD;
    }
    return VALID;
}


short verifyAndAllocate(unsigned char** ptr, unsigned long dc, int size_to_copy, int buffer){
    int allocated_size;
    unsigned char* new_ptr;
    if(!ptr)
        return ERROR;

    if(dc == 0)
        allocated_size = buffer;
    else
        allocated_size  = ceil((double)dc/(double)buffer) * buffer; /* Find current allocated size of image */

    if((dc + size_to_copy) > allocated_size) /* need to allocate more memory */
    {
        allocated_size += buffer;
        new_ptr = (unsigned char *)realloc(*ptr, allocated_size *sizeof(unsigned char));
        if(!new_ptr)
        {
            return ERROR; /* If not enough memory */
        }
        *ptr = new_ptr;
    }

    return VALID;
}