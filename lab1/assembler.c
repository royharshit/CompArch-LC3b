/*
    Name 1: Harshit Roy
    Name 2: 
    UTEID 1: 
    UTEID 2: 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <math.h>

#define MAX_LABEL_LEN 20
#define MAX_SYMBOLS 255
#define MAX_LINE_LENGTH 255

typedef enum {
        DONE,
        OK,
        EMPTY_LINE
} Status;

typedef struct {
        int address;
        char label[MAX_LABEL_LEN + 1];
} TableEntry;

TableEntry symbolTable[MAX_SYMBOLS];

int getAddress( char *label) {

    for(int i=0; i<MAX_SYMBOLS; i++) {
        if (strcmp(symbolTable[i].label, label) == 0) {
            return symbolTable[i].address;
        }
    }

    return -1;

}

int binaryToInt(const char *binaryString) {
    int result = 0;
    int length = strlen(binaryString);

    for (int i = 0; i < length; i++) {
        if (binaryString[length - i - 1] == '1') {
            result += pow(2, i);
        }
    }

    return result;
}

int toNum( char * pStr )
{
        char * t_ptr;
        char * orig_pStr;
        int t_length,k;
        int lNum, lNeg = 0;
        long int lNumLong;

        orig_pStr = pStr;
        if( *pStr == '#' )                              /* decimal */
        {
                pStr++;
                if( *pStr == '-' )                              /* dec is negative */
                {
                        lNeg = 1;
                        pStr++;
                }
                t_ptr = pStr;
                t_length = strlen(t_ptr);

                for(k=0;k < t_length;k++)
                {
                        if (!isdigit(*t_ptr))
                        {
                                printf("Error: invalid decimal operand, %s\n",orig_pStr);
                                exit(4);
                        }
                        t_ptr++;
                }
                lNum = atoi(pStr);
                if (lNeg)
                        lNum = -lNum;

                return lNum;
        }
		else if( *pStr == 'x' ) /* hex     */
        {
                pStr++;
                if( *pStr == '-' )                              /* hex is negative */
                {
                        lNeg = 1;
                        pStr++;
                }
                t_ptr = pStr;
                t_length = strlen(t_ptr);
                for(k=0;k < t_length;k++)
                {
                        if (!isxdigit(*t_ptr))
                        {
                                printf("Error: invalid hex operand, %s\n",orig_pStr);
                                exit(4);
                        }
                        t_ptr++;
                }
                lNumLong = strtol(pStr, NULL, 16);    /* convert hex string into integer */
                lNum = (lNumLong > INT_MAX)? INT_MAX : lNumLong;
                if( lNeg )
                        lNum = -lNum;
                return lNum;
        }
        else
        {
                printf( "Error: invalid operand, %s\n", orig_pStr);
                exit(4);  /* This has been changed from error code 3 to error code 4, see clarification 12 */
        }
}

int isOpcode (char *opcode) {

        if (strcmp(opcode,"add") == 0 |
                strcmp(opcode,"and") == 0|
                strcmp(opcode,"br") == 0 |
                strcmp(opcode,"brn") == 0 |
                strcmp(opcode,"brz") == 0 |
                strcmp(opcode,"brp") == 0 |
                strcmp(opcode,"brnz") == 0 |
                strcmp(opcode,"brzp") == 0 |
                strcmp(opcode,"brnp") == 0 |
                strcmp(opcode,"brnzp") == 0 |
                strcmp(opcode,"jmp") == 0 |
                strcmp(opcode,"jsr") == 0 |
                strcmp(opcode,"jsrr") == 0 |
                strcmp(opcode,"ldb") == 0 |
                strcmp(opcode,"ldw") == 0 |
                strcmp(opcode,"lea") == 0 |
                strcmp(opcode,"not") == 0 |
                strcmp(opcode,"ret") == 0 |
                strcmp(opcode,"rti") == 0 |
                strcmp(opcode,"lshf") == 0 |
                strcmp(opcode,"rshfl") == 0 |
                strcmp(opcode,"rshfa") == 0 |
                strcmp(opcode,"stb") == 0 |
                strcmp(opcode,"stw") == 0 |
                strcmp(opcode,"trap") == 0 |
                strcmp(opcode,"xor") == 0 |
                strcmp(opcode,"halt") == 0 |
                strcmp(opcode,"nop") == 0)
                return 1;
        else
                return 0;

}

