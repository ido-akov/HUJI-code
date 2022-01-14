def quadratic_equation(a, b, c):  #function receives coefficients of a quadratic equation and returns solution set
    discriminant= b ** 2 - (4 * a * c)
    if discriminant < 0:  #if discriminant is negative return no solutions
        return None, None
    else:
        discriminant **= 0.5  #in order to avoid error compute the square root only if the discriminant isn't negative
        sol1 = (-b + discriminant) / 2 * a
        sol2 = (-b - discriminant) / 2 * a
        if sol1 == sol2:  #if both solutions are the same- the equation has one solution. return the first
            return sol1, None
    return sol1, sol2  #if both solutions are different, return both

def quadratic_equation_user_input():
    coeff1, coeff2, coeff3 = [float(coeffs) for coeffs in input('Insert coefficients a, b, and c: ').split()]  #convert
    # and assign three values inputted by user to three variables
    sols = quadratic_equation(coeff1, coeff2, coeff3)  #plug variables into function, build tuple with solutions
    if sols[1] != None:  #if second solution exists: assign tuple values to two variables
        sol1, sol2 = sols
        print('The equation has two solutions:', sol1, 'and', sol2)
    elif sols[0] == None:  #if first solution doesn't exist the equation has no solutions
        print('The equation has no solutions')
    else:  #last case- print only solution
        print('The equation has one solution:', sols[0])

