//good for static memory seg
@segment
D = M //get data from address
@SP
A=M //go to stack top
M = D
@SP
M = M + 1