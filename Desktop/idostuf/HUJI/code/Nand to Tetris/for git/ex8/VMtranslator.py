import re
import sys
import os

# regular expression matching for VM commands
MEM_REGEX = "^\\s*(push|pop)\\s+(.+?)\\s+(\\d+).*$"
JUMP_REGEX = "True|False|End|nonNeg|normal|Set|Loop|Continue"  # use less key words?
ARITH_REGEX = "^\\s*(add|sub|neg|and|or|not|eq|gt|lt).*$"
COMMAND_TYPE_REGEX = "^\\s*([a-z-]+)\\b"
FLOW_REGEX = "^\\s*(goto|if-goto|label)\\s+(.+?)\\s.*$"
FUNCTION_REGEX = "^\\s*(call|function)\\s+(.+?)\\s+(\\d+).*$"

# constants
RETURN = "return"
RET = RETURN[:-3]
FUNCTION = "function"
LABEL = "label"
FUNC_NAME = "funcName"
INDEX = "index"
SEGMENT = "segment"
ARG_NUM = "argNum"
CALL_SYS_INIT = "call Sys.init 0"
BOOTSTRAP_ASM = "bootstrap.asm"
MEM_COMMANDS = {"push", "pop"}
ARITH_COMMANDS = {"add", "sub", "neg", "and", "or", "not", "gt", "lt", "eq"}
FLOW_COMMANDS = {LABEL, "if-goto", "goto"}
ADDRESS_DICT = {"local": "LCL", "argument": "ARG", "this": "THIS", "that": "THAT"}
EMPTY_STRING = ""
UNDER_LINE = "_"
READ = "r"
APPEND = "a"
ASM = ".asm"
VM = ".vm"
DOT = '.'
UNIQUE = '$'

# global variables
# end file name
output_file_name = EMPTY_STRING
# name of current program translated
program_name = EMPTY_STRING
# keep track of jumps within translations of vm commands
jmp_num = 0
# We need these for creating a return address label in the translation of function commands
cur_func, cur_call = EMPTY_STRING, 0


def main():
    """
    get path to file/dir as argument, VMTranslate file if ends with .vm, or all such files in dir
    :return: none
    """
    global output_file_name
    path = sys.argv[1]
    # name of output file
    # if path points to file with .vm suffix VMTranslate it
    if os.path.isfile(path) and os.path.splitext(path)[-1] == VM:
        output_file_name = os.path.splitext(path)[0] + ASM
        VMTranslate(path)
    else:
        if os.path.isdir(path):
            dir_name = os.path.basename(os.path.normpath(path))
            output_file_name = os.path.join(path, dir_name + ASM)
            # VMTranslate all files in directory ending with .vm
            [VMTranslate(os.path.join(path, file)) for file in os.listdir(path) if file.endswith(VM)]


def VMTranslate(file_path):
    """
    VMTranslate file containing symbolic Hack code to a new file containing Hack machine code
    :param file_path: path to file
    :return: None
    """
    global program_name
    # set current program name- to be used for static memory segment
    program_name = os.path.split(file_path)[1][:-2]
    if not os.path.exists(output_file_name):
        # write in bootstrap code first
        with open(output_file_name, APPEND) as end_file, open(BOOTSTRAP_ASM, READ) as bootstrap:
            end_file.writelines(bootstrap.readlines())
            end_file.writelines(write_function_command(CALL_SYS_INIT))
    # now translate file
    with open(file_path, READ) as file, open(output_file_name, APPEND) as end_file:
        for next_line in file:
            vm_command = re.search(COMMAND_TYPE_REGEX, next_line)
            if vm_command:
                command_type = vm_command.group()
                asm_code = code_writer(command_type, next_line)
                end_file.writelines(asm_code)


def write_return_command(vm_line):
    '''
    translate vm return command to asm code
    :param vm_line: line to be translated
    :return: asm lines
    '''
    asm_instructions = _rewrite_asm_jumps(vm_line)
    return asm_instructions


