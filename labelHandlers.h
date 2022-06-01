
#ifndef ASSEMBLER_LABELHANDLERS_H
#define ASSEMBLER_LABELHANDLERS_H

#include "common.h"

/* For external command addresses */
typedef struct address {
    unsigned long value;
    struct address* next;
} addresses;

/* Symbol Table */
typedef struct labelnode {
    char name[31];
    unsigned long value;
    addresses* address;
    unsigned short type; 	/* default = 0, entry = 1, extern = 2 */
    unsigned short img; 	/* data = 0, code = 1, extern = 2 */
    struct labelnode* next;
} symbol;


symbol* getLabel(char* name,symbol* head);

short checkSavedWord(char* word);   /* Checks if a command */

short addLabel(char* name,unsigned long *val,short type,short img,symbol** head);

short addEntryLabel(char* name,symbol** head);

short freeSymbols(symbol** curr); /* Free all allocated symbols */

short addICtoAddresses(unsigned long ic,symbol* head); /* add IC to all label addresses */

short addAddressToExtern(symbol* label,unsigned long value); /* add address where external labels are used */

short validateLabel (char* word);

#endif
