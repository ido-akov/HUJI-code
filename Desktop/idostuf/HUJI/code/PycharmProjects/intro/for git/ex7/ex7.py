def print_to_n(n):
    """ function receives int and prints all numbers from 1 to int"""
    if n >= 1:
        print_to_n(n - 1)
        print(n)


def print_reversed(n):
    """ function receives int and prints all numbers from int to 1"""
    if n >= 1:
        print(n)
        print_reversed(n-1)


def is_prime(n):
    """ function receives int n > 1, determines whether n is prime and returns corresponding boolean"""
    def has_divisor_smaller_than (n, i):
        """ helper function: determines if n has divisors smaller than i"""
        if i == 2:
            return False
        if n % (i - 1) == 0:
            return True
        return has_divisor_smaller_than(n, i-1)
    if n == 2:
        return True
    if n % 2 == 0:
        return False
    return not has_divisor_smaller_than(n, int(n ** 0.5))


def exp_n_x(n, x):
    """ function receives int, calculates approximation of nth member of exponential series"""
    def factorial(l):
        if l == 1:
            return l
        return l * factorial(l - 1)

    if n == 0:
        return 1
    return x ** n / factorial(n) + exp_n_x(n-1, x)  #plug x into taylor polynomial of nth order


def play_hanoi(hanoi, n, src, dest, temp):
    """ function receives graphic program hanoi, int, three graphic objects ('needles'),
     solves hanoi tower of size n"""
    if n == 1:
        hanoi.move(src, dest)
    else:
        play_hanoi(hanoi, n - 1, src, temp, dest)  #move n-1 disks to temporary point
        hanoi.move(src, dest)  #move largest disk to destination
        play_hanoi(hanoi, n - 1, temp, dest, src)  #move n-1 disks from temporary point to destination


def make_all_sequences(char_list, n):
    """ helper function: returns all sequences of length n including any/all chars from char_list"""
    def recur_help(arr, char_list, n, index = 0):
        """ recursive function that takes initial empty array and writes in all possible sequences"""
        if n == 0:
            initial_arr.append(arr[:index])  #write in sequence of length n
        if n > 0:
            for i in char_list:
                arr[index] = i
                arr.append(recur_help(arr, char_list, n - 1,
                                              index + 1))

    initial_arr = [''] * n
    recur_help(initial_arr, char_list, n)
    return [a for a in initial_arr[n:] if a]  #return entries in array from nth point, not counting None values


def print_sequences(char_list, n):
    """ function receives char_list, int, prints all sequences of length n
     including any/all chars from char_list
     """
    for a in make_all_sequences(char_list, n):
        print(''.join(a))

def print_no_repetition_sequences(char_list, n):
    """ function receives char_list, int, prints permutations of chars in char_list
     (with no repetitions of chars)"""
    def perms(lst):
        """ helper function- creates array of all permutations of length n"""
        if not lst:
            yield []
        yield from (''.join([lst[i], *p]) for i in range(len(lst)) for p in perms(lst[:i] + lst[i + 1:]))

    for l in list(perms(char_list)):
        print(l[:n])


def parentheses(n):
    """ function receives int, returns all balanced sequences of n pairs of parentheses """
    def convert_binary_to_parens(binary_list):
        """ helper function: converts binary digits to parens"""
        return ''.join(['(' if i == 0 else ')' for i in binary_list])  #right parens for 0, left parens for 1

    def get_good_sequences(list_of_binaries):
        """ helper function: filters and keeps balanced binary sequences out of all binary sequences"""
        good_sequences = []
        for l in list_of_binaries:
            flag = True  #initial boolean
            if l[0] == 0 and l.count(0) == l.count(1):
                for d in range(1, len(l)):
                    if sum(l[:d + 1]) > (d + 1) / 2:
                        flag = False
                if flag:  #if all conditions for balanced sequence satisified:
                    good_sequences.append(l)
        return [convert_binary_to_parens(i) for i in good_sequences]

    return get_good_sequences(make_all_sequences([0, 1], 2 * int(n)))


def up_and_right(n, k):
    """ function receives point in cartesian plane and returns all paths
     comprising of up/right movement to point """
    def convert_binary_to_directions(binary_list):
        """ helper function: converts binary digits to 'r', 'u'"""
        return ''.join(['r' if i == 0 else 'u' for i in binary_list])

    def get_good_sequences(list_of_binaries):
        """ helper function: filters and keeps binary sequences with n 0's and k 1's"""
        good_sequences = [l for l in list_of_binaries if l.count(0) == n and l.count(1) == k]
        #if right number of 0's and 1's
        for i in good_sequences:
            print(convert_binary_to_directions(i))

    get_good_sequences(make_all_sequences([0, 1], n + k))


def flood_fill(image, start):
    """ function receives 2-d array and starting point, returns updated 2-d array comprising all possible
    shifts from '.' to '*' according to neighbor law."""
    def flood(image, row, col):
        image[row][col] = '*'  #switch cell from empty to flooded

    image[start[0]][start[1]] = '*'  #starter cell
    for i in range(1, len(image) - 1):  #within matrix without touching edges
        for j in range(1, len(image[0]) - 1):
            if (i, j) == start:  #don't look at starting cell
                continue
            if image[i][j] == '.':  #if cell is empty:
                if start[0] == i:
                    if start[1] == j - 1 or start[1] == j + 1:  #check if neighbor from above or below
                        flood(image, i, j)
                        flood_fill(image, (i, j))
                elif start[1] == j:
                    if start[0] == i - 1 or start[0] == i + 1:  #check if neighbor from left or right
                        flood(image, i, j)
                        flood_fill(image, (i, j))




