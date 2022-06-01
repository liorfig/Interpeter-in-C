#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "instructionHandlers.h"
#include "directiveHandlers.h"
#include "errorDefs.h"
#include "common.h"

/* Validate if legal label */
short validateLabel (char* word){
    int i;
    int length = strlen(word);
    char* temp;
    temp = malloc(length+1);
    strcpy(temp,word);
    if (temp[length-1]==':') { /* if label included with ':' */
        temp[length-1]='\0'; /* remove ':' */
        length--;
    }
    if (length > 31) {   /* validate length of label */
        free(temp);
        return LABELTOOLONG;
    }
    if (length == 0){   /* if label is empty */
        free(temp);
        return INVALIDLABEL;
    }
    for (i = 0; i < length; i++) {
        if (!((isdigit(temp[i]) && i > 0) || isalpha(temp[i]))) {    /* make sure first index is only alphabet, and that all are alphabet or digits*/
            if (!((isspace(temp[i])) && i == length-1)) { /*if whitespace character after label */
                free(temp);
                return INVALIDLABEL;
            }
        }
    }
    if (checkSavedWord(temp) == ERROR) {
        free(temp);
        return LABELSAVEDWORD;
    }
    free(temp);
    return VALID;
}

/* Checks if a command */
short checkSavedWord(char* word){
    short error;
    error = identifyCMD(word);
    if (error != ERROR) /* Should be error, because shouldn't identify */
        return ERROR;
    error = identifyDirective(word);
    if (error != ERROR) /* Should be error, because shouldn't identify */
        return ERROR;
    return VALID;
}

short addLabel(char* name,unsigned long *val,short type,short img,symbol** head){
    symbol *current;
    symbol* newnode;
    if(*head == NULL){
        (*head) = (symbol*)malloc(sizeof(symbol));
        (*head)->type=type;
        (*head)->img=img;
        strcpy((*head)->name,name);
        (*head)->value=*val;
        (*head)->next=NULL;
        (*head)->address=NULL;
        return VALID;
    }
    current = *head;
    if (strcmp(current->name,name) == 0) /* if label already exists */
        return LABELEXISTS;
    while(current->next != NULL){
        if (strcmp(current->name,name) == 0) /* if label already exists */
            return LABELEXISTS;
        current = current->next;
    }
    newnode = (symbol*)malloc(sizeof(symbol));
    newnode->type=type;
    newnode->img=img;
    strcpy(newnode->name,name);
    newnode->value=*val;
    newnode->address=NULL;
    newnode->next=NULL;

    current->next = newnode;
    return VALID;
}

short addEntryLabel(char* name,symbol** head){
    symbol* label;
    label = getLabel(name,*head);
    if (label == NULL)       /* If label isn't defined in file */
        return ENTRYLABELDONTEXIST;
    if (label->type==EXTERN)     /* If label is already defined as external */
        return ENTRYDEFINEDEXTERN;

    label->type = ENTRY;
    return VALID;
}

short addAddressToExtern(symbol* label,unsigned long value){
    addresses *current;
    addresses *newnode;
    if (label->address == NULL){
        label->address = (addresses *)malloc(sizeof(addresses));
        label->address->value = value;
        label->address->next = NULL;
        return VALID;
    }
    current = label->address;

    while (current->next != NULL) {
            current = current->next;
    }
    newnode = (addresses *)malloc(sizeof(addresses));
    newnode->value = value;
    newnode->next = NULL;

    current->next = newnode;
    return VALID;
}

symbol* getLabel(char* name,symbol* head){
    symbol *current;
    current = head;
    if (head == NULL)
        return NULL;
    if (strcmp(head->name,name) == 0)
        return current;
    while(current->next != NULL){
        current = current->next;
        if (strcmp(current->name,name) == 0){
            return current;
        }
    }
    return NULL;
}

short addICtoAddresses(unsigned long ic,symbol* head){
    if (head != NULL) {
        while (head->next != NULL) {
            if (head->img == DATA)
                head->value+=ic;
            head = head->next;
        }
        if (head->img == DATA)
            head->value+=ic;
    }
    return VALID;
}

short freeSymbols(symbol** curr){
        symbol *temp;
    if (*curr != NULL) {
        temp = *curr;
        while ((*curr)->next != NULL) {
            *curr = (*curr)->next;
            free(temp);
            temp = *curr;
        }
        free(*curr);
    }
    return VALID;
}

