import re
import sys
import os

# regexes for matching commands in the template ASM files
MEM_REGEX = "^(push|pop)(.+?)(\d+)$"
JUMP_REGEX = "True|False|End|nonNeg|normal|Set"
WHITESPACE_REGEX = "\\s|\\/\\/.*$"
ARITH_REGEX = "^add|sub|neg|and|or|not|eq|gt|lt$"

# constants
EMPTY_STRING = ""
UNDERSCORE = "_"
ADDRESS_DICT = {"local": "LCL", "argument": "ARG", "this": "THIS", "that": "THAT"}
INDEX_REPLACE = "@i"
SEGMENT_REPLACE = "@segment"
READ = "r"
APPEND = "a"
ASM = ".asm"
VM = ".vm"
LINE_END = "\n"
ADDRESS = '@'
SLASH = '/'
# global variable for keeping track of jumps in arithmetic commands
command_num = 0
output_file_name = EMPTY_STRING


def main():
    """
    get path to file/dir as argument, VMTranslate file if ends with .asm, or all such files in dir
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
    # get program name for static commands!
    prog_name = os.path.split(file_path)[1][:-2]
    with open(file_path, READ) as file, open(output_file_name, APPEND) as end_file:
        for next_line in file:
            # replace white spaces and comments
            subbed_line = re.sub(WHITESPACE_REGEX, EMPTY_STRING, next_line)
            if not subbed_line:
                # line is whitespace or comment
                continue
            mem_command = re.search(MEM_REGEX, subbed_line)
            if mem_command:
                # memory access command- push/pull segment i
                stack_command = mem_command.group(1)  # push or pull
                segment = mem_command.group(2)  # local/
                index = mem_command.group(3)  # some integer
                # asm_code is a list into which we write translated ASM instructions releva
                asm_code = write_memory_command(index, prog_name, stack_command, segment)
            else:
                # arithmetic command- add/sub/...
                asm_code = write_arithmetic_command(subbed_line)
            end_file.writelines(asm_code)


def write_arithmetic_command(vm_line):
    '''
    receive arithmetic vm instruction: add/sub/neg/and/or/not/eq/gt/lt
    return list with relevant ASM code (NOTHING NEEDS TO BE REPLACED)

    :param subbed_line: vm instruction
    :return: list with ASM instructions
    '''
    global command_num
    asm_instructions = []
    with open(vm_line + ASM, READ) as file:
        for line in file:
            subbed_line = line
            jump = re.search(JUMP_REGEX, line)
            if jump:
                # line is a label or a jump command. We specify where exactly to jump in the final ASM file
                # by adding on the command_num to the label
                jump_command = jump.group()
                subbed_line = subbed_line.replace(jump_command, jump_command + str(command_num))
            asm_instructions.append(subbed_line)
    command_num = command_num + 1
    return asm_instructions


def write_memory_command(index, prog_name, stack_command, segment):
    '''
    receive memory access vm instruction: push/pull segment i
    :param index: index of memory segment we access
    :param prog_name: vm program name, we need this argument in case segment == "stack", because we then translate
    @segment to @<prog_name>.i
    :param stack_command: push or pop
    :param segment: memory segment on which we do command
    :return: list with updated ASM instructions
    '''
    asm_instructions = []
    # replace @i with index_string
    index_string = ADDRESS + index + LINE_END
    if segment in ADDRESS_DICT:
        my_file = open(stack_command + ASM, READ)
        # replace @segment with address_string
        address_string = ADDRESS + ADDRESS_DICT.get(segment) + LINE_END
    else:
        my_file = open(stack_command + UNDERSCORE + segment + ASM, READ)
        # this will be used as replacement only if segment == "static"
        address_string = ADDRESS + prog_name + index + LINE_END
    for line in my_file:
        # replace all appearances of @i and @segment with necessary strings
        instruction = line.replace(INDEX_REPLACE, index_string).replace(SEGMENT_REPLACE, address_string)
        asm_instructions.append(instruction)
    my_file.close()
    return asm_instructions


if __name__ == "__main__":
    main()
