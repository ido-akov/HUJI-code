//pop two top entries in stack, check if second less than first, push 0 if true else push -1
@SP
AM = M - 1
D = M //top most entry
@nonNeg
D;JGE

@SP
A = M - 1
D = M //second top most entry
@normal
D;JLT //jump to normal algorithm if x is negative as well
//otherwise x is larger
@False
0;JMP

(nonNeg)
@SP
A = M - 1
D = M //second top most entry
@normal  
D;JGE //jump to normal algorithm if x is non-negative as well
//otherwise x is smaller
@True
0;JMP

(normal)
//D is second top most entry
@SP
A = M
D = D - M
@True
D;JLT

(False)
D = 0
@Set
0;JMP

(True)
D = -1
(Set)
@SP
A = M - 1
M = D