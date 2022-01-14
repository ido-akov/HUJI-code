class Car:
    """
    1-D object to be placed on a board. A car can be moved in either direction relating to its orientation
    """
    def __init__(self, name, length, location, orientation):
        """
        A constructor for a Car object
        :param name: A string representing the car's name
        :param length: A positive int representing the car's length.
        :param location: A tuple representing the car's head (row, col) location
        (location with minimal distance from coordinate (0,0)
        :param orientation: One of either 0 (VERTICAL) or 1 (HORIZONTAL)
        """
        # Note that this function is required in your Car implementation.
        # However, is not part of the API for general car types.
        self.name = name
        if length > 0:
            self.length = length
        self.location = location
        self.orientation = orientation


    def car_coordinates(self):
        """
        :return: A list of coordinates the car is in
        """
        if self.orientation == 0:
            return [(self.location[0] + i, self.location[1]) for i in range(self.length)]
        elif self.orientation == 1:
            return [(self.location[0], self.location[1] + i) for i in range(self.length)]
        return

    def possible_moves(self):
        """
        :return: A dictionary of strings describing possible movements permitted by this car.
        """
        #For this car type, keys are from 'udrl'
        #The keys for vertical cars are 'u' and 'd'.
        #The keys for horizontal cars are 'l' and 'r'.
        #You may choose appropriate strings.
        #The dictionary returned should look something like this:
        #result = {'f': "cause the car to fly and reach the Moon",
        #          'd': "cause the car to dig and reach the core of Earth",
        #          'a': "another unknown action"}
        #A car returning this dictionary supports the commands 'f','d','a'
        if self.orientation == 0:
            move_dict = {'u': 'move car one up in current column',
                         'd': 'move car one down in current column' }
        else:
            move_dict = {'r': 'move car one cell right in current row',
                         'l': 'move car one cell left in current row'}
        return move_dict


    def movement_requirements(self, movekey):
        """ 
        :param movekey: A string representing the key of the required move.
        :return: A list of cell locations which must be empty in order for this move to be legal.
        """
        #For example, a car in locations [(1,2),(2,2)] requires [(3,2)] to
        #be empty in order to move down (with a key 'd').
        #deal with trip of any length in one direction?
        if movekey in self.possible_moves():
            if movekey == 'd':
                required_cell = self.car_coordinates()[-1]
                return [(required_cell[0] + 1, required_cell[1])]
            elif movekey == 'u':
                required_cell = self.car_coordinates()[0]
                return [(required_cell[0] - 1, required_cell[1])]
            elif movekey == 'r':
                required_cell = self.car_coordinates()[-1]
                return [(required_cell[0], required_cell[1] + 1)]
            elif movekey == 'l':
                required_cell = self.car_coordinates()[0]
                return [(required_cell[0], required_cell[1] - 1)]
        return

    def move(self, movekey):
        """ 
        :param movekey: A string representing the key of the required move.
        :return: True upon success, False otherwise
        """
        #use movement_requirements?
        if movekey in self.possible_moves():
            row = self.location[0]
            col = self.location[1]
            if movekey == 'd':
                self.location = row + 1, col
            elif movekey == 'u':
                self.location = row - 1, col
            elif movekey == 'l':
                self.location = row, col - 1
            else:
                self.location = row, col + 1
            return True
        return False


    def get_name(self):
        """
        :return: The name of this car.
        """
        return self.name
