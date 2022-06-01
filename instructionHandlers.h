

#ifndef ASSEMBLER_INSTRUCTIONHANDLERS_H
#define ASSEMBLER_INSTRUCTIONHANDLERS_H

#include "common.h"
#include "labelHandlers.h"

#define	CI_BUFFER ((sizeof (int))*51)
#define COMMAND_BUF_SIZE 27
#define J_START_INDEX 23

short identifyCMD(char* word);  /* Identify Instruction */
short handleInstruction(short index, char* newline, symbol* head,unsigned long * ic,unsigned int **ci);/* handle Instruction */

unsigned makeR(unsigned opcode,unsigned rs,unsigned rt,unsigned rd,unsigned funct);    /* Convert R */
unsigned makeI(unsigned opcode,unsigned rs,unsigned rt,signed immed);   /* Convert I */
unsigned makeJ(unsigned opcode,unsigned reg,unsigned address);  /* Convert J */

unsigned HandleER(char* line,short index,symbol* head,unsigned long ic); /* Handle Erithmetic R */
unsigned HandleMV(char* line,short index,symbol* head,unsigned long ic); /* Handle Move R */
unsigned HandleEI(char* line,short index,symbol* head,unsigned long ic); /* Handle Erithmetic (& Save and Load) I */
unsigned HandleBI(char* line,short index,symbol* head,unsigned long ic); /* Handle Boolean I */
unsigned HandleJMP(char* line,short index,symbol* head,unsigned long ic); /* Handle JMP */
unsigned HandleLA(char* line,short index,symbol* head,unsigned long ic); /* Handle LA & CALL */
unsigned HandleSTOP(char* line,short index,symbol* head,unsigned long ic); /* Handle STOP */

/* Command struct */
typedef struct cmd {
    char name[5];
    char type;
    short funct:5;
    unsigned short opcode:6;
    unsigned (*pfunc)(char* line,short index,symbol* head,unsigned long ic);
} command;


/* Bitfield Structs */
typedef struct R {
    unsigned int opcode:6;
    unsigned int rs:5;
    unsigned int rt:5;
    unsigned int rd:5;
    unsigned int funct:5;
    unsigned int notused:6;
}R;

typedef struct I {
    unsigned int opcode:6;
    unsigned int rs:5;
    unsigned int rt:5;
    signed int immed:16;
}I;

typedef struct J {
    unsigned int opcode:6;
    unsigned int reg:1;
    unsigned int address:25;
}J;

#endif