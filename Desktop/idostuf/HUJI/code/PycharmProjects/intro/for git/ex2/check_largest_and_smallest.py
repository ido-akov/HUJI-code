from largest_and_smallest import largest_and_smallest

def function_4_check():  #test check for function from question 4
    flag = True  #initial boolean value
    try1, try2, try3, try4, try5 = (1, 1, 2), (1, 2, 1), (2, 2, 1), (1, 1, 1), (2, 1, 2)  #5 test cases
    try_loop = [try1, try2, try3, try4, try5]  #list of cases
    sol_loop = [(2, 1)] * 3 + [(1, 1), (2, 1)]  #list of relevant solutions
    for t, s in zip(try_loop, sol_loop):  #iteration over both lists, with a compare test between relevant entries
        x, y, z = t  #splitting each tuple to three variables to plug into the function
        if largest_and_smallest(x, y, z) != s:  #a comparing test between relevant entries
            flag = False
    return flag

if __name__ =='__main__':
    function_4_check()
