from wordsearch import down_search

def check_down_search():  #test check for function 'c'
    flag = (True, 'success')  #initial boolean value
    [first, second, third] = [[], [['a'],['b'],['c'],['d']], [['a','b','c','d'], ['a','b','c','d']]]
    #'c' test cases
    try_loop = [first, second, third]  #list of cases
    sol_loop = [[], [['a','b','c','d']], [['a','a'], ['b','b'], ['c','c'], ['d','d']]] #list of relevant solutions
    for t, s in zip(try_loop, sol_loop):  #iteration over both lists, with a compare test between relevant entries
        if down_search(t) != s:
            flag = (False, 'fail')
    print('Function "check_down_search" test', flag[1])
    return flag[0]

if __name__ == '__main__':
    check_down_search()

