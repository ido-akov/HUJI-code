//pop implementation on pointer memory seg
@index
D = A //0 or 1
@THIS
D = D + A //D is now THIS or THAT
@R13
M = D //save address in free register for later 
@SP
M = M - 1
A=M //go to stack top
D = M
@R13
A = M
M = D //put stack top in THIS or THAT