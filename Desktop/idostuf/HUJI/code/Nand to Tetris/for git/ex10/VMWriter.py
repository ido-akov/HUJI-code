LINE_END = '\n'
PUSH = 'push'
POP = 'pop'
SPACE = ' '
LABEL = 'label'
GOTO = "goto"
IF = "if-"
CALL = "call"
FUNCTION = "function"
RETURN = 'return'
WRITE = 'w'


class VMWriter:
    def __init__(self, output_file):
        self.file = open(output_file, WRITE)

    def writePush(self, segment, index):
        self.file.write("{0} {1} {2}".format(PUSH, segment, index) + LINE_END)

    def writePop(self, segment, index):
        self.file.write("{0} {1} {2}".format(POP, segment, index) + LINE_END)

    def writeArithmetic(self, command):
        self.file.write(command + LINE_END)

    def writeLabel(self, labelName):
        self.file.write("{0} {1}".format(LABEL, labelName) + LINE_END)

    def writeGoto(self, labelName):
        self.file.write("{0} {1}".format(GOTO, labelName) + LINE_END)

    def writeIf(self, labelName):
        self.file.write("{0}{1} {2}".format(IF, GOTO, labelName) + LINE_END)

    def writeCall(self, name, nArgs):
        self.file.write("{0} {1} {2}".format(CALL, name, nArgs) + LINE_END)

    def writeFunction(self, name, nLocals):
        self.file.write("{0} {1} {2}".format(FUNCTION, name, nLocals) + LINE_END)

    def writeReturn(self):
        self.file.write(RETURN + LINE_END)

    def close(self):
        self.file.close()