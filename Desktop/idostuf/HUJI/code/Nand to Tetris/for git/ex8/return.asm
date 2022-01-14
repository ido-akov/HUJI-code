//return to previous function
@LCL
D = M
@R13 //var FRAME
M = D //put LCL into var FRAME
@R5
A = D - A //FRAME - 5
D = M //de-ref for return address
@R14 //var RET
M = D
@SP //pop
AM = M - 1
D = M
@ARG
A = M
M = D //into ARG
D = A + 1
@SP
M = D //SP = ARG + 1
@R4
D = A
@R15 //loop var
M = D //

(Loop)
@R13
AM = M - 1
D = M //de-ref FRAME - i
@R15
A = M //go to relevant pointer
M = D
@R15
MD = M - 1
@Continue
D;JEQ
@Loop
0;JMP

(Continue)
@R14
A = M
0;JMP