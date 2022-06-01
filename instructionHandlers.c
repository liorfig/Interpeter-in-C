#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instructionHandlers.h"
#include "errorDefs.h"
#include "parser.h"
#include "common.h"

/* Command Table*/
command instructions[COMMAND_BUF_SIZE] =
        {
                {"add", 'R', 1, 0,HandleER},
                {"sub", 'R', 2, 0,HandleER},
                {"and", 'R', 3, 0,HandleER},
                {"or", 'R', 4, 0,HandleER},
                {"nor", 'R', 5, 0,HandleER},
                {"move", 'R', 1, 1,HandleMV},
                {"mvhi", 'R', 2, 1,HandleMV},
                {"mvlo", 'R', 3, 1,HandleMV},
                {"addi", 'I', 0, 10,HandleEI},
                {"subi", 'I', 0, 11,HandleEI},
                {"andi", 'I', 0, 12,HandleEI},
                {"ori", 'I', 0, 13,HandleEI},
                {"nori", 'I', 0, 14,HandleEI},
                {"bne", 'I', 0, 15,HandleBI},
                {"beq", 'I', 0, 16,HandleBI},
                {"blt", 'I', 0, 17,HandleBI},
                {"bgt", 'I', 0, 18,HandleBI},
                {"lb", 'I', 0, 19,HandleEI},
                {"sb", 'I', 0, 20,HandleEI},
                {"lw", 'I', 0, 21,HandleEI},
                {"sw", 'I', 0, 22,HandleEI},
                {"lh", 'I', 0, 23,HandleEI},
                {"sh", 'I', 0, 24,HandleEI},
                {"jmp", 'J', 0, 30,HandleJMP},
                {"la", 'J', 0, 31,HandleLA},
                {"call", 'J', 0, 32,HandleLA},
                {"stop", 'J', 0, 63,HandleSTOP},
        };

short handleInstruction(short index, char* newline,symbol* head,unsigned long *ic,unsigned int **ci){
    unsigned int cmd;
    unsigned long cmdindex;
    cmd = instructions[index].pfunc(newline,index,head,*ic);
    if (cmd <= 20) /* If in range of error codes */
        return cmd; /* cmd will hold error code */

    cmdindex = (*ic-100)/4; /* instruction counter will always be index*4 and starts from 100 */
    if (cmdindex%50==0 && cmdindex > 0)
        *ci = realloc(ci,(cmdindex*(sizeof (int))+(sizeof (int))*51));
    ci[0][cmdindex] = cmd;
    *ic += 4;   /* add 4 bytes to ic */
    return VALID;
}


/* Returns index of cmd in table, or -1 if not found */
short identifyCMD(char* word){
    short i = 0;
    while(strcmp(word,instructions[i].name) && i < COMMAND_BUF_SIZE){ /* check if code command */
        i++;
    }
    if (i < COMMAND_BUF_SIZE)
        return i;       /* return index in cmd table */
    else
        return ERROR;
}

/* Parse erithmetic R instructions */
unsigned HandleER(char* line,short index,symbol* head,unsigned long ic) {
    unsigned rs,rt,rd,cmd;
    char* oper;

    oper = getNextWord(&line,OPERDELIM);
    if (oper == NULL) {
        return MISSINGOPERANDS;
    }
    if (*oper == '\0') {
        return EMPTYOPERAND;
    }
    rs = convertRegister(oper);

    oper = getNextWord(&line,OPERDELIM);
    if (oper == NULL) {
        return MISSINGOPERANDS;
    }
    if (*oper == '\0') {
        return EMPTYOPERAND;
    }
    rt = convertRegister(oper);

    oper = getNextWord(&line,OPERDELIM);
    if (oper == NULL) {
        return MISSINGOPERANDS;
    }
    if (*oper == '\0') {
        return EMPTYOPERAND;
    }

    rd = convertRegister(oper);

    if (rs == REGERROR || rt == REGERROR || rd == REGERROR){
        return ILLEGALOPERAND;
    }

    oper = getNextWord(&line,OPERDELIM); /* Check for more operands */
    if (oper != NULL) {
        return TOOMANYOPERANDS;
    }

    cmd = makeR(instructions[index].opcode,rs,rt,rd,instructions[index].funct);
    return cmd;
}

