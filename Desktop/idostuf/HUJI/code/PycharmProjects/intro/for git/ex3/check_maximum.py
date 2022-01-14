from ex3 import maximum

def function_max_check():  #test check for function 3
    flag = (True, 'OK')  #initial boolean value
    try1, try2, try3, try4 = [1.00, 1, 1.001, 1.01], [.99, .98, 1.0, 1], [2 ** 0.5, 239 / 169], [1, 1, 1, 1] #4 test cases
    try_loop = [try1, try2, try3, try4]  #list of cases
    sol_loop = [1.01, 1.0, 2 ** 0.5, 1] #list of relevant solutions
    for t, s in zip(try_loop, sol_loop):  #iteration over both lists, with a compare test between relevant entries
        if maximum(t) != s:  #a comparing test between relevant entries
            flag = (False, 'Fail')
        print('Test', try_loop.index(t), flag[1])
    return flag[0]

if __name__ == '__main__':
    function_max_check()
