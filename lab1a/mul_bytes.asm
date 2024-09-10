            .ORIG x3000
            LEA R6 ADDR_3100
            LEA R5 OVERFLOW

            LDB R0 R6 #0 ;R0 stores the data1 at x3100
            LDB R1 R6 #1 ;R1 stores the data2 at x3101
            LDW R4 R5 #0 ;R4 stores -255

            AND R2 R2 #0 ;R2 stores the result
            AND R3 R3 #0 ;R3 stores the overflow

MULTI       ADD R2 R2 R0
            ADD R1 R1 #-1
            BRP MULTI

            
            ;Check Overflow
            ADD R4 R4 R2
            BRNZ NO_OVERFLOW
            ADD R3 R3 #1

NO_OVERFLOW STB R2 R6 #2 ; Store the result at x3102
            STB R3 R6 #3 ; Store the overflow at x3103
            
ADDR_3100 .FILL x3100
OVERFLOW .FILL xFF01 ; -255
            .END
