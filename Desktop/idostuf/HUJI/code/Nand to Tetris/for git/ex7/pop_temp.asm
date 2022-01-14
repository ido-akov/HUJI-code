//good for temp memory segment
@i
D = A
@R5
D = D + A
@R13
M = D //save address in free register for later
@SP
M = M - 1
A=M //go to stack top
D = M
@R13
A = M //temp variable to which we want to write
M = D