/* Parse Move R instructions */
unsigned HandleMV(char* line,short index,symbol* head,unsigned long ic) {
    unsigned rs,rd,cmd;
    char* oper;

    oper = getNextWord(&line,OPERDELIM);
    if (oper == NULL) {
        return MISSINGOPERANDS;
    }
    if (*oper == '\0') {
        return EMPTYOPERAND;
    }
    rs = convertRegister(oper);

    oper = getNextWord(&line,OPERDELIM);
    if (oper == NULL) {
        return MISSINGOPERANDS;
    }
    if (*oper == '\0') {
        return EMPTYOPERAND;
    }
    rd = convertRegister(oper);

    if (rs == REGERROR || rd == REGERROR){
        return ILLEGALOPERAND;
    }

    oper = getNextWord(&line,OPERDELIM); /* Check for more operands */
    if (oper != NULL) {
        return TOOMANYOPERANDS;
    }

    cmd = makeR(instructions[index].opcode,rs,0,rd,instructions[index].funct);
    return cmd;
}

/* Parse Erithmetic I instructions */
unsigned HandleEI(char* line,short index,symbol* head,unsigned long ic) {
    unsigned rs,rt,cmd;
    char* oper;
    signed immed;
    short error;

    oper = getNextWord(&line,OPERDELIM);
    if (oper == NULL) {
        return MISSINGOPERANDS;
    }
    if (*oper == '\0') {
        return EMPTYOPERAND;
    }
    rs = convertRegister(oper);

    oper = getNextWord(&line,OPERDELIM);
    if (oper == NULL) {
        return MISSINGOPERANDS;
    }
    if (*oper == '\0') {
        return EMPTYOPERAND;
    }
    error = convertNum(oper,INT_TYPE,&immed);
    if (error != VALID)
        return error;

    oper = getNextWord(&line,OPERDELIM);
    if (oper == NULL) {
        return MISSINGOPERANDS;
    }
    if (*oper == '\0') {
        return EMPTYOPERAND;
    }
    rt = convertRegister(oper);

    if (rs == REGERROR || rt == REGERROR || immed == CMDERROR){
        return ILLEGALOPERAND;
    }

    oper = getNextWord(&line,OPERDELIM); /* Check for more operands */
    if (oper != NULL) {
        return TOOMANYOPERANDS;
    }

    cmd = makeI(instructions[index].opcode,rs,rt,immed);
    return cmd;
}

/* Parse Boolean I instructions */
unsigned HandleBI(char* line,short index,symbol* head,unsigned long ic) {
    unsigned rs,rt,cmd;
    char* oper;
    signed immed;
    symbol* symb;
    oper = getNextWord(&line,OPERDELIM);
    if (oper == NULL) {
        return MISSINGOPERANDS;
    }
    if (*oper == '\0') {
        return EMPTYOPERAND;
    }
    rs = convertRegister(oper);
    oper = getNextWord(&line,OPERDELIM);
    if (oper == NULL) {
        return MISSINGOPERANDS;
    }
    if (*oper == '\0') {
        return EMPTYOPERAND;
    }

    rt = convertRegister(oper);
    oper = getNextWord(&line,OPERDELIM);

    if (oper == NULL) {
        return MISSINGOPERANDS;
    }
    if (*oper == '\0') {
        return EMPTYOPERAND;
    }
    symb = getLabel(oper,head);
    if (symb == NULL || symb->type == EXTERN) /* if label doesn't exist or if it's extern */
        return INVALIDLABEL;

    if (rs == REGERROR || rt == REGERROR){
         return ILLEGALOPERAND;
    }

    immed = symb->value - ic;

        oper = getNextWord(&line,OPERDELIM); /* Check for more operands */
        if (oper != NULL) {
            return TOOMANYOPERANDS;
        }

    cmd = makeI(instructions[index].opcode,rs,rt,immed);
    return cmd;
}