int isPseudoOpcode(char *opcode) {

        if (strcmp(opcode, ".fill") == 0)
            return 1;
        else
            return 0;

}

int convertRegToInt(char *reg) {

    reg++;
    return (int)(*reg-'0');

}

uint16_t encode_instruction_shf(char **dr, char **sr, char **amount, int b5, int b4) {

    char *opcode="1101";
    uint16_t instruction = 0;
    instruction += (binaryToInt(opcode) <<12);

    int dr_int;
    dr_int = convertRegToInt(*dr);
    instruction += (dr_int <<9);

    int sr_int;
    sr_int = convertRegToInt(*sr);
    instruction += (sr_int <<6);

    instruction += b5*(1<<5) + b4*(1<<4) + toNum(*amount);

    return instruction;

}

uint16_t encode_instruction_not(char **dr, char **sr) {

    char *opcode="1001";
    uint16_t instruction = 0;
    instruction += (binaryToInt(opcode) <<12);

    int dr_int;
    dr_int = convertRegToInt(*dr);
    instruction += (dr_int <<9);

    int sr_int;
    sr_int = convertRegToInt(*sr);
    instruction += (sr_int <<6);

    instruction += binaryToInt("111111");

    return instruction;

}

uint16_t encode_instruction_jsr(char **offset, int pc) {

    char *opcode="0100";
    uint16_t instruction = 0;
    instruction += (binaryToInt(opcode) <<12);
    instruction += (1 <<11);

    int address = getAddress(*offset);
    int offset_int;

    if ( address == -1) {
        offset_int = toNum(*offset);
    } else {
        offset_int = (address - pc)/2;
    }

    if (offset_int > 0)
        instruction += offset_int;
    else
        instruction += offset_int + 2*(1<<10);

    return instruction;
}

uint16_t encode_instruction_jsrr(char **sr1) {

    char *opcode="0100";
    uint16_t instruction = 0;
    instruction += (binaryToInt(opcode) <<12);

    int sr_int;
    sr_int = convertRegToInt(*sr1);
    instruction += (sr_int <<6);

    return instruction;
}

uint16_t encode_instruction_jmp(char **sr1) {

    char *opcode="1100";
    uint16_t instruction = 0;
    instruction += (binaryToInt(opcode) <<12);

    int sr_int;
    sr_int = convertRegToInt(*sr1);
    instruction += (sr_int <<6);

    return instruction;

}

uint16_t encode_instruction_br(int n, int z, int p, char **offset, int pc) {

    uint16_t instruction = 0;

    if (n == 1)
        instruction += (1<<11);
    if (z == 1)
        instruction += (1<<10);
    if (p == 1)
        instruction += (1<<9);

    int address = getAddress(*offset);
    int offset_int;

    if ( address == -1) {
        offset_int = toNum(*offset);
    } else {
        offset_int = (address - pc)/2;
    }

    if (offset_int > 0)
        instruction += offset_int;
    else
        instruction += offset_int + 2*(1<<8);

    return instruction;

}

uint16_t encode_instruction_lea(char** dr, char** offset, int pc) {

    char *opcode="1110";
    uint16_t instruction = 0;
    instruction += (binaryToInt(opcode) <<12);

    int dr_int;
    dr_int = convertRegToInt(*dr);
    instruction += (dr_int <<9);

    int address = getAddress(*offset);
    int offset_int;
    if (address != -1)
        offset_int = (getAddress(*offset) - pc)/2;
    else
        offset_int = (toNum(*offset) -pc)/2;

    instruction += offset_int;

    return instruction;
}

uint16_t encode_instruction_ldw(char *opcode, char** dr, char** sr, char** offset) {

    uint16_t instruction = 0;
    instruction += (binaryToInt(opcode) <<12);

    int dr_int;
    dr_int = convertRegToInt(*dr);
    instruction += (dr_int <<9);

    int sr_int;
    sr_int = convertRegToInt(*sr);
    instruction += (sr_int <<6);

    int offset_int= toNum(*offset);

    if (offset_int >= 0)
        instruction += offset_int;
    else
        instruction += offset_int + 2*(1 <<5);

    return instruction;

}

uint16_t encode_instruction_trap(char** trapvect) {

    uint16_t instruction = (binaryToInt("1111") <<  12);

    int trapvect_int = toNum(*trapvect);

    if (trapvect_int > 0)
        instruction += trapvect_int;

    return instruction;

}

