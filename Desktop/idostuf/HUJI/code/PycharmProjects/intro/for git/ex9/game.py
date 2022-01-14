from sys import argv
from car import Car
from board import Board
from helper import load_json

INPUT_MSG = "Please enter car name and direction in which to move like this: name,direction. \n" \
            "For hint press 'h'. \n"
ERROR_MESSAGE = 'Car name or direction is not valid.'
NO_MOVE_MESSAGE= "Move couldn't be accomplished. Please try again"
WIN_MSG= 'Congratulations! You Won.'
JSON_MSG= "please write in path of a json file"

class Game:
    """
    Class object comprises a game of rush-hour on a 7 x 7 board, in which moving a car to the spot (3,7) comprises
    winning the game
    """

    def __init__(self, board):
        """
        Initialize a new Game object.
        :param board: An object of type board
        """
        #You may assume board follows the API
        # implement your code here (and then delete the next line - 'pass')
        self.board = board


    def valid_input(self):
        while True:
            usr_inp = input(INPUT_MSG)
            if usr_inp == 'h':
                print(self.board.possible_moves())
                continue
            else:
                if len(usr_inp) != 3 or usr_inp[1] != ',':
                    print(ERROR_MESSAGE)
                    continue
                usr_inp = usr_inp.split(',')
                if usr_inp[0] not in self.board.all_colors() or usr_inp[1] not in 'udlr':
                    print(ERROR_MESSAGE)
                    continue
                else:
                    return usr_inp



    def __single_turn(self):
        """
        Note - this function is here to guide you and it is *not mandatory*
        to implement it. 

        The function runs one round of the game :
            1. Get user's input of: what color car to move, and what 
                direction to move it.
            2. Check if the input is valid.
            3. Try moving car according to user's input.

        Before and after every stage of a turn, you may print additional 
        information for the user, e.g., printing the board. In particular,
        you may support additional features, (e.g., hints) as long as they
        don't interfere with the API.
        """
        user_input = self.valid_input()
        if self.board.move_car(*user_input) == True:
            return
        else:
            print(NO_MOVE_MESSAGE)
            return


    def play(self):
        """
        The main driver of the Game. Manages the game until completion.
        :return: None
        """
        # implement your code here (and then delete the next line - 'pass')
        while True:
            print(self.board.__str__())
            self.__single_turn()
            if self.board.cell_content(self.board.target_location()):
                print(WIN_MSG)
                break

def get_json_file():
    json_file = input(JSON_MSG)
    return json_file

def main():
    new_board = Board()
    arguments = argv
    if len(arguments) > 1:
        json_file = argv[1]
    else:
        json_file = get_json_file()
    car_dict = load_json(json_file)
    for key, value in car_dict.items():
        new_car = Car(key, value[0], tuple(value[1]), value[2])
        if len(new_board.cars) < 6 and new_car.length in range(2,5):
            new_board.add_car(new_car)
    new_game = Game(new_board)
    new_game.play()


if __name__== "__main__":
    #Your code here
    #All access to files, non API constructors, and such must be in this
    #section, or in functions called from this section.
    main()



