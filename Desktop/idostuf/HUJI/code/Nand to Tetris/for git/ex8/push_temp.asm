//good for temp memory seg
@index
D = A
@R5
A = D + A //pointer to desired address
D = M //get data from address
@SP
A=M //go to stack top
M = D
@SP
M = M + 1