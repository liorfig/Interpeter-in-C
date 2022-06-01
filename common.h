
#ifndef ASSEMBLER_COMMON_H
#define ASSEMBLER_COMMON_H

#define       SCHAR_TYPE  1
#define       SHRT_TYPE     2
#define       INT_TYPE      3
#define       UINT_TYPE     4

/* type definitions */
#define DEFAULT 0
#define ENTRY 1
#define EXTERN 2
#define DATA 0
#define CODE 1


short isValidNum(double num,int type);
unsigned convertRegister(char *oper); /* get next register */
short convertNum(char *oper,short type,int *num); /* get next immed number */

#endif
