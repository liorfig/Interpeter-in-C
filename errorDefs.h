
#ifndef ASSEMBLER_ERRORDEFS_H
#define ASSEMBLER_ERRORDEFS_H

#define VALID 0
#define ERROR -1 /* Generic error */

#define CMDERROR 0 /* because type unsigned can't be -1 */
#define REGERROR 33 /* because register goes upto 32 */

#define ENTRYNOLABEL 1
#define ENTRYNOLABELTEXT "No label found after .entry directive"
#define LINETOOLONG 2
#define LINETOOLONGTEXT "Line exceeds maximum allowed length"
#define UNRECOGNIZEDCMD 3
#define UNRECOGNIZEDCMDTEXT "Unrecognized Command"
#define INVALIDLABEL 4
#define INVALIDLABELTEXT "Invalid label"
#define LABELEXISTS 5
#define LABELEXISTSTEXT "Label already exists"
#define LABELSAVEDWORD 6
#define LABELSAVEDWORDTEXT "Label is a saved word"
#define ENTRYLABELDONTEXIST 7
#define ENTRYLABELDONTEXISTTEXT ".entry directive on label that isn't defined"
#define ENTRYDEFINEDEXTERN 8
#define ENTRYDEFINEDEXTERNTEXT ".entry directive on label defined as external"
#define WRONGFILEEXTENSION 9
#define WRONGFILEEXTENSIONTEXT "input file extension should be .as!"
#define EXTERNNOLABEL 10
#define EXTERNNOLABELTEXT ".extern directive with no label"
#define NOTENOUGHMEMORY 11
#define NOTENOUGHMEMORYTEXT "Not enough space for image"
#define EXTERNNOLABELTEXT ".extern directive with no label"
#define MISSINGOPERANDS 12
#define MISSINGOPERANDSTEXT "Expected more operands"
#define FILEDOESNOTEXIST 13
#define FILEDOESNOTEXISTTEXT "File wasn't found"
#define NOMEMORY 14
#define NOMEMORYTEXT "Not enough memory to allocate"
#define ILLEGALOPERAND 15
#define ILLEGALOPERANDTEXT "Illegal operands used"
#define LABELTOOLONG 16
#define LABELTOOLONGTEXT "Label length over limit"
#define LABELNOTFOUND 17
#define LABELNOTFOUNDTEXT "Label not found"
#define TOOMANYOPERANDS 18
#define TOOMANYOPERANDSTEXT "Too many operands given"
#define EMPTYOPERAND 19
#define EMPTYOPERANDTEXT "Empty Operand"
#define NUMTOOBIG 20
#define NUMTOOBIGTEXT "Given operand too large to store in requested data space"

#define MAX_ERROR_LENGTH 80

#endif
