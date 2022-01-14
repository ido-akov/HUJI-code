//good for local/arg/this/that memory segs
@index
D = A
@segment
D = D + M
@R13
M = D //save address in free register for later 
@SP
AM = M - 1 //go to stack top
D = M
@R13
A = M
M = D