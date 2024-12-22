.ORIG x1200
	ADD R6, R6, #-2 ; Storing the values of register R0 and R1
	STW R0, R6, #0  ; on the stack
	ADD R6, R6, #-2
	STW R1, R6, #0 
	LEA R0, ADDR
	LDW R0, R0, #0; R0 = x4000
	LDW R1, R0, #0; R1 = val at x4000
	ADD R1, R1, #1; R1 increment by 1
	STW R1, R0, #0; MEM[x4000] <-- R1
	LDW R1, R6, #0; Restoring the values of register R1 and R0
	ADD R6, R6, #2; from the stack
	LDW R0, R6, #0
	ADD R6, R6, #2
	RTI
ADDR 	.FILL x4000
	.end	