unsigned HandleJMP(char* line,short index,symbol* head,unsigned long ic){
    unsigned reg,cmd,address;
    char* oper;
    symbol *label;
    oper = getNextWord(&line,CMDDELIM);
    if (oper == NULL) {
        return MISSINGOPERANDS;
    }
    if (*oper == '\0') {
        return EMPTYOPERAND;
    }

    address = convertRegister(oper);
    reg = 1;
    if (address == REGERROR) { /* if error it might be a label */
        label = getLabel(oper, head);
        if (label == NULL)
            return ILLEGALOPERAND;
        address = label->value;
        reg = 0;
        if (label->type == EXTERN)
            addAddressToExtern(label, ic);
    }

    oper = getNextWord(&line,OPERDELIM); /* Check for more operands */
    if (oper != NULL) {
        return TOOMANYOPERANDS;
    }

    cmd = makeJ(instructions[index].opcode,reg,address);
    return cmd;
}

/* Handles la and call */
unsigned HandleLA(char* line,short index,symbol* head,unsigned long ic){
    unsigned cmd,address;
    char* name;
    symbol* label;
    name = getNextWord(&line,CMDDELIM); /* get label Operand */

    if (name == NULL) {
        return MISSINGOPERANDS;
    }

    label = getLabel(name,head);
    if (label != NULL)
        address = label->value;
    else {
        return LABELNOTFOUND;
    }
    addAddressToExtern(label,ic);

    name = getNextWord(&line,OPERDELIM); /* Check for more operands */
    if (name != NULL) {
        return TOOMANYOPERANDS;
    }


    cmd = makeJ(instructions[index].opcode,0,address);
    return cmd;
}

unsigned HandleSTOP(char* line,short index,symbol* head,unsigned long ic){
    unsigned cmd;
    char* oper;

    oper = getNextWord(&line,OPERDELIM); /* Check for more operands */
    if (oper != NULL) {
        return TOOMANYOPERANDS;
    }

    cmd = makeJ(instructions[index].opcode,0,0);
    return cmd;
}

/* Convert the R instruction to machine code */
unsigned makeR(unsigned opcode,unsigned rs,unsigned rt,unsigned rd,unsigned funct){
    R cmd = {opcode , rs,rt , rd, funct,0};
    unsigned int MCcommand = 0; /* command converted to machine code */
    unsigned int temp;

    /* push each part of R command bits to machine code command */
    temp = cmd.opcode;
    MCcommand |=  (temp << 26);

    temp = cmd.rs;
    MCcommand |= (temp << 21);

    temp = cmd.rt;
    MCcommand |= (temp << 16);

    temp = cmd.rd;
    MCcommand |= (temp << 11);

    temp = cmd.funct;
    MCcommand |= (temp << 6);

    temp = cmd.notused;
    MCcommand |= (temp);

    return MCcommand;
}

/* Convert the I instruction to machine code */
unsigned makeI(unsigned opcode,unsigned rs,unsigned rt,signed immed){
    I cmd = {opcode , rs, rt, immed};
    unsigned int MCcommand = 0; /* command converted to machine code */
    unsigned int temp;
    unsigned short stemp;
    /* push each part of I command bits to machine code command */
    temp = cmd.opcode;
    MCcommand |=  (temp << 26);

    temp = cmd.rs;
    MCcommand |= (temp << 21);

    temp = cmd.rt;
    MCcommand |= (temp << 16);

    stemp = cmd.immed;
    MCcommand |= (stemp);

    return MCcommand;
}

/* Convert the J instruction to machine code */
unsigned makeJ(unsigned opcode,unsigned reg,unsigned address){
    J cmd = {opcode , reg, address};
    unsigned int MCcommand = 0; /* command converted to machine code */
    unsigned int temp;

    /* push each part of I command bits to machine code command */
    temp = cmd.opcode;
    MCcommand |=  (temp << 26);

    temp = cmd.reg;
    MCcommand |= (temp << 25);

    temp = cmd.address;
    MCcommand |= (temp);

    return MCcommand;
}


