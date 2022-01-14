from copy import deepcopy
from sys import argv

ERROR_MESSAGE_ARGUMENTS = 'Please write in all parameters: word_file, matrix_file, output_file, directions'
ERROR_MESSAGE_FILES = " doesn't exist"
ERROR_MESSAGE_CHARS = "Please write directions as chars from 'udlrwxyz'"
def check_input_args(args):
    """
    :param args: receives list of inputs from command-line- in format [PATHNAME\file.py, word_file,
    matrix_file, output_file, directions]
    :return: Error messages if problems with parameters, else None
    """
    if len(args) != 5:
        return ERROR_MESSAGE_ARGUMENTS

    try:
        open(args[1], 'r')
    except FileNotFoundError:
        return + args[1] + ERROR_MESSAGE_FILES
    try:
        open(args[2], 'r')
    except FileNotFoundError:
        return args[2] + ERROR_MESSAGE_FILES
    for a in args[4]:
        if a not in 'udlrwxyz':
            return ERROR_MESSAGE_CHARS
    return

def read_wordlist_file(filename):
    """
    receives file containing valid words in word-format
    returns list of words stripped of \n at end of each line
    """
    word_list = open(filename, 'r')  #create pipe to relevant file
    return [w.rstrip() for w in word_list.readlines()]  #get rid of newspaces

def read_matrix_file(filename):
    """
    receives file containing letters in matrix-format,
    returns list of lists, each containing a row of the matrix
    """
    matrix = open(filename, 'r')
    mat = [list(m.rstrip()) for m in matrix.readlines()]  #get rid of newspaces at end of each line
    for m in mat:  #iteration: get rid of 'commas'
        m[:] = list(filter(lambda a: a != ',', m))
    return mat  #return 2-d


def get_all_substrings(input_string):
    """
    helper function to helper function:
    receives string, returns all substrings
    """
    length = len(input_string)
    return [input_string[i:j + 1] for i in range(length) for j in range(i, length)]  # list comprehension

def compare_words(dictionary, matrix):
    """
    helper function:
    receives dictionary with items (word, count), receives 2-d rectangular matrix as list of lists
    returns dictionary with updated values according to search within matrix
    """

    for m in matrix:
        for s in get_all_substrings(''.join(m)):  #convert mth list in matrix to string of letters
            if s in dictionary.keys():
                dictionary[s] += 1
    return dictionary  #updated values in dictionary


def down_search(matrix):
    """
    :param 2-d rectangular matrix as list of lists,
    :returns matrix transposed
    """
    new_list = []
    try:
        for i in range(len(matrix[0])):  # same length for every row
            y = list()
            for m in matrix:
                y.append(m[i])
            new_list.append(y)
        return new_list
    except IndexError:  # if empty matrix received as parameter, skip computation and return []
        return []

def reverse(matrix):
    """
    :param: 2-d rectangular matrix as list of lists
    :returns: matrix with reversed rows
    """
    new_list = deepcopy(matrix)
    for i in new_list:
        i.reverse()
    return new_list

def get_rows(matrix):
    """
    receives 2-d matrix, returns copy of matrix
    """
    return [[c for c in r] for r in matrix]

def diagonal_search(matrix):
    """
    :param 2-d rectangular matrix as list of lists
    :returns: matrix with diagonals from top left downwards
    """
    if len(matrix) == 1:  # if one-row matrix- return original row
        return matrix
    else:
        b = [None] * (len(matrix) - 1)  # create new row with empty lists
        matrix = [b[:i] + r + b[i:] for i, r in enumerate(get_rows(matrix))]  # create new larger matrix with
        # empty cells on sides of each line in order to search down each column and get diagonals
        return [[c for c in r if c is not None] for r in down_search(matrix)]  # search down columns for non-empty lists

def search_loop(dictionary, matrix, directions):
    """
    receives dictionary with items (word, count), receives 2-d rectangular matrix as list of lists,
    receives string of chars specifying search direction
    :return: dictionary with updated values
    """

    for d in directions:  #iterate over string with chars specifying search directions
        if d == 'r':
            dictionary.update(compare_words(dictionary, matrix))
        if d == 'l':
            dictionary.update(compare_words(dictionary, reverse(matrix)))
        if d == 'd':
            dictionary.update(compare_words(dictionary, down_search(matrix)))
        if d == 'u':
            dictionary.update(compare_words(dictionary, reverse(down_search(matrix))))
        if d == 'w':
            dictionary.update(compare_words(dictionary, reverse(diagonal_search(matrix))))
        if d == 'x':
            dictionary.update(compare_words(dictionary, reverse(diagonal_search(reverse(matrix)))))  #
        if d == 'y':
            dictionary.update(compare_words(dictionary, diagonal_search(reverse(matrix))))
        if d == 'z':
            dictionary.update(compare_words(dictionary,diagonal_search(matrix)))
    return dictionary

def find_words_in_matrix(word_list, matrix, directions):  #receives list and dict?
    """
    function receives list of words, 2-d rectangular matrix as list of lists, string of characters specifying
    searching directions in matrix
    """
    word_dict = {}.fromkeys(word_list,0)  # create dictionary from word_list, with initial value of 0 for each key
    directions = list({}.fromkeys(directions).keys())  # clean out unecessary repetitions in directions
    word_dict.update(search_loop(word_dict, matrix, directions))
    words_found = [word for word in word_dict.items() if word[1] > 0]
    # pairs_sorted = sorted(words_found, key=lambda x: word_list.index(x[0]))
    # pairs_sorted = sorted(words_found, key=lambda x: x[0].lower())
    return words_found

def write_output_file(results, output_file):
    """
    :param: list containing results
    :returns file containing each member of list on new line """
    with open(output_file, 'w+') as file:
        for r in results:
            y = r[0], str(r[1])
            file.write(','.join(y) + '\n')  # write in specified output format


def main():
    """
    main function: puts all others together.
    """
    arguments = argv  #list of parameters received from command-line
    if not check_input_args(arguments):  #if no errors:
        found_words = find_words_in_matrix(read_wordlist_file(arguments[1]),  #use command line params in function
                        read_matrix_file(arguments[2]), arguments[4])
        write_output_file(found_words, arguments[3])  #write to filename as specified in command-line
    else:
        print(check_input_args(arguments))  #if error messages received- print

if __name__ == "__main__":
    main()