def write_function_command(vm_line):
    '''
    translate vm function command: function/call funcName argNum, to asm code
    :param vm_line: line to be translated
    :return: asm lines
    '''
    global cur_func, cur_call
    func_command = re.search(FUNCTION_REGEX, vm_line)
    command_type, func_name, arg_num = func_command.group(1), func_command.group(2), \
                                       func_command.group(3)
    #update cur_func if new function declaration, otherwise update cur_call because call declaration
    cur_func, cur_call = (func_name, 0) if command_type == FUNCTION else (cur_func, cur_call + 1)
    return_address = cur_func + UNIQUE + RET + DOT + str(cur_call)
    # we use return_address only if command_type is "call"
    instructions = _rewrite_asm_jumps(command_type)
    # replace dummy funcName, argNum and ret with relevant values
    repl = lambda line: \
        line.replace(FUNC_NAME, func_name).replace(ARG_NUM, arg_num).replace(RET, return_address)
    return [repl(line) for line in instructions]


def code_writer(command_type, vm_line):
    '''
    function receives command type and vm line, writes relevant ASM code
    :param command_type: mem/arithmetic/flow/function
    :param line: vm_line
    :return: asm lines
    '''
    if command_type in MEM_COMMANDS:
        # memory access command- pop/push segment index
        return write_memory_command(vm_line)
    elif command_type in ARITH_COMMANDS:
        # arithmetic command- add/sub/...
        return write_arithmetic_command(command_type)
    elif command_type in FLOW_COMMANDS:
        # branching command- if-goto/goto/label label
        return write_flow_command(vm_line)
    elif command_type == RETURN:
        # return command
        return write_return_command(command_type)
    else:
        # function command- function/call funcName argNum
        return write_function_command(vm_line)


def write_arithmetic_command(vm_line):
    '''
    receive arithmetic vm instruction: add/sub/neg/and/or/not/eq/gt/lt
    return list with relevant ASM code (NOTHING NEEDS TO BE REPLACED)

    :param subbed_line: vm instruction
    :return: list with ASM instructions
    '''
    asm_instructions = _rewrite_asm_jumps(vm_line)
    return asm_instructions


def _rewrite_asm_jumps(vm_line):
    '''
    add unique number to labels in an asm file
    :param vm_line: line which we are translating
    :return: updated asm file with necessary label extensions
    '''
    global jmp_num
    asm_instructions = []
    with open(vm_line + ASM, READ) as file:
        for line in file:
            subbed_line = line
            jump = re.search(JUMP_REGEX, line)
            if jump:
                # line is a label or a jump command. We specify where exactly to jump in the final ASM file
                # by adding on jmp_num to the label
                jump_command = jump.group()
                subbed_line = subbed_line.replace(jump_command, jump_command + str(jmp_num))
            asm_instructions.append(subbed_line)
    jmp_num = jmp_num + 1
    return asm_instructions


def write_memory_command(vm_line):
    '''
    receive memory access vm instruction of form push/pull segment i, return translated ASM code
    :param vm_line: next line to parse
    :return: list with updated ASM instructions
    '''

    command = re.search(MEM_REGEX, vm_line)
    command_type, segment, index = command.group(1), command.group(2), command.group(3)
    if segment in ADDRESS_DICT:
        file_name = command_type + ASM
        # stack register pointing to necessary segment
        segment_address = ADDRESS_DICT.get(segment)
    else:
        file_name = command_type + UNDER_LINE + segment + ASM
        # this will be used as replacement only if segment == "static"
        segment_address = program_name + index
    with open(file_name, READ) as my_file:
        # replace index and segment with vm-parsed values
        return [line.replace(INDEX, index).replace(SEGMENT, segment_address) for line in my_file]


def write_flow_command(vm_line):
    '''
    translate a branching command to asm
    :param vm_line: line to be translated
    :return: asm code
    '''
    command = re.search(FLOW_REGEX, vm_line)
    command_type = command.group(1)
    # write in to label which function belongs to
    pre_label = cur_func if cur_func == EMPTY_STRING else (cur_func + UNIQUE)
    label = pre_label + command.group(2)
    with open(command_type + ASM, READ) as file:
        return [line.replace(LABEL, label) for line in file]


if __name__ == "__main__":
    main()
