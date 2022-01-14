//push command for constant memory seg
@i
D = A
@SP
A=M //go to stack top
M = D
@SP
M = M + 1