def largest_and_smallest(num1, num2, num3):  #receives three integers- returns tuple comprising largest on the left
    # and smallest on the right
    max_val, min_val = num1, num2  #initial max/min values chosen randomly
    num_list = [num1, num2, num3]  #list comprising three arguments
    for l in num_list:  #iteration over list
        if l <= min_val:
            min_val = l  #if current number in list is smaller than or equal, min_val receives current number
        else:
            max_val = l
    return max_val, min_val

