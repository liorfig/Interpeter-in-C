#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "assembler.h"
#include "errorDefs.h"

 /* Error Array */
char* errors[] = {VALID,
                  ENTRYNOLABELTEXT,
                  LINETOOLONGTEXT,
                  UNRECOGNIZEDCMDTEXT,
                  INVALIDLABELTEXT,
                  LABELEXISTSTEXT,
                  LABELSAVEDWORDTEXT,
                  ENTRYLABELDONTEXISTTEXT,
                  ENTRYDEFINEDEXTERNTEXT,
                  WRONGFILEEXTENSIONTEXT,
                  EXTERNNOLABELTEXT,
                  NOTENOUGHMEMORYTEXT,
                  MISSINGOPERANDSTEXT,
                  FILEDOESNOTEXISTTEXT,
                  NOMEMORYTEXT,
                  ILLEGALOPERANDTEXT,
                  LABELTOOLONGTEXT,
                  LABELNOTFOUNDTEXT,
                  TOOMANYOPERANDSTEXT,
                  EMPTYOPERANDTEXT,
                  NUMTOOBIGTEXT};

int main(int argc, char *argv[]) {
    short error;

    /* Images */
    unsigned char *di; /* Data Image */
    unsigned int *ci; /* Code Image */

   /* Counters */
    unsigned long ic; /* Instruction Counter*/
    unsigned long dc; /* Data Counter*/

    /* Symbol Table */
    symbol* head;

    int i;
    for (i = 1;i < argc; i++) {
        if (strcmp(&(argv[i][strlen(argv[i])-3]),".as") != 0){
            errorHandler(WRONGFILEEXTENSION,0);
            continue;
        }
        else {
            FILE *fp = fopen(argv[i], "r");
            if (fp == NULL){
                errorHandler(FILEDOESNOTEXIST,0);
            }

            /* init counters */
            ic = IC_START;
            dc = DC_START;

            /* init Images */
            ci = malloc(CI_BUFFER);
            if (ci == NULL){
                errorHandler(NOMEMORY,0);
                return ERROR;
            }
            di = malloc(DI_BUFFER);
            if (di == NULL){
                errorHandler(NOMEMORY,0);
                return ERROR;
            }
            /* init Symbol table */
            head = NULL;

            error = Reader(fp, 1,&head,&di,&ci,&dc,&ic);
            if (error == VALID) {
                rewind(fp);
                addICtoAddresses(ic,head);   /* Add IC value to data label addresses */
                ic = IC_START; /* Restart counter for cmd index */
                error = Reader(fp, 2, &head, &di, &ci, &dc, &ic);
                if (error == VALID) {
                    outputMain(ci, di, ic, dc, argv[i]);
                    outputEntry(head,argv[i]);
                    outputExtern(head,argv[i]);
                }
            }
            free(ci);
            free(di);
            freeSymbols(&head);
            free(fp);
        }
    }
    return VALID;
}

short Reader(FILE *fp,short iteration,symbol** head,unsigned char **di,unsigned int **ci, unsigned long *dc, unsigned long *ic){
    int linenum = 0;
    short error;
    short iferror = VALID;
    char line[MAX_LINE_LEN+2];

    while (fgets(line, MAX_LINE_LEN+1, fp) != NULL) { /* iterate through file */
        linenum++;      /* update line number */

        /* If line too long */
        if (strlen(line) > MAX_LINE_LEN) {
            error = LINETOOLONG;
        }

            /* If comment or empty */
        else if (emptyOrComment(line) == SKIP){
            continue;
        }
            /* If valid line */
        else if (iteration == 1){
            error = firstLineParser(line, head, di, ci, dc, ic);
        }
        else if (iteration == 2){
            error = secondLineParser(line, head, di, ci, dc, ic);
        }
        if (error != VALID) /* Print errors if found */{
            errorHandler(error, linenum);
            iferror = ERROR;
        }
    }
    return iferror;
}

short emptyOrComment(char* line){

    while(*line != '\n' && *line != EOF){
        if(!(isspace(*line))){
            if(*line == ';')
                return SKIP;
            else
                return READ;
        }
        line++;
    }
    return SKIP;
}

short outputMain(unsigned int *ci,unsigned char *di,unsigned long ic,unsigned long dc,char* name){
    FILE *out;
    unsigned int counter = 100;
    unsigned int i=0;
    unsigned int cmdindex = (ic-100)/4;
    int j;
    char* newname;

    changeEx(name,OB,&newname);
    out = fopen(newname,"w");
    if (out == NULL)
        printf("why");
    free(newname);
    fprintf(out,"     %02lu %02lu \n",(ic-100),(dc)); /* print amount of code bytes and then data bytes */

    if (ic > 100){     /* print code image if there is code */
    while(i < cmdindex){
        fprintf(out,"%04d ",counter); /* print counter */
        counter += 4;

        for(j=0;j<4;j++) { /* print each command in code image */
            fprintf(out,"%X",(ci[i] & 0xF0) >> 4); /* high nibble */
            fprintf(out,"%X ",ci[i] & 0x0F); /* low nibble */
            ci[i] >>= 8;
        }
        fprintf(out,"\n");
        i++;
    }}
    i=0;
    if(*di > 0){ /* print data image */
    while(i<=dc){
        fprintf(out,"%04d ",counter); /* print counter */
        counter += 4;
        for (j=0;j < 4;j++) {
            if (i+j >= dc)
                break;
            fprintf(out,"%02X ",di[i+j]);
        }
        fprintf(out,"\n");
        i+=4;
    }}
    fclose(out);
    return VALID;
}

short outputExtern(symbol* head, char* name){
    FILE *out;
    int count = 0;

    char* newname;
    changeEx(name,EXT,&newname);

    out = fopen(newname,"w");

    if (head != NULL) {
        while (head->next != NULL) {
            if (head->type == EXTERN){
                count += printAddresses(out,head);
            }
            head = head->next;
        }
        if (head->type == EXTERN){
            count += printAddresses(out,head);
        }
    }
    fclose(out);
    if (count==0)
        remove(newname);
    free(newname);
    return VALID;

}
short outputEntry(symbol* head, char* name){
    FILE *out;

    int count = 0;
    char* newname;
    changeEx(name,ENT,&newname);

    out = fopen(newname,"w");

    if (head != NULL) {
        while (head->next != NULL) {
            if (head->type == ENTRY)
                fprintf(out,"%s %04lu \n",head->name,head->value);
            count++;
            head = head->next;
        }
        if (head->type == ENTRY){
            count++;
            fprintf(out,"%s %04lu ",head->name,head->value);
        }
    }
    fclose(out);
    if (count==0)
        remove(newname);
    free(newname);
    return VALID;
}

short errorHandler(short errorcode,unsigned int linenum){
    char errortext[MAX_ERROR_LENGTH];
    strcpy(errortext,errors[errorcode]);
    if (linenum != 0)
        fprintf(stderr,"%s, in line: %d\n",errortext,linenum);
    else
        fprintf(stderr,"%s",errortext);
    return VALID;
}

short changeEx(char* name,char* extension, char** newname){
    *newname = (char*) calloc(strlen(name)+2,sizeof(char));
    if (*newname==NULL)
        return ERROR;
    strcpy(*newname,name);
    strtok(*newname,DOT);
    strcat(*newname,extension);
    return VALID;
}

int printAddresses(FILE* out,symbol* label){
    addresses *current = label->address;
    int count = 0;
    if(current != NULL){
        while(current->next != NULL){
            fprintf(out,"%s %04lu\n",label->name,current->value);
            current = current->next;
            count++;
        }
        fprintf(out,"%s %04lu\n",label->name,current->value);
        count++;
    }
    return count;
}
