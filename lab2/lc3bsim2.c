/*
    Remove all unnecessary lines (including this one) 
    in this comment.
    REFER TO THE SUBMISSION INSTRUCTION FOR DETAILS

    Name 1: Harshit Roy 
    Name 2:
    UTEID 1:
    UTEID 2: 
*/

/***************************************************************/
/*                                                             */
/*   LC-3b Instruction Level Simulator                         */
/*                                                             */
/*   EE 460N                                                   */
/*   The University of Texas at Austin                         */
/*                                                             */
/***************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/***************************************************************/
/*                                                             */
/* Files: isaprogram   LC-3b machine language program file     */
/*                                                             */
/***************************************************************/

/***************************************************************/
/* These are the functions you'll have to write.               */
/***************************************************************/

void process_instruction();

/***************************************************************/
/* A couple of useful definitions.                             */
/***************************************************************/
#define FALSE 0
#define TRUE  1

/***************************************************************/
/* Use this to avoid overflowing 16 bits on the bus.           */
/***************************************************************/
#define Low16bits(x) ((x) & 0xFFFF)

/***************************************************************/
/* Main memory.                                                */
/***************************************************************/
/* MEMORY[A][0] stores the least significant byte of word at word address A
   MEMORY[A][1] stores the most significant byte of word at word address A 
*/

#define WORDS_IN_MEM    0x08000 
int MEMORY[WORDS_IN_MEM][2];

/***************************************************************/

/***************************************************************/

/***************************************************************/
/* LC-3b State info.                                           */
/***************************************************************/
#define LC_3b_REGS 8
#define REG_NUM_RETURN_PC 7

int RUN_BIT;	/* run bit */


typedef struct System_Latches_Struct{

  int PC,		/* program counter */
    N,		/* n condition bit */
    Z,		/* z condition bit */
    P;		/* p condition bit */
  int REGS[LC_3b_REGS]; /* register file. */
} System_Latches;

/* Data Structure for Latch */

System_Latches CURRENT_LATCHES, NEXT_LATCHES;

/***************************************************************/
/* A cycle counter.                                            */
/***************************************************************/
int INSTRUCTION_COUNT;

