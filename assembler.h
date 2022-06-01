
#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H

#define	MAX_LINE_LEN 81 /* including '\n' */

#define	DC_START 0
#define	IC_START 100

#define	DI_BUFFER 100
#define	CI_BUFFER (sizeof (int))*51

#define SKIP 0
#define READ 1

#define DOT "."
#define OB ".ob"
#define EXT ".ext"
#define ENT ".ent"

short Reader(FILE *fp,short iteration,symbol** head,unsigned char **di,unsigned int **ci, unsigned long *dc, unsigned long *ic); /* Reads through file */
short emptyOrComment(char* line); /* Checks whether line is empty or comment */
short errorHandler(short errorcode,unsigned int linenum); /* Identify and print errors to stderr */
short changeEx(char* name,char* extension, char** newname); /* Changes extension from .as to required */

short outputMain(unsigned int *ci,unsigned char *di,unsigned long ic,unsigned long dc,char* name);
short outputEntry(symbol* head, char* name);
short outputExtern(symbol* head, char* name);
int printAddresses(FILE* out,symbol* label); /* Returns number of addresses printed */

#endif
