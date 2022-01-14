//push implementation on pointer memory segment
@i
D = A 
@THIS
A = D + A //address is THIS or THAT
D = M
@SP
A=M //go to stack top
M = D //push into stack THIS or THAT
@SP
M = M + 1