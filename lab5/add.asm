.ORIG x3000
AND R1 R1 #0
ADD R1 R1 #10
LEA R0 DATA
LDW R0 R0 #0
AND R3 R3 #0
LOOP LDB R2 R0 #0 
ADD R3 R3 R2
LDB R2 R0 #1 
ADD R3 R3 R2 
ADD R0 R0 #2 
ADD R1 R1 #-1
BRP LOOP
STW R3 R0 #0
JMP R3
DATA .FILL xC000
.END
