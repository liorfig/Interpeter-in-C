

#ifndef ASSEMBLER_DIRECTIVEHANDLERS_H
#define ASSEMBLER_DIRECTIVEHANDLERS_H

#include "common.h"

#define DATA_BUF_SIZE 4
#define	DI_BUFFER 100

#define BYTE 1
#define HALFWORD 2
#define WORD 4

short identifyDirective(char* word);
short handleDirective(short index,char* line,unsigned char** di,unsigned long* dc);
short handleDB(char* line,unsigned char** di,unsigned long* dc);
short handleDH(char* line,unsigned char** di,unsigned long* dc);
short handleDW(char* line,unsigned char** di,unsigned long* dc);
short handleASCIZ(char* line,unsigned char** di,unsigned long* dc);
short verifyAndAllocate(unsigned char** ptr, unsigned long dc, int size_to_copy,int buffer);

/* Directive struct */
typedef struct dr {
    char name[7];
    short (*pfunc)(char* line,unsigned char** di,unsigned long* dc);
} directive;

#endif
