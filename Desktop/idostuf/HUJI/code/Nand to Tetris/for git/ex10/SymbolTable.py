CLASS_KINDS = {'static', 'field'}
SUBROUTINE_KINDS = {'argument', 'var'}


class SymbolTable:
    def __init__(self):
        self.classTable = {}
        self.classCount = {kind: 0 for kind in CLASS_KINDS}
        self.subRoutineTable = {}
        self.subroutineCount = {kind: 0 for kind in SUBROUTINE_KINDS}

    def startSubroutine(self):
        self.subRoutineTable.clear()
        self.subroutineCount.fromkeys(self.subroutineCount, 0)

    def define(self, name, type, kind):
        if kind in SUBROUTINE_KINDS:
            self.subRoutineTable[name] = (type, kind, self.subroutineCount[kind])
            self.subroutineCount[kind] = self.subroutineCount[kind] + 1
        else:
            self.classTable[name] = (type, kind, self.classCount[kind])
            self.classCount[kind] = self.classCount[kind] + 1

    def varCount(self, kind):
        return self.subroutineCount[kind] if kind in SUBROUTINE_KINDS else self.classCount[kind]

    def kindOf(self, name):
        return self.subRoutineTable[name][1] if name in self.subRoutineTable else self.classTable[name][1]

    def typeOf(self, name):
        return self.subRoutineTable[name][0] if name in self.subRoutineTable else self.classTable[name][0]

    def indexOf(self, name):
        return self.subRoutineTable[name][2] if name in self.subRoutineTable else self.classTable[name][2]