uint16_t encode_instruction_add(char *opcode, char** dr, char** sr1, char** sr2) {

    uint16_t instruction = 0;
    instruction += (binaryToInt(opcode) <<12);

    int dr_int, sr1_int, sr2_int;
    dr_int = convertRegToInt(*dr);
    instruction += (dr_int <<9);

    sr1_int = convertRegToInt(*sr1);
    instruction += (sr1_int <<6);

    if ( *sr2[0] == 'r') {
        sr2_int = convertRegToInt(*sr2);
        instruction += sr2_int;
    } else {
        instruction += (1 <<5);
        sr2_int = toNum(*sr2);
        if (sr2_int >= 0)
            instruction += sr2_int;
        else
            instruction += sr2_int + 2*(1 <<4);
    }

    return instruction;
}

void toUpperCase(char str[]) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}

Status readAndParse( FILE * pInfile, char * pLine, char ** pLabel, char ** pOpcode, char ** pArg1, char ** pArg2, char ** pArg3, char ** pArg4)
{

    char * lRet, * lPtr;
    int i;

    // Read each line and print it to the console
    while (!fgets(pLine, MAX_LINE_LENGTH, pInfile)) {
        return( DONE );
    }

    for( i = 0; i < strlen( pLine ); i++ )
        pLine[i] = tolower( pLine[i] );

    /* convert entire line to lowercase */
    *pLabel = *pOpcode = *pArg1 = *pArg2 = *pArg3 = *pArg4 = pLine + strlen(pLine);

    /* ignore the comments */
    lPtr = pLine;

    /* Removing the comment part */
    while( *lPtr != ';' && *lPtr != '\0' && *lPtr != '\n' )
        lPtr++;

    *lPtr = '\0';
    if( !(lPtr = strtok( pLine, "\t\n ," ) ) )
        return( EMPTY_LINE );

    if( isOpcode( lPtr ) == 0 && lPtr[0] != '.' ) /* found a label */
    {
       *pLabel = lPtr;
       if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );
    }

    if (strcmp(lPtr,".end") == 0 ) return(DONE);

    *pOpcode = lPtr;

    if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

    *pArg1 = lPtr;

    if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

    *pArg2 = lPtr;

    if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

    *pArg3 = lPtr;

    if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

    *pArg4 = lPtr;


    return( OK );

}

void printHex(FILE *file_out, int num) {
    if (num < 0) {
        fprintf(file_out, "0x%04X\n", num & 0xFFFF);
    } else {
        fprintf(file_out, "0x%04X\n", num);
    }
}

