
;  Name 1: Harshit Roy
;  Name 2: Margaret Lee
;  UTEID 1: hr9873
;  UTEID 2: ml55822  
;

            .ORIG x3000
            LEA R6 ADDR_3100 ;R6 <- address of ADDR_3100 (say, x3200)
            LEA R5 OVERFLOW  ;R5 <- address of OVERFLOW (x3202)

            LDW R6 R6 #0 ;R6 <- MEM[R6] = MEM[x3200] = x3100
	        LDB R0 R6 #0 ;R0 <- MEM[R6] = MEM[x3100] = data1
            LDB R1 R6 #1 ;R1 <- MEM[R6+1] = MEM[x3101] = data2
            
	        LDW R4 R5 #0 ;R4 <- MEM[R5] = MEM[x3201] = xFF81

            AND R2 R2 #0 ;R2 <- 0, stores result 
            AND R3 R3 #0 ;R3 <- 0, stores overflow

MULTI	    ADD R1 R1 #-1
	        BRN EXIT		
	        ADD R2 R2 R0           
            BR MULTI
       
            ;Check Overflow
EXIT        ADD R4 R4 R2
            BRNZ NO_OVERFLOW
            ADD R3 R3 #1

NO_OVERFLOW STB R2 R6 #2 ;R2 -> MEM[R6+2] = MEM[x3202]
            STB R3 R6 #3 ;R3 -> MEM[R6+3] = MEM[x3203]
            
ADDR_3100 .FILL x3100
OVERFLOW .FILL xFF81 ; -127
            .END
