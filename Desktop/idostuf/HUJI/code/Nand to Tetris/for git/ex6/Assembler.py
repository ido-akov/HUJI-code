import re
import sys
import os

# tables that hold bit data corresponding to instructions
DEST_TABLE = {"None": "000", "M": "001", "D": "010", "MD": "011", "A": "100", "AM": "101",
              "AD": "110", "AMD": "111"}

COMP_TABLE = {"0": "101010", "1": "111111", "-1": "111010", "D": "001100", "A": "110000", "!D": "001101",
              "!A": "110001", "-D": "001111", "-A": "110011", "D+1": "011111", "A+1": "110111", "D-1": "001110",
              "A-1": "110010", "D+A": "000010", "D-A": "010011", "A-D": "000111", "D&A": "000000", "D|A": "010101",
              "M": "110000", "!M": "110001", "-M": "110011", "M+1": "110111", "M-1": "110010", "D+M": "000010",
              "D-M": "010011", "M-D": "000111", "D&M": "000000", "D|M": "010101"}

SHIFT_TABLE = {"D>>": "010000", "D<<": "110000", "A>>": "000000", "A<<": "100000", "M>>": "000000", "M<<": "100000"}

JMP_TABLE = {"None": "000", "JGT": "001", "JEQ": "010", "JGE": "011", "JLT": "100", "JNE": "101",
             "JLE": "110", "JMP": "111"}

# regular expressions for pattern matching and grouping program instructions
A_INSTR_REGEX = "^@(.+)$"
C_INSTR_REGEX = "^((.{1,3})=)?(.{1,3}?)(;(.{3}))?$"
LABEL_REGEX = "^\\((.+)\\)$"
WHITESPACE_REGEX = "\\s|\\/\\/.*$"

# magic numbers
BIT_LENGTH = 16
READ = "r"
WRITE = "w"
HACK = "hack"
MEMORY = "M"
C_INSTR_BIT = "1"
SHIFT_BITS = "01"
NORMAL_BITS = "11"
ASM = ".asm"
ONE = "1"
ZERO = "0"
LINE_END = "\n"
# global vars
symb_table = {"R0": "0", "R1": "1", "R2": "2", "R3": "3", "R4": "4", "R5": "5", "R6": "6", "R7": "7", "R8": "8",
              "R9": "9", "R10": "10", "R11": "11", "R12": "12", "R13": "13", "R14": "14", "R15": "15",
              "SCREEN": "16384", "KBD": "24576", "SP": "0", "LCL": "1", "ARG": "2", "THIS": "3", "THAT": "4"}
var_counter = 16


def a_instn_to_binary(a_reg):
    """
    convert a-instruction symbol or decimal to binary
    :param a_reg: string specifying symbol or decimal value
    :return: a-instruction in binary
    """
    global var_counter
    # check if a_reg is a symbol
    if not a_reg.isdigit():
        if not (a_reg in symb_table):
            # a_reg is a new symbol, add to dict, set to next variable register
            symb_table[a_reg] = var_counter
            var_counter = var_counter + 1
        a_reg = symb_table[a_reg]  # switch symbol with relevant register
    binary = "{0:b}".format(int(a_reg))
    remaining = BIT_LENGTH - len(binary)  # num of zeros to add for 16-bit string
    return ZERO * remaining + binary


def main():
    """
    get path to file/dir as argument, assemble file if ends with .asm, or all such files in dir
    :return: none
    """
    #
    path = sys.argv[1]
    # if path points to file with .asm suffix assemble it
    if os.path.isfile(path) and os.path.splitext(path)[-1] == ASM:
        assemble(path)
    else:
        if os.path.isdir(path):
            # assemble all files in directory ending with .asm
            [assemble(os.path.join(path, file)) for file in os.listdir(path) if file.endswith(ASM)]


def assemble(file_path):
    """
    assemble file containing symbolic Hack code to a new file containing Hack machine code
    :param file_path: path to file
    :return: None
    """
    # parse instructions to list
    parsed_instns = first_pass(file_path)
    # convert instructions to binary
    second_pass(parsed_instns, file_path)


def second_pass(instn_list, orig_file_path):
    """
    go over a/c-instructions and convert to binary code
    :param instn_list: list of instructions
    :param orig_file_path: path of file to translate
    :return: none
    """
    # create correct file path and name
    assembled_file_path = orig_file_path[:-3] + HACK
    with open(assembled_file_path, WRITE) as assembled_file:
        for next_instn in instn_list:
            a_inst = re.search(A_INSTR_REGEX, next_instn)
            if a_inst is not None:
                # line is a-instruction
                a_reg = a_inst.group(1)
                bin_code = a_instn_to_binary(a_reg)
            else:
                # line is a c-instruction
                c_instr = re.search(C_INSTR_REGEX, next_instn)
                bin_code = c_instn_to_binary(c_instr)
            assembled_file.write(bin_code + LINE_END)


def c_instn_to_binary(c_instr):
    """
    translate a C-instruction to binary code
    :param c_instr: string representing a c-instruction
    :return: string representing same instruction in binary
    """
    #  convert to string in case of None
    dest = str(c_instr.group(2))
    dest_binary = DEST_TABLE.get(dest)
    comp = str(c_instr.group(3))
    #  check if its a shift computation
    x_bits = SHIFT_BITS if comp in SHIFT_TABLE else NORMAL_BITS
    #  check if computation uses memory data or address data
    a_bit = ONE if MEMORY in comp else ZERO
    # Get correct binary instruction for computation
    comp_binary = COMP_TABLE.get(comp) if x_bits == NORMAL_BITS else SHIFT_TABLE.get(comp)
    jmp = str(c_instr.group(5))
    jmp_binary = JMP_TABLE.get(jmp)
    return C_INSTR_BIT + x_bits + a_bit + comp_binary + dest_binary + jmp_binary


def first_pass(file_path):
    """
    pass over file with Hack symbolic code, parse all labels and get rid of whitespace/comments
    :param file_path: path to file
    :return: list including only A/C instructions
    """
    line_num = 0
    instn_list = []
    with open(file_path, READ) as file:
        for next_line in file:
            # replace white spaces and comments
            subbed_line = re.sub(WHITESPACE_REGEX, "", next_line)
            # check if line is label declaration
            label = re.search(LABEL_REGEX, next_line)
            if label is not None:
                #  put in dict, don't add to instn-list
                label_name = label.group(1)
                symb_table[label_name] = line_num
            else:
                # add to instn-list
                if subbed_line:
                    # line is not empty
                    instn_list.append(subbed_line)
                    line_num = line_num + 1
    return instn_list


if __name__ == "__main__":
    main()
