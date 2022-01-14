def input_list():
    """ having run function receive strings from user, keep appending onto list while string received isn't empty.
    return list of inputted strings """
    user_list = []
    string_from_user = input()
    while string_from_user != '':  #keep appending strings while inputted string isn't empty
        user_list.append(string_from_user)
        string_from_user = input()
    return user_list


def concat_list(str_list):
    """ receive list of strings, return one long string with all individual strings spaced out"""
    concat_str = ''
    for s in str_list:  # iteration over list
        if s != str_list[-1]:  #while item isn't last in list- concatenate to one long string with added space
            concat_str += (str(s) + ' ')
        else:  #last string is concatenated onto long string without added space
            concat_str += str(s)
    return concat_str


def maximum(num_list):
    """ receives list of any type, returns maximum of types convertable to int, or else None"""
    if not num_list:  #if list is empty, return None
        return
    max = int()
    for n in num_list:  #iteration over list
        try:  #try to convert nth item to an int
            n = int(n)
        except ValueError:  #if n isn't convertible: continue to next item
            continue
        if n >= max or bool(n) > bool(max):  #if current item is larger than maximum
            max = n
    if not max:  #at conclusion of loop check if max still has value 0
        return int()
    return max  #otherwise return max


def cyclic(lst1, lst2):
    """ function receives two lists, checks whether one is a permutation of the other.
     If so- function returns True, otherwise returns False."""
    def lst_perm(lst, shifter = 0):
        """ helper function- receives list and an integer n,
        creates cyclic permutation of list by n steps """
        new_lst = lst[::]  # shallow copy of list to mutate into permutation
        for l in lst:  #iteration over list
            new_lst[(lst.index(l) + shifter) % len(lst)] = l
        return new_lst  # return mutated copy of list

    if not lst1 and not lst2:
        return True
    for n in range(len(lst1)): #checks all possible permutations and compares to lst2
        if lst_perm(lst1, n) == lst2:
            return True
    return False


def seven_boom(n):
    """ receives integer, returns game of seven_boom up until integer"""
    game_list = []
    for i in range(1,n+1):  #iteration from 1 up till received integer
        if i % 7 == 0 or str(i)[-1] == '7':  #if number is a product of 7 or ends with 7
            game_list.append('boom')  #write boom
        else:  #otherwise write number
            game_list.append(str(i))
    return game_list


def histogram(n, num_list):
    """ receive range of integers 0-n, list of integers, return list of how many times each number in range appears
    within list, in order"""
    range_list = []
    hist_list = []
    for i in range(n):
        range_list.append(i)  #range_list includes numbers to be checked within num_list
    for r in range_list:  #iteration over all ints in range
        counter = int()  #appearance default: zero
        for n in num_list:
            if r == n:
                counter += 1  #for every time current number appears in list, increase counter by 1
        hist_list.append(counter)  #append number of appearances to histogram
    return hist_list


def prime_factors(n):
    """ receive integer, return all prime factors"""
    primes_list = []
    i = 2  #first prime number
    while i <= n:
        if not n % i:  #if n divides by i- append i to primes_list. Because
            primes_list.append(i)
            n //= i  #repeat process with largest factor of n- divide into prime factors
        else:
            i += 1  # if n doesn't divide by i- check next largest i.
    return primes_list


def cartesian(lst1, lst2):
    """ receive two lists, return all ordered pairs of list items"""
    if not lst1 or not lst2:
        return []
    pairs_list = []
    for l1 in lst1:
        for l2 in lst2:  #nested loop:
            pairs_list.append([l1, l2])
    return pairs_list


def pairs(num_list, n):
    """ receives list of integers and an integer n,
    returns pairs of integers from list who's sum matches n. Permutated pairs of integers don't count as different
     pairs. """
    pairs_list = []  #empty list for matching pairs
    if not num_list:
        return pairs_list
    for n1 in num_list:  #iteration over num_list, in order to find a possible matching pair for each integer
        new_list = num_list[::]  #new copy of num_list
        new_list.remove(n1)
        for n2 in new_list:  #iteration over list excluding current integer
            if n2 + n1 == n:  #if match is found:
                good_pair = [n1, n2]  #create list with pair
                if not pairs_list:  #if list of all good pairs is empty- append
                    pairs_list.append(good_pair)
                else:  #if list of all good pairs isn't empty:
                    bool_check = True  #flag for iteration over list of good pairs
                    for p in pairs_list:  #iterate over list of good pairs
                        if cyclic(good_pair, p) == True:  #if list of good pairs contains permutation of current
                        #good pair, then don't append
                            bool_check = False
                    if bool_check == True:  #otherwise append
                        pairs_list.append(good_pair)
    return pairs_list