int main(int argc, char* argv[]) {

    char *prgName   = NULL;
    char *iFileName = NULL;
    char *oFileName = NULL;

    prgName   = argv[0];
    iFileName = argv[1];
    oFileName = argv[2];

    char Line[MAX_LINE_LENGTH+1], *Label, *Opcode, *Arg1, *Arg2, *Arg3, *Arg4;
    FILE *file;
    uint16_t pc = 0;
    uint16_t symbol_count = 0;


    // Open the file in read mode
    file = fopen(iFileName, "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    Status Ret;

    // Pass 1 
    do {

        Ret = readAndParse( file, Line, &Label, &Opcode, &Arg1, &Arg2, &Arg3, &Arg4 );

        if( Ret != DONE && Ret != EMPTY_LINE )
        {
                if (strcmp(Opcode, ".orig") == 0)
                        pc = toNum(Arg1);

                if (Label[0] != '\0' && Ret != DONE) {
                        symbolTable[symbol_count].address = pc - 2;
                        strcpy(symbolTable[symbol_count].label, Label);
                        symbol_count++;
                }

                if (strcmp(Label, ".end") == 0)
                        break;

                pc = pc + 2;
        }


    } while( Ret != DONE );
    // Close the file
    fclose(file);

    pc = 0;

    // Open the file in read mode
    file = fopen(iFileName, "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    FILE* file_out = fopen(oFileName, "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    //Pass 2

    do {

        Ret = readAndParse( file, Line, &Label, &Opcode, &Arg1, &Arg2, &Arg3, &Arg4 );

        if( Ret != DONE && Ret != EMPTY_LINE )
        {
            if (strcmp(Opcode, ".orig") == 0) {
                pc = toNum(Arg1);
            }

            if(strcmp(Opcode, ".orig")==0 | strcmp(Opcode, ".fill") == 0) {
                printHex(file_out,  toNum(Arg1));
            } else if(strcmp(Opcode, "add")==0) {
                printHex(file_out,  encode_instruction_add("0001", &Arg1, &Arg2, &Arg3));
            } else if(strcmp(Opcode, "and")==0) {
                printHex(file_out,  encode_instruction_add("0101", &Arg1, &Arg2, &Arg3));
            } else if(strcmp(Opcode, "xor")==0) {
                printHex(file_out,  encode_instruction_add("1001", &Arg1, &Arg2, &Arg3));
            } else if(strcmp(Opcode, "lea")==0) {
                printHex(file_out,  encode_instruction_lea(&Arg1, &Arg2, pc));
            } else if(strcmp(Opcode, "ldw")==0) {
                printHex(file_out,  encode_instruction_ldw("0110", &Arg1, &Arg2, &Arg3));
            } else if(strcmp(Opcode, "ldb")==0) {
                printHex(file_out,  encode_instruction_ldw("0010", &Arg1, &Arg2, &Arg3));
            } else if(strcmp(Opcode, "stw")==0) {
                printHex(file_out,  encode_instruction_ldw("0111", &Arg1, &Arg2, &Arg3));
            } else if(strcmp(Opcode, "stb")==0) {
                printHex(file_out,  encode_instruction_ldw("0011", &Arg1, &Arg2, &Arg3));
            } else if(strcmp(Opcode, "trap")==0) {
                printHex(file_out,  encode_instruction_trap(&Arg1));
            } else if(strcmp(Opcode, "halt")==0) {
                char *trapvec="x25";
                printHex(file_out,  encode_instruction_trap(&trapvec));
			} else if(strcmp(Opcode, "br")==0) {
                printHex(file_out,  encode_instruction_br(1, 1, 1, &Arg1, pc));
            } else if(strcmp(Opcode, "brz")==0) {
                printHex(file_out,  encode_instruction_br(0, 1, 0, &Arg1, pc));
            } else if(strcmp(Opcode, "brn")==0) {
                printHex(file_out,  encode_instruction_br(1, 0, 0, &Arg1, pc));
            } else if(strcmp(Opcode, "brp")==0) {
                printHex(file_out,  encode_instruction_br(0, 0, 1, &Arg1, pc));
            } else if(strcmp(Opcode, "brzp")==0) {
                printHex(file_out,  encode_instruction_br(0, 1, 1, &Arg1, pc));
            } else if(strcmp(Opcode, "brnp")==0) {
                printHex(file_out,  encode_instruction_br(1, 0, 1, &Arg1, pc));
            } else if(strcmp(Opcode, "brnz")==0) {
                printHex(file_out,  encode_instruction_br(1, 1, 0, &Arg1, pc));
            } else if(strcmp(Opcode, "brnzp")==0) {
                printHex(file_out,  encode_instruction_br(1, 1, 1, &Arg1, pc));
            } else if(strcmp(Opcode, "jmp")==0) {
                printHex(file_out,  encode_instruction_jmp(&Arg1));
            } else if(strcmp(Opcode, "ret")==0) {
                char *base="r7";
                printHex(file_out,  encode_instruction_jmp(&base));
            } else if(strcmp(Opcode, "jsr")==0) {
                printHex(file_out,  encode_instruction_jsr(&Arg1, pc));
            } else if(strcmp(Opcode, "jsrr")==0) {
                printHex(file_out,  encode_instruction_jsrr(&Arg1));
            } else if(strcmp(Opcode, "not")==0) {
                printHex(file_out,  encode_instruction_not(&Arg1, &Arg2));
            } else if(strcmp(Opcode, "rti")==0) {
                fprintf(file_out, "0x8000\n");
            } else if(strcmp(Opcode, "lshf")==0) {
                printHex(file_out,  encode_instruction_shf(&Arg1, &Arg2, &Arg3,  0, 0));
            } else if(strcmp(Opcode, "rshfl")==0) {
                printHex(file_out,  encode_instruction_shf(&Arg1, &Arg2, &Arg3, 0, 1));
            } else if(strcmp(Opcode, "rshfa")==0) {
                printHex(file_out,  encode_instruction_shf(&Arg1, &Arg2, &Arg3, 1, 1));
            } else if(strcmp(Opcode, "nop")==0) {
                fprintf(file_out, "0x0000\n");
            }
            pc = pc + 2;
        }

    } while( Ret != DONE );

    fclose(file);
    fclose(file_out);

    return EXIT_SUCCESS;
}

