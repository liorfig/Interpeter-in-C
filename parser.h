
#ifndef ASSEMBLER_PARSER_H
#define ASSEMBLER_PARSER_H

#define CMDDELIM ' ' /* Whitespace Delimiter */
#define OPERDELIM ',' /* Operand Delimiter */

#include "labelHandlers.h"
/* Parses line */
short firstLineParser(char* line,symbol** head,unsigned char **di,unsigned int **ci, unsigned long *dc, unsigned long *ic);
short secondLineParser(char* line,symbol** head,unsigned char **di,unsigned int **ci, unsigned long *dc, unsigned long *ic);
char* getNewline(char* oldline); /* Returns pointer to rest of line after next word */
char* getNextWord(char** line,char delim); /* Returns pointer to first word in given string, and sets line pointer from end of returned word */

#endif
