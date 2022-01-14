//call function
@ret
D = A
@SP
M = M + 1
A = M - 1
M = D //push address to stack
@R4
D = A
@R13 //save 4
M = D
@R14 //temp var for loop
M = 0

(Loop)
@R14
AM = M + 1 //addresses, R14++
D = M //D = *RAM[address]
@SP
M = M + 1
A = M - 1
M = D
@R14
D = M //current i
@R4
D = A - D //4 - i
@Continue
D;JEQ
@Loop
0;JMP

(Continue)
@R5
D = A
@argNum
D = D + A
@SP
D = M - D //SP - n - 5
@ARG
M = D
@SP
D = M
@LCL
M = D
@funcName
0;JMP
(ret)