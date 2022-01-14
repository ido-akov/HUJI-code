class Board:
    """
    A 2-D object of size 7 x 7 including a target cell (3,7). Includes a list containing all cars on board.
    """

    def __init__(self):
        # Note that this function is required in your Board implementation.
        # However, is not part of the API for general board types.
        self.height = 7
        self.width = 7
        self.cars = []

    def __str__(self):
        """
        This function is called when a board object is to be printed.
        :return: A string of the current status of the board
        """
        #The game may assume this function returns a reasonable representation
        #of the board for printing, but may not assume details about it.
        matrix = [self.cell_content(coord) + ' ' if self.cell_content(coord)
                  else '_ ' for coord in self.cell_list()[:-1]]
        matrix = [''.join(matrix[x:x + 7]) for x in range(0, len(matrix), 7)]
        as_string = '\n'.join(''.join(row) for row in matrix)
        return as_string

    def cell_list(self):
        """ This function returns the coordinates of cells in this board
        :return: list of coordinates
        """
        #In this board, returns a list containing the cells in the square
        #from (0,0) to (6,6) and the target cell (3,7)
        return [(i,j) for i in range(self.height) for j in range(self.width)] + [(3,7)]

    def possible_moves(self):
        """ This function returns the legal moves of all cars in this board
        :return: list of tuples of the form (name,movekey,description)
                 representing legal moves
        """
        #From the provided example car_config.json file, the return value could be
        #[('O','d',"some description"),('R','r',"some description"),('O','u',"some description")]
        all_legal_moves = []
        for car in self.cars:
            movekeys = [k for k in car.possible_moves()]
            for m in movekeys:
                required_cell = car.movement_requirements(m)
                for r in required_cell:
                    if not self.cell_content(r) and r in self.cell_list():
                        all_legal_moves.append((car.get_name(), m, "legal move"))
        return all_legal_moves

    def target_location(self):
        """
        This function returns the coordinates of the location which is to be filled for victory.
        :return: (row,col) of goal location
        """
        return (3,7)

    def cell_content(self, coordinate):
        """
        Checks if the given coordinates are empty.
        :param coordinate: tuple of (row,col) of the coordinate to check
        :return: The name if the car in coordinate, None if empty
        """
        for c in self.cars:
            if coordinate in c.car_coordinates():
                #problem- if accidentally two cars in same coord returns first car, although both are
                return c.get_name()

        return

    def all_other_car_coordinates(self, car):
        """

        :param car: car object of
        :return:
        """
        other_cars = [others for others in self.cars if others != car]
        return[l for car in other_cars for l in car.car_coordinates()]

    def add_car(self, car):
        """
        Adds a car to the game.
        :param car: car object of car to add
        :return: True upon success. False if failed
        """
        #Remember to consider all the reasons adding a car can fail.
        #You may assume the car is a legal car object following the API
        if car.get_name() in self.all_colors():
            return False
        for c in car.car_coordinates():
            if not self.check_cords(c):
                return False
        self.cars.append(car)
        return True

    def check_cords(self, coord):
        """ helper function: checks if car has coordinate which is currently taken or isn't in board.
        Returns true if no problem- else returns false."""
        if self.cell_content(coord) or coord not in self.cell_list():
                return False
        return True



    def all_colors(self):
        return [car.get_name() for car in self.cars]

    def move_car(self, name, movekey):
        """
        moves car one step in given direction.
        :param name: name of the car to move
        :param movekey: Key of move in car to activate
        :return: True upon success, False otherwise
        """
        for car in self.cars:
            if car.get_name() == name and movekey in car.possible_moves():
                for c in car.movement_requirements(movekey):
                    if not self.check_cords(c):
                        return False
                return car.move(movekey)
        return False
