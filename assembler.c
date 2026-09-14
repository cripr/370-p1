/**
 * Project 1
 * Assembler code fragment for LC-2K
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//Every LC2K file will contain less than 1000 lines of assembly.
#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
static void checkForBlankLinesInCode(FILE *inFilePtr);
static inline int isNumber(char *);
static inline void printHexToFile(FILE *, int);
static int endsWith(char *, char *);

struct Label_addr {
    int pc;
    char label1[MAXLINELENGTH];
};

// helper function declarations
uint32_t bit_opcode(char *opcode);


// main function
int
main(int argc, char **argv)
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

    if (!endsWith(inFileString, ".as") &&
        !endsWith(inFileString, ".s") &&
        !endsWith(inFileString, ".lc2k")
    ) {
        printf("warning: assembly code file does not end with .as, .s, or .lc2k\n");
    }

    if (!endsWith(outFileString, ".mc")) {
        printf("error: machine code file must end with .mc\n");
        exit(1);
    }

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }

    // Check for blank lines in the middle of the code.
    checkForBlankLinesInCode(inFilePtr);

    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    //my code goes under here

    int PC = 0;

    struct Label_addr labels[MAXLINELENGTH];
    int num_labels = 0;

    // 1st run: read and store labels' addresses
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
        if (label[0] != '\0') {
            strcpy(labels[num_labels].label1, label);
            labels[num_labels].pc = PC;
            num_labels++;
        }
        PC++;
    }

    // 2nd run: translate all lines to machine code

    PC = 0;
    uint32_t total_bit_reps[MAXLINELENGTH];
    int num_inst = 0;
    

    rewind(inFilePtr);
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
        uint32_t inst_bits = 0;

        //.fill handling
        if(!strcmp(opcode, ".fill")) {
            if (isNumber(arg0)) {
                inst_bits = atoi(arg0);
            }
            else {
                for (int i = 0; i < num_labels; i++) {
                    if (!strcmp(labels[i].label1, arg0)) {
                        inst_bits = labels[i].pc;
                    }
                }
            }
        }

        // rest of instructions
        else {
            uint32_t op_type = bit_opcode(opcode);
            //beq handling
            if(op_type == 4) {
                int offset = atoi(arg2);
                for (int i = 0; i < num_labels; i++) {
                    if (!strcmp(labels[i].label1, arg2)) {
                        offset = labels[i].pc - (PC + 1);
                    }
                }
                inst_bits = (inst_bits << 3) | op_type;
                inst_bits = (inst_bits << 3) | atoi(arg0);
                inst_bits = (inst_bits << 3) | atoi(arg1);
                inst_bits = (inst_bits << 16) | offset;
            }
            else if(op_type == 0 || op_type == 1) {
                inst_bits  = (inst_bits << 3) | op_type;
                inst_bits = (inst_bits << 3) | atoi(arg1);
                inst_bits = (inst_bits << 3) | atoi(arg2);
                inst_bits = (inst_bits << 13) | 0;
                inst_bits = (inst_bits << 3) | atoi(arg0);
            }
            else if(op_type == 2 || op_type == 3) {
                int offset = atoi(arg2);
                for (int i = 0; i < num_labels; i++) {
                    if (!strcmp(labels[i].label1, arg2)) {
                        offset = labels[i].pc;
                    }
                }
                inst_bits = (inst_bits << 3) | op_type;
                inst_bits = (inst_bits << 3) | atoi(arg0);
                inst_bits = (inst_bits << 3) | atoi(arg1);
                inst_bits = (inst_bits << 16) | offset;
            }
            else if(op_type == 5) {
                inst_bits = (inst_bits << 3) | op_type;
                inst_bits = (inst_bits << 3) | atoi(arg1);
                inst_bits = (inst_bits << 3) | atoi(arg2);
                inst_bits = (inst_bits << 16) | 0;
            } 
            else if(op_type == 6 || op_type == 7) {
                inst_bits = (inst_bits << 3) | op_type;
                inst_bits = (inst_bits << 22) | atoi(arg0);
            }
        }
        total_bit_reps[num_inst] = inst_bits;
        num_inst++;
        PC++;
    }

    
    for(int i = 0; i < num_inst; i++) {
        printHexToFile(outFilePtr, total_bit_reps[i]);
    }


    //my code ends here

    /* here is an example for how to use readAndParse to read a line from
        inFilePtr */
    //if (! readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
    //    /* reached end of file */
    //}