/***************************************************************/
/*                                                             */
/* Procedure : help                                            */
/*                                                             */
/* Purpose   : Print out a list of commands                    */
/*                                                             */
/***************************************************************/
void help() {                                                    
  printf("----------------LC-3b ISIM Help-----------------------\n");
  printf("go               -  run program to completion         \n");
  printf("run n            -  execute program for n instructions\n");
  printf("mdump low high   -  dump memory from low to high      \n");
  printf("rdump            -  dump the register & bus values    \n");
  printf("?                -  display this help menu            \n");
  printf("quit             -  exit the program                  \n\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : cycle                                           */
/*                                                             */
/* Purpose   : Execute a cycle                                 */
/*                                                             */
/***************************************************************/
void cycle() {                                                

  process_instruction();
  CURRENT_LATCHES = NEXT_LATCHES;
  INSTRUCTION_COUNT++;
}

/***************************************************************/
/*                                                             */
/* Procedure : run n                                           */
/*                                                             */
/* Purpose   : Simulate the LC-3b for n cycles                 */
/*                                                             */
/***************************************************************/
void run(int num_cycles) {                                      
  int i;

  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating for %d cycles...\n\n", num_cycles);
  for (i = 0; i < num_cycles; i++) {
    if (CURRENT_LATCHES.PC == 0x0000) {
	    RUN_BIT = FALSE;
	    printf("Simulator halted\n\n");
	    break;
    }
    cycle();
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : go                                              */
/*                                                             */
/* Purpose   : Simulate the LC-3b until HALTed                 */
/*                                                             */
/***************************************************************/
void go() {                                                     
  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating...\n\n");
  while (CURRENT_LATCHES.PC != 0x0000)
    cycle();
  RUN_BIT = FALSE;
  printf("Simulator halted\n\n");
}

/***************************************************************/ 
/*                                                             */
/* Procedure : mdump                                           */
/*                                                             */
/* Purpose   : Dump a word-aligned region of memory to the     */
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void mdump(FILE * dumpsim_file, int start, int stop) {          
  int address; /* this is a byte address */

  printf("\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
  printf("-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    printf("  0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  printf("\n");

  /* dump the memory contents into the dumpsim file */
  fprintf(dumpsim_file, "\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
  fprintf(dumpsim_file, "-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    fprintf(dumpsim_file, " 0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  fprintf(dumpsim_file, "\n");
  fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : rdump                                           */
/*                                                             */
/* Purpose   : Dump current register and bus values to the     */   
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void rdump(FILE * dumpsim_file) {                               
  int k; 

  printf("\nCurrent register/bus values :\n");
  printf("-------------------------------------\n");
  printf("Instruction Count : %d\n", INSTRUCTION_COUNT);
  printf("PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
  printf("CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  printf("Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    printf("%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
  printf("\n");

  /* dump the state information into the dumpsim file */
  fprintf(dumpsim_file, "\nCurrent register/bus values :\n");
  fprintf(dumpsim_file, "-------------------------------------\n");
  fprintf(dumpsim_file, "Instruction Count : %d\n", INSTRUCTION_COUNT);
  fprintf(dumpsim_file, "PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
  fprintf(dumpsim_file, "CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  fprintf(dumpsim_file, "Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    fprintf(dumpsim_file, "%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
  fprintf(dumpsim_file, "\n");
  fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : get_command                                     */
/*                                                             */
/* Purpose   : Read a command from standard input.             */  
/*                                                             */
/***************************************************************/
void get_command(FILE * dumpsim_file) {                         
  char buffer[20];
  int start, stop, cycles;

  printf("LC-3b-SIM> ");

  scanf("%s", buffer);
  printf("\n");

  switch(buffer[0]) {
  case 'G':
  case 'g':
    go();
    break;

  case 'M':
  case 'm':
    scanf("%i %i", &start, &stop);
    mdump(dumpsim_file, start, stop);
    break;

  case '?':
    help();
    break;
  case 'Q':
  case 'q':
    printf("Bye.\n");
    exit(0);

  case 'R':
  case 'r':
    if (buffer[1] == 'd' || buffer[1] == 'D')
	    rdump(dumpsim_file);
    else {
	    scanf("%d", &cycles);
	    run(cycles);
    }
    break;

  default:
    printf("Invalid Command\n");
    break;
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : init_memory                                     */
/*                                                             */
/* Purpose   : Zero out the memory array                       */
/*                                                             */
/***************************************************************/
void init_memory() {                                           
  int i;

  for (i=0; i < WORDS_IN_MEM; i++) {
    MEMORY[i][0] = 0;
    MEMORY[i][1] = 0;
  }

}

/**************************************************************/
/*                                                            */
/* Procedure : load_program                                   */
/*                                                            */
/* Purpose   : Load program and service routines into mem.    */
/*                                                            */
/**************************************************************/
void load_program(char *program_filename) {                   
  FILE * prog;
  int ii, word, program_base;

  /* Open program file. */
  prog = fopen(program_filename, "r");
  if (prog == NULL) {
    printf("Error: Can't open program file %s\n", program_filename);
    exit(-1);
  }

  /* Read in the program. */
  if (fscanf(prog, "%x\n", &word) != EOF)
    program_base = word >> 1;
  else {
    printf("Error: Program file is empty\n");
    exit(-1);
  }

  ii = 0;
  while (fscanf(prog, "%x\n", &word) != EOF) {
    /* Make sure it fits. */
    if (program_base + ii >= WORDS_IN_MEM) {
	    printf("Error: Program file %s is too long to fit in memory. %x\n",
             program_filename, ii);
	    exit(-1);
    }

    /* Write the word to memory array. */
    MEMORY[program_base + ii][0] = word & 0x00FF;
    MEMORY[program_base + ii][1] = (word >> 8) & 0x00FF;
    ii++;
  }

  if (CURRENT_LATCHES.PC == 0) CURRENT_LATCHES.PC = (program_base << 1);

  printf("Read %d words from program into memory at %d.\n\n", ii, CURRENT_LATCHES.PC);
}

/************************************************************/
/*                                                          */
/* Procedure : initialize                                   */
/*                                                          */
/* Purpose   : Load machine language program                */ 
/*             and set up initial state of the machine.     */
/*                                                          */
/************************************************************/
void initialize(char *program_filename, int num_prog_files) { 
  int i;

  init_memory();
  for ( i = 0; i < num_prog_files; i++ ) {
    load_program(program_filename);
    while(*program_filename++ != '\0');
  }
  CURRENT_LATCHES.Z = 1;  
  NEXT_LATCHES = CURRENT_LATCHES;
    
  RUN_BIT = TRUE;
}

/***************************************************************/
/*                                                             */
/* Procedure : main                                            */
/*                                                             */
/***************************************************************/
int main(int argc, char *argv[]) {                              
  FILE * dumpsim_file;

  /* Error Checking */
  if (argc < 2) {
    printf("Error: usage: %s <program_file_1> <program_file_2> ...\n",
           argv[0]);
    exit(1);
  }

  printf("LC-3b Simulator\n\n");

  initialize(argv[1], argc - 1);

  if ( (dumpsim_file = fopen( "dumpsim", "w" )) == NULL ) {
    printf("Error: Can't open dumpsim file\n");
    exit(-1);
  }

  while (1)
    get_command(dumpsim_file);
    
}

/***************************************************************/
/* Do not modify the above code.
   You are allowed to use the following global variables in your
   code. These are defined above.

   MEMORY

   CURRENT_LATCHES
   NEXT_LATCHES

   You may define your own local/global variables and functions.
   You may use the functions to get at the control bits defined
   above.

   Begin your code here 	  			       */

/***************************************************************/

int16_t signExtend5To16(int8_t num) {

    // Check if the sign bit (bit 4 for 5-bit number) is set
    if (num & 0x10) { // 0x10 is 0001 0000 in binary
        return (int16_t)(num | 0xFFE0);  // Extend the sign bit
    }

    return (int16_t)num;  // No extension needed for non-negative numbers

}

int16_t extend9to16(int16_t x) {
    // Check if the 9th bit (sign bit) is set
    if (x & 0x0100) {
        // If set, extend the sign bit to the left
        x |= 0xFE00;
    }
    return x;
}

int16_t signExtendTo16(int16_t num, unsigned int size) {
    if (size > 16) {
        return num;
    }
    
    if ((num >> (size - 1)) & 0x1) {
        return (num | (0xFF << size)) & 0xFFFF;
    }

    return num;
}

void setCC(int16_t num) {
    
    NEXT_LATCHES.P = 0;
    NEXT_LATCHES.N = 0;
    NEXT_LATCHES.Z = 0;
    
    if (num > 0) {
      NEXT_LATCHES.P = 1;
    } else if (num < 0) {
      NEXT_LATCHES.N = 1;
    } else {
      NEXT_LATCHES.Z = 1;
    }
    return;
}

void process_instruction(){
  /*  function: process_instruction
   *  
   *    Process one instruction at a time  
   *       -Fetch one instruction
   *       -Decode 
   *       -Execute
   *       -Update NEXT_LATCHES
   */     

    // Fetch
 
    int16_t inst = MEMORY[CURRENT_LATCHES.PC >> 1][0] | MEMORY[CURRENT_LATCHES.PC >> 1][1] << 8;
    NEXT_LATCHES.PC += 2;

    int8_t opcode, dr, sr1, sr2, bit_5, imm5, bit_11;
    int8_t baseR, n, z, p, offset6, amount4;
    int16_t val, memaddr, pcoffset9;
    
    //Decode
    opcode = (inst >> 12)&0x0F;
    inst -= opcode << 12;
 
    switch (opcode) {
        case 0b0000:  //BR
            n = (inst >> 11)&0x01;  
            z = (inst >> 10)&0x01;
            p = (inst >> 9)&0x01; 
            inst -= ((n << 11) + (z << 10) + (p << 9));             

            if (CURRENT_LATCHES.N && n || CURRENT_LATCHES.P && p || CURRENT_LATCHES.Z && z ) {
                NEXT_LATCHES.PC += extend9to16(inst) << 1;  
            }
            break;
 
        case 0b0001:  //ADD
            dr = (inst >> 9)&0x07;
            inst -= dr << 9;
            sr1 = (inst >> 6)&0x07;
            inst -= sr1 << 6;
            bit_5 = (inst >> 5)&0x01;
            inst -= bit_5 << 5;
            if (bit_5 == 0) {
                sr2 = inst;
                val = CURRENT_LATCHES.REGS[sr1] + CURRENT_LATCHES.REGS[sr2]; 
            } else {
                imm5 = inst;
                val = CURRENT_LATCHES.REGS[sr1] + signExtend5To16(imm5); 
            }
            NEXT_LATCHES.REGS[dr] = Low16bits(val);
            setCC(val);
            break;

        case 0b0010:  //LDB
            dr =  (inst >> 9)&0x07;
            baseR = (inst >> 6)&0x07;
            offset6 = signExtendTo16(inst & 0x3F, 6);
            memaddr = CURRENT_LATCHES.REGS[baseR] + offset6;
            if (memaddr % 2 == 0 ) {
                val = signExtendTo16(MEMORY[memaddr >> 1][0], 8);
            } else {
                val = signExtendTo16(MEMORY[memaddr >> 1][1], 8);
            }
            NEXT_LATCHES.REGS[dr] = Low16bits(val);
            break;

        case 0b0011:  //STB
            sr1 =  (inst >> 9)&0x07;
            baseR = (inst >> 6)&0x07;
            offset6 = signExtendTo16(inst & 0x3F, 6);
            memaddr = CURRENT_LATCHES.REGS[baseR] + offset6;
            if (memaddr % 2 == 0 ) {
                MEMORY[memaddr >> 1][0] = CURRENT_LATCHES.REGS[sr1] & 0xFF;
            } else {
                MEMORY[memaddr >> 1][1] = CURRENT_LATCHES.REGS[sr1] & 0xFF;
            }
            break;

        case 0b0100:  //JSR(R)
            bit_11 = (inst >> 11)&0x01;
            inst -= bit_11 << 11;
            NEXT_LATCHES.REGS[7] = NEXT_LATCHES.PC; 
            if (bit_11 == 0) {
                sr1 = (inst >> 6)&0x07;
                NEXT_LATCHES.PC = CURRENT_LATCHES.REGS[sr1];
            } else {
                NEXT_LATCHES.PC += Low16bits(Low16bits(signExtendTo16((inst & 0x07FF), 11) << 1)); 
            } 
            break;     
            
        case 0b0101:  //AND
            dr = (inst >> 9)&0x07;
            inst -= dr << 9;
            sr1 = (inst >> 6)&0x07;
            inst -= sr1 << 6;
            bit_5 = (inst >> 5)&0x01;
            inst -= bit_5 << 5;
            if (bit_5 == 0) {
                sr2 = inst;
                val = CURRENT_LATCHES.REGS[sr1]&CURRENT_LATCHES.REGS[sr2]; 
            } else {
                imm5 = inst;
                val = CURRENT_LATCHES.REGS[sr1]& signExtend5To16(imm5);     
            }
            NEXT_LATCHES.REGS[dr] = Low16bits(val);
            setCC(val);
            break;

        case 0b0110:  //LDW
            dr =  (inst >> 9)&0x07;
            baseR = (inst >> 6)&0x07;
            offset6 = signExtendTo16(inst & 0x3f, 6);
            memaddr = CURRENT_LATCHES.REGS[baseR] + (offset6 << 1);
            val = MEMORY[memaddr >> 1][0] | MEMORY[memaddr >> 1][1] << 8;
            NEXT_LATCHES.REGS[dr] = Low16bits(val);
            setCC(val);
            break;

        case 0b0111:  //STW
            sr1 =  (inst >> 9)&0x07;
            baseR = (inst >> 6)&0x07;
            offset6 = signExtendTo16(inst & 0x3f, 6);
            memaddr = CURRENT_LATCHES.REGS[baseR] + (offset6 << 1);
            val = CURRENT_LATCHES.REGS[sr1];
            MEMORY[memaddr >> 1][0] = sr1 & 0xFF;
            MEMORY[memaddr >> 1][1] = (sr1 >> 8) & 0xFF;
            break;

        case 0b1000:  //RTI; need not to be implemented
            break;
        
        case 0b1001:  //XOR
            dr = (inst >> 9) & 0x07;
            sr1 = (inst >> 6) & 0x07;
            if ((inst & 0x20) >> 5 == 0) {
                sr2 = inst & 0x07;
                val = CURRENT_LATCHES.REGS[sr1] ^ CURRENT_LATCHES.REGS[sr2];
            } else if ((inst & 0x20) >> 5 == 1) {
                imm5 = signExtendTo16((inst & 0x1F), 5);
                val = CURRENT_LATCHES.REGS[sr1] ^ imm5;
            }
            NEXT_LATCHES.REGS[dr] = Low16bits(val);
            setCC(val);
            break;

        case 0b1010:  //not used
            break;

        case 0b1011:  //not used
            break;

        case 0b1100:  //JMP
            sr1 = (inst >> 6) & 0x07;
            NEXT_LATCHES.PC = CURRENT_LATCHES.REGS[sr1];  
            break;

        case 0b1101:  //SHF
            dr = (inst >> 9) & 0x07;
            sr1 = (inst >> 6) & 0x07;
            amount4 = inst & 0xF;
            if ((inst & 0x10) >> 4 == 0) {
                val = CURRENT_LATCHES.REGS[sr1] << amount4;
            } else {
                if ((inst & 0x20) >> 5 == 0) {
                    val = CURRENT_LATCHES.REGS[sr1] >> amount4;
                } else {
                    val = signExtendTo16(CURRENT_LATCHES.REGS[sr1] >> amount4, 16 - amount4);
                }
            }
            NEXT_LATCHES.REGS[dr] = Low16bits(val);
            setCC(val);
            break;

        case 0b1110:  //LEA
            dr =  (inst >> 9)&0x07;
            pcoffset9 = signExtendTo16(inst & 0x1FF, 9);
            NEXT_LATCHES.REGS[dr] = Low16bits(NEXT_LATCHES.PC + (pcoffset9 << 1));
            break;
        
        case 0b1111:  //TRAP
            NEXT_LATCHES.REGS[REG_NUM_RETURN_PC] = NEXT_LATCHES.PC;
            memaddr = (inst & 0xFF) << 1;
            NEXT_LATCHES.PC = MEMORY[memaddr >> 1][0] | MEMORY[memaddr >> 1][1] << 8;
            break;
        
        default: 
            break;
    }
         
}


