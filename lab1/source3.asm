.ORIG x3000
ADD R2, R3, R4 ; R2 ê R3 + R4
ADD R2, R3, #7 ; R2 ê R3 + 7
AND R2, R3, R4 ; R2 ê R3 AND R4
AND R2, R3, #7 ; R2 ê R3 AND 7
BRn LOOP;
BRp LOOP;
BRz LOOP;
BRnz LOOP;
BRzp LOOP ; Branch to LOOP if the last result was zero or positive.
BRnp LOOP;
BRnzp LOOP;
BR LOOP ; Unconditionally Branch to NEXT.
JMP R2
RET
JSR LOOP
JSRR R3
LDB R4, R2, #10
LDW R4, R2, #10
LEA R4, LOOP
NOT R4, R2
RTI
LSHF R2, R3, #3
RSHFL R2, R3, x7
RSHFA R2, R3, x7
STB R4, R2, #10
STW R4, R2, #10
TRAP x23
XOR R3, R1, R2 ; R3 ê R1 XOR R2
XOR R3, R1, #12 ; R3 ê R1 with bits [3], [2] complemented.
LOOP NOT R3, R2 ; R3 ê NOT(R2)
.END
