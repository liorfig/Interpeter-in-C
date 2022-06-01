#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "directiveHandlers.h"
#include "instructionHandlers.h"
#include "parser.h"
#include "errorDefs.h"
#include "common.h"

short firstLineParser(char* line,symbol** head,unsigned char **di,unsigned int **ci, unsigned long *dc, unsigned long *ic){
    short error = VALID;
    char* label = NULL;
    short type; /* type identifier */
    unsigned long zp;  /* used for pointer to 0 in case of extern */
    char* word;

    word = getNextWord(&line, CMDDELIM); /* Extract first word */

    /* if is a label */
    if (word[strlen(word)-1] == ':') {
        error = validateLabel((char *) word);
        if (error != VALID) {
            return error;
        }
        label = malloc(strlen(word)+1);
        strcpy(label,word);

        if (line == NULL) /* If line ended */
            return MISSINGOPERANDS;

        label[strlen(label)-1] = '\0'; /* replace ':' with \0 */
        word = getNextWord(&line, CMDDELIM); /* Extract first word */
    }


    /* If it's a directive command */
    if (word[0] == '.'){

        /* case of .entry */
        if (strcmp(word,".entry") == 0){
            return VALID;   /* go back to reader, will handle this in second read. */
        }

        /* case of .extern */
        if (strcmp(word,".extern") == 0){
            if (label != NULL) {
            }
            label = getNextWord(&line, CMDDELIM); /* Extract next word (should be label) */

            if (label == NULL){
                return EXTERNNOLABEL;
            }
            error = validateLabel((char *) label); /* Validate label */
            if (error != VALID) {
                return error;
            }
            zp = 0; /* transfer pointer to 0 */
            addLabel(label, &zp,EXTERN, EXTERN,head);

            label = getNextWord(&line, CMDDELIM); /* Check for more arguments */
            if (label != NULL)
                return TOOMANYOPERANDS;

            return VALID; /* Go to next line */
        }

        /* case of data cmd */
        type = identifyDirective(word);
        if (type == ERROR){ /* if wasn't any of the above and ended with '.' */
            error = UNRECOGNIZEDCMD;
            return error;
        }
        if (label != NULL)
            addLabel(label, dc, DATA, DEFAULT,head);  /* Add label if included */
        return handleDirective(type,line,di,dc);
    }

    /* If it's an instruction cmd */
    type = identifyCMD(word);
    if (type == ERROR){     /* if wasn't any of the above */
        error = UNRECOGNIZEDCMD;
        return error;
    }
    if (label != NULL) {
        error = addLabel(label, ic, DEFAULT, CODE, head);   /* Add label if included */
        if (error != VALID)
            return error;
    }
    *ic += 4;   /* add 4 bytes to ic */
    return VALID;
}

short secondLineParser(char* line,symbol** head,unsigned char **di,unsigned int **ci, unsigned long *dc, unsigned long *ic){
    short error = VALID;
    char* label = NULL;
    short type; /* type identifier */

    char* word;

    word = getNextWord(&line, CMDDELIM); /* Extract first word */

    /* if is a label*/
    if (word[strlen(word)-1] == ':') {
        label = malloc(strlen(word)+1);
        strcpy(label,word);

        word = getNextWord(&line,CMDDELIM); /* Extract next word */
    }

    /* If it's a directive command */
    if (word[0] == '.') {
        /* case of .entry */
        if (strcmp(word, ".entry")==0) {
            label = getNextWord(&line,CMDDELIM);
            if (label == NULL) { /* if no label after cmd */
                return ENTRYNOLABEL;
            }

            error = addEntryLabel(label,head);
            if (error != VALID)
                return error;
            return VALID;   /* done with line */
        }
    }

    /* If it's an instruction cmd */
    type = identifyCMD(word);
    if (type == ERROR){     /* Should be instruction, if was unidentified cmd would be found in first read*/
        return VALID;
    }
    error = handleInstruction(type,line,*head,ic,ci);
    if (error != VALID)
        return error;
    return VALID;
}

char* getNextWord(char** line,char delim){
    char* word;
    if (*line == NULL)
        return NULL;
    while (isspace(**line) && **line != '\n' && **line != EOF){ /* skip whitespace */
        (*line)++;
    }
    word = *line; /* place pointer after whitespace */
    while (**line != delim && **line != EOF && !isspace(**line)){
        (*line)++;
    }
    if (**line != '\n' && **line != EOF) {
        **line = '\0';
        (*line)++;
    }
    else {
        **line = '\0';
        *line = NULL; /* nullifies pointer if line ended */
    }
    return word;
}