//
    ///* this is how to rewind the file ptr so that you start reading from the
    //    beginning of the file */
    //rewind(inFilePtr);
//
    ///* after doing a readAndParse, you may want to do the following to test the
    //    opcode */
    //if (!strcmp(opcode, "add")) {
    //    /* do whatever you need to do for opcode "add" */
    //}
//
    ///* here is an example of using isNumber. "5" is a number, so this will
    //   return true */
    //if(isNumber("5")) {
    //    printf("It's a number\n");
    //}
//
    ///* here is an example of using printHexToFile. This will print a
    //   machine code word / number in the proper hex format to the output file */
    //printHexToFile(outFilePtr, 123);

    return(0);
}

//MY HELPER FUNCTIONS

uint32_t bit_opcode(char *opcode) {
    if(!strcmp(opcode, "add")) {return 0;}
    if(!strcmp(opcode, "nor")) {return 1;}
    if(!strcmp(opcode, "lw")) {return 2;}
    if(!strcmp(opcode, "sw")) {return 3;}
    if(!strcmp(opcode, "beq")) {return 4;}
    if(!strcmp(opcode, "jalr")) {return 5;}
    if(!strcmp(opcode, "halt")) {return 6;}
    if(!strcmp(opcode, "noop")) {return 7;}
    exit(1);
}



//my helper functions end here

// Returns non-zero if the line contains only whitespace.
static int lineIsBlank(char *line) {
    char whitespace[4] = {'\t', '\n', '\r', ' '};
    int nonempty_line = 0;
    for(int line_idx=0; line_idx < strlen(line); ++line_idx) {
        int line_char_is_whitespace = 0;
        for(int whitespace_idx = 0; whitespace_idx < 4; ++ whitespace_idx) {
            if(line[line_idx] == whitespace[whitespace_idx]) {
                line_char_is_whitespace = 1;
                break;
            }
        }
        if(!line_char_is_whitespace) {
            nonempty_line = 1;
            break;
        }
    }
    return !nonempty_line;
}

// Exits 2 if file contains an empty line anywhere other than at the end of the file.
// Note calling this function rewinds inFilePtr.
static void checkForBlankLinesInCode(FILE *inFilePtr) {
    char line[MAXLINELENGTH];
    int blank_line_encountered = 0;
    int address_of_blank_line = 0;
    rewind(inFilePtr);

    for(int address = 0; fgets(line, MAXLINELENGTH, inFilePtr) != NULL; ++address) {
        // Check for line too long
        if (strlen(line) >= MAXLINELENGTH-1) {
            printf("error: line too long\n");
            exit(1);
        }

        // Check for blank line.
        if(lineIsBlank(line)) {
            if(!blank_line_encountered) {
                blank_line_encountered = 1;
                address_of_blank_line = address;
            }
        } else {
            if(blank_line_encountered) {
                printf("Invalid Assembly: Empty line at address %d\n", address_of_blank_line);
                exit(2);
            }
        }
    }
    rewind(inFilePtr);
}


/*
* NOTE: The code defined below is not to be modifed as it is implimented correctly.
*/

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int
readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
    char *arg1, char *arg2)
{
    char line[MAXLINELENGTH];
    char *ptr = line;

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
	/* reached end of file */
        return(0);
    }

    /* check for line too long */
    if (strlen(line) == MAXLINELENGTH-1) {
	printf("error: line too long\n");
	exit(1);
    }

    // Ignore blank lines at the end of the file.
    if(lineIsBlank(line)) {
        return 0;
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) {
	/* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
        opcode, arg0, arg1, arg2);

    return(1);
}

static inline int
isNumber(char *string)
{
    int num;
    char c;
    return((sscanf(string, "%d%c",&num, &c)) == 1);
}


// Prints a machine code word in the proper hex format to the file
static inline void 
printHexToFile(FILE *outFilePtr, int word) {
    fprintf(outFilePtr, "0x%08X\n", word);
}

// Returns 1 if string ends with substr, 0 otherwise
static int
endsWith(char *string, char *substr) {
    size_t stringLen = strlen(string);
    size_t substrLen = strlen(substr);
    if (stringLen < substrLen) {
        return 0; // string too short
    }
    char *stringEnd = string + stringLen - substrLen;
    if (strcmp(stringEnd, substr) == 0) {
        return 1;
    }
    return 0;
}
