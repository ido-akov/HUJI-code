//good for local/arg/this/that memory segments
@i
D = A
@segment
D = D + M
@R13
M = D //save address in free register for later 
@SP
M = M - 1
A=M //go to stack top
D = M
@R13
A = M
M = D