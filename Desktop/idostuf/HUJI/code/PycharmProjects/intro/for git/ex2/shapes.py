import math
def circle_area(radius):  #receives radius, returns area of circle
    return (math.pi * radius ** 2)

def equi_area (side):  #receives side of equilateral triangle, returns area
    return (math.sqrt(3) / 4 * side ** 2)

def shape_area():  #function prints request for number, expects input and calls relevant function accordingly
    choose_shape = input('Choose shape (1=circle, 2=rectangle, 3=triangle): ')
    if choose_shape == '1':
        return circle_area(int(input()))
    elif choose_shape == '2':
        side1 = int(input())
        side2 = int(input())
        return (side1 * side2)  #area of a rectangle
    elif choose_shape == '3':
        return equi_area(int(input()))
    else:   #None as return value for any irrelevant input
        return None

