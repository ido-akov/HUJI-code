def calculate_mathematical_expression(num1, num2, char): #completes calculation with two numbers (int or float) and char
    if (char == '/' and num2 == 0) or all([char != '+', char != '-', char != '*', char != '/']):  #'None' cases: if
    #char isn't legal or if attempting to divide by zero
        return None
    if char == '+':
        return num1 + num2
    if char == '-':
        return num1 - num2
    if char == '*':
        return num1 * num2
    if char == '/':
        return num1 / num2

def calculate_from_string(expr):  #receives string and returns calculation based on the string
    first_space = expr.index(' ')  #finds index of space between first num and operator
    num1 = float(expr[:first_space])  #converts sub-string up to space into float
    expr = expr[first_space + 1: ]  #slices and rebinds string to later sub-string of operator and second num
    char = expr[0]  #operator
    num2 = float(expr[2: ])  #converts sub-string of second num to float
    return calculate_mathematical_expression(num1, num2, char)  #calls previous function
