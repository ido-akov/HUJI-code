//pop top two entries in stack, push -1 if equal else 0
@SP
AM = M - 1
D = M //top most entry
A = A - 1
MD = M - D //now this entry is difference
@True
D;JEQ
@SP
A = M - 1
M = 0
@End
0;JMP

(True)
@SP
A = M - 1
M = -1
(End)