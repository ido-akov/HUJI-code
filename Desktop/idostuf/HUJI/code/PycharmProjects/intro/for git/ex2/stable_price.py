def is_it_stable(thresh_val, year1, year2, year3):  #receives initial value and three different prices, returns
# True if difference between each of two consecutive prices is less than value, False if either difference is larger
# than the initial value
    flag = True  #boolean value
    if (abs(year1 - year2) > thresh_val) or (abs(year2 - year3) > thresh_val):  #if either difference is above threshold
        # assign False to boolean
        flag = False
    return flag