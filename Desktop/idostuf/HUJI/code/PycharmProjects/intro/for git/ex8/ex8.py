def backtracker(cur_set, k, index, picked, lst = [], b = False):
    """ function receives data from helper function , as well as boolean regarding which helper to further call.
    Function backtracks according to base case"""
    if index == len(cur_set):  #base case
        return

    cur_set[index] = True
    if b:  #if first variation
        k_subset_helper_1(cur_set, k, index + 1, picked + 1)
        cur_set[index] = False
        k_subset_helper_1(cur_set, k, index + 1, picked)
    else:  #if second variation
        k_subset_helper_2(cur_set, k, index + 1, picked + 1, lst)
        cur_set[index] = False
        k_subset_helper_2(cur_set, k, index + 1, picked, lst)



def k_subset_helper_1(cur_set, k, index, picked):
    """ first helper function: receives boolean array, int k, current index, number of indexes picked, prints
    correct sub-sets of size k, calls backtracker further"""
    # Base: we picked out k items.
    if k == picked:
        subs = [t for t in range(len(cur_set)) if cur_set[t]]
        print(subs)  # print list of indexes in cur_set containing True
        return
    backtracker(cur_set, k, index, picked, [], True)


def print_k_subsets(n,k):
    """function receives ints n, k, prints all subsets of size k within range(n)"""
    if k <= n:

        cur_set = [False] * n
        k_subset_helper_1(cur_set, k, 0, 0)


def k_subset_helper_2(cur_set, k, index, picked, lst = []):
    """ second helper function: receives boolean array, int k, current index, number of indexes picked, appends
        correct sub-sets of size k to lst, calls backtracker further"""
    # Base: we picked out k items.
    if k == picked:
        subs = [t for t in range(len(cur_set)) if cur_set[t]]
        lst.append(subs)  # print list of indexes in cur_set containing True
        return
    backtracker(cur_set, k, index, picked, lst)


def fill_k_subsets(n, k, lst = []):
    """function receives ints n, k, returns list including all subsets of size k within range(n)"""
    if k <= n:

        cur_set = [False] * n
        k_subset_helper_2(cur_set, k, 0, 0, lst)


def k_subset_helper_3(n, k):
    '''
    Third helper: generator that yields all subsets of size k according to binomial formula
    '''
    if k == 0 or n < k:
        yield []
    elif n == k:
        yield list(range(n))
    else:
        # Use recursive formula based on binomial coeffecients:
        # choose(n, k) = choose(n - 1, k - 1) + choose(n - 1, k)
        for s in k_subset_helper_3(n - 1, k - 1):
            s.append(n - 1)
            yield s
        for s in k_subset_helper_3(n - 1, k):
            yield s


def return_k_subsets(n, k):
    """function receives ints n, k, returns list including all subsets of size k within range(n)"""
    return list(k_subset_helper_3(n,k))

def NextEmptyCell(board, row = 0, col = 0):
    """
    generator receives square 2-D array containing digits from 0-9, current row/col,
    yields next cell containing 0 """
    empty_cells_forward = [(x,y) for x in range(row,len(board[0])) for y in range(col,len(board[0])) if board[x][y] == 0]
    if empty_cells_forward:
        yield from empty_cells_forward
    else:
        empty_cells_backward = [(x, y) for x in range(0, len(board[0])) for y in range(0, len(board[0])) if board[x][y] == 0]
        if empty_cells_backward:
            yield from empty_cells_backward
        else:
            yield -1, -1

def no_conflicts(board, row, col, value, n):
    """ receives receives square 2-D array, returns True if possible to place current value in current row/col, else
    returns False"""
    row_check = [value != board[row][x] for x in range(n)]
    root_n = int(n ** 0.5)
    if not False in row_check:
        column_check = [value != board[x][col] for x in range(n)]
        if not False in column_check:
            square_row_top_left, square_col_top_left = root_n * (row // root_n), root_n * (col // root_n)
            #rewrite this somehow using mod? square bounds-
            square_check = [value != board[x][y] for x in range(square_row_top_left, square_row_top_left + root_n)
                           for y in range(square_col_top_left, square_col_top_left + root_n)]
            if not False in square_check:
                return True
    return False
# rewrite this

def solve_sudoku(board, row=0, col=0):
    """ function receives partly solved sudoku puzzle: an NxN array partly including digits 1-9, and otherwise 0's
    returns True if solvable and fills board with solution, else returns False."""
    n = len(board[0])
    values = range(1,n + 1)
    def extend_solution(position):
        """ Helper backtracker function: receives current position as tuple of form (row,col),
        calls itself using next free position, backtracks if next value can't be placed. """
        row, col = position
        if row == -1:  #if no 0's in the array
            return True
        for value in values:
            if no_conflicts(board, row, col, value, n):
                board[row][col] = value
                if extend_solution(next(NextEmptyCell(board,row,col))):
                    return True
                board[row][col] = 0
        return False

    return extend_solution(next(NextEmptyCell(board)))
