//good for local/arg/this/that memory segments
@i
D = A
@segment
A = D + M //pointer to desired address
D = M //get data from address
@SP
A=M //go to stack top
M = D
@SP
M = M + 1