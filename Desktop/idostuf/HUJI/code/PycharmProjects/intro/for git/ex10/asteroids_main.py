from screen import Screen
import sys
from random import randint
from ship import Ship
from asteroid import Asteroid
from torpedo import Torpedo

DEFAULT_ASTEROIDS_NUM = 5


class GameRunner:
    """this class is responsible for running the game Asteroids, integrating the Ship,Asteroid and Torpedo Classes"""

    ASTEROID_SPEED_BOTTOM = 1
    ASTEROID_SPEED_TOP = 4
    TORPEDO_LIM = 10
    SPECIAL_TORPEDO_LIM = 5
    SHIP_LIVES = 3
    SPECIAL_SHOT = 1000
    DEFAULT_SCORE = 0

    # Messages constants
    SHIP_INTERSECTION_TITLE = 'Oh no!'
    SHIP_INTERSECTION_MSG = "Intersected with asteroid"
    WIN_TITLE = 'Won!'
    WIN_MSG = "Nothing left to do bro"
    LOSE_TITLE = 'Lost!'
    LOSE_MSG = "see you in the afterlife"
    QUIT_TITLE = 'Bye!'
    QUIT_MSG = "hope you enjoyed dodging asteroids in the milky way"

    def __init__(self, asteroids_amount):
        """here we create the Screen and Ship objects initialize the next parameters:
        self.__screen_max_x, self.__screen_max_y, self.__screen_min_x, self.__screen_min_y for screen size
        we create lists to hold the torpedos and asteroids that will be created
        self.input_asteroids(asteroids_amount) is the amount of asteroids in the game
        self.__score: our score begins in a pre defined value
        self.__game_loop_counter - counts the
        self.__lives - a predefined amount
        """
        self.__screen = Screen()
        self.__screen_max_x = self.__screen.SCREEN_MAX_X
        self.__screen_max_y = self.__screen.SCREEN_MAX_Y
        self.__screen_min_x = self.__screen.SCREEN_MIN_X
        self.__screen_min_y = self.__screen.SCREEN_MIN_Y
        self.__ship = Ship(self.make_rand_coords())
        self.__asteroids = []
        self.__torpedos = []
        self.input_asteroids(asteroids_amount)
        self.__score = self.DEFAULT_SCORE
        self.__game_loop_counter = 0
        self.__lives = self.SHIP_LIVES

    def make_rand_coords(self):
        """this function creates random coordinates that are within the boundaries of the screen"""
        return (randint(self.__screen_min_x, self.__screen_max_x),
                randint(self.__screen_min_y, self.__screen_max_y, ))

    def make_rand_speed(self):
        """this function creates a random speed for asteroids on the x and y axis """
        all_vals = list(range(-self.ASTEROID_SPEED_TOP, -self.ASTEROID_SPEED_BOTTOM + 1)) +\
                   list(range(self.ASTEROID_SPEED_BOTTOM, self.ASTEROID_SPEED_TOP + 1))
        return all_vals[randint(0, 7)], all_vals[randint(0, 7)]

    def input_asteroids(self, asteroids_amount):
        """
        this func loads the asteroids to the screen
        :param asteroids_amount: a predefined amount of asteroids
        """
        while asteroids_amount > 0:
            asteroid = Asteroid(self.make_rand_coords(), self.make_rand_speed())
            if self.no_loc_problems:
                self.__screen.register_asteroid(asteroid, asteroid.size)
                self.__asteroids.append(asteroid)
                asteroids_amount -= 1

    def no_loc_problems(self, asteroid):
        """this func checks if the created asteroid has the same coordinates as the ship
        if so returns False and the asteroid isn't added to the screen if True we add it"""
        if asteroid.x_coord != self.__ship.x_coord and asteroid.y_coord != self.__ship.y_coord:
            return True
        return False

    def get_new_coords(self, obj):
        """this func gets an object and returns its new coordinates with respect to the board"""
        new_coordinates = (obj.x_speed + obj.x_coord - self.__screen.SCREEN_MIN_X) %\
                          (self.__screen.SCREEN_MAX_X - self.__screen.SCREEN_MIN_X) + self.__screen.SCREEN_MIN_X, \
               (obj.y_speed + obj.y_coord - self.__screen.SCREEN_MIN_Y) %\
                          (self.__screen.SCREEN_MAX_X - self.__screen.SCREEN_MIN_X) + self.__screen.SCREEN_MIN_Y
        return new_coordinates

    def draw_ship(self):
        """this func gets the ship coords and heading and adds it to the screen"""
        self.__screen.draw_ship(self.__ship.x_coord, self.__ship.y_coord, self.__ship.heading)

    @staticmethod
    def set_new_coords(obj, coords):
        """
        this func updates an objects coordinates
        :param obj: an object of the game (ship\asteroid\torped)
        :param coords: a tuple of 2 vals x and y coords
        it addresses the move function that every object has
        """
        obj.move(coords)

    def change_direction_right(self, val):
        """this func changes the degree of the ship if val == True"""
        if val:
            self.__ship.set_heading(-7)

    def change_direction_left(self, val):
        """this func changes the degree of the ship if val == True"""
        if val:
            self.__ship.set_heading(7)

    def change_speed(self, val):
        """this function sets the speed of the ship"""
        if val:
            self.__ship.set_speed()

    def run(self):
        """this function runs the game iteratively"""
        # if not self.__screen._lives:
        #     return
        self._do_loop()
        self.__screen.start_screen()

    def _do_loop(self):
        """this function is in charge of one loop of the game"""
        # You don't need to change this method!
        self._game_loop()

        # Set the timer to go off again
        self.__screen.update()
        self.__screen.ontimer(self._do_loop, 5)

    def _game_loop(self):
        """this function is the game loop it counts the game rounds so far and monitors the winning \\ losing status
        and also updates the ship asteroid and torpedo objects"""
        self.__game_loop_counter += 1
        self.game_conditions()
        self.asteroid_movement()
        self.ship_check()
        self.torpedo_check()

    def ship_check(self):
        """this function is responsible for the proper work of the object ship"""
        self.draw_ship()
        self.change_direction_right(self.__screen.is_right_pressed())
        self.change_direction_left(self.__screen.is_left_pressed())
        self.teleport_ship(self.__screen.is_teleport_pressed())
        self.change_speed(self.__screen.is_up_pressed())
        self.update_coords(self.__ship)

    def teleport_ship(self, val):
        """
        this function is responsible for the safe teleport of the ship to a asteroid free space on the screen
        :param val: boolean from self.__screen.is_teleport_pressed()
        """
        if val:
            (x, y) = self.make_rand_coords()
            self.__ship.x_coord, self.__ship.y_coord = (x, y)
            while self.check_intersection():
                (x, y) = self.make_rand_coords()
                self.__ship.x_coord, self.__ship.y_coord = (x, y)

    def check_intersection(self):
        """this func checks if there is intersection
        returns True if there is else, False"""
        for ast in self.__asteroids:
            if ast.has_intersection(self.__ship):
                return True
        return False

        # flag = True
        # while flag:
        #     (x, y) = self.make_rand_coords()
        #     (z, w) = self.__ship.x_coord, self.__ship.y_coord
        #     self.__ship.x_coord, self.__ship.y_coord = (x, y)
        #     dummy = False
        #     for ast in self.__asteroids:
        #         if self.has_intersection(self.__ship, ast):
        #             self.__ship.x_coord, self.__ship.y_coord = (z, w)
        #             dummy = True
        #             break
        #     if dummy:
        #         continue
        #     break

    def asteroid_movement(self):
        """this func loops through the asteroid list and draws the asteroid it also
        refers to the intersections"""
        for asteroid in self.__asteroids:
            self.__screen.draw_asteroid(asteroid, asteroid.x_coord, asteroid.y_coord)
            self.update_coords(asteroid)
            self.asteroid_intersections(asteroid)

    def asteroid_intersections(self, asteroid):
        """
        :param asteroid: an object of type asteroid
        :return:
        """
        if asteroid.has_intersection(self.__ship):
            self.__screen.show_message(self.SHIP_INTERSECTION_TITLE, self.SHIP_INTERSECTION_MSG)
            self.asteroid_remove(asteroid)
            self.__screen.remove_life()
            self.__lives -= 1
        for torp in self.__torpedos:
            if asteroid.has_intersection(torp):
                self.add_points(asteroid)
                self.__screen.set_score(self.__score)
                if torp.special:
                    self.torpedo_remove(torp)
                    self.asteroid_remove(asteroid)
                    self.__score += self.SPECIAL_SHOT
                    self.__screen.set_score(self.__score)
                else:
                    self.torpedo_remove(torp)
                    self.split_asteroid(asteroid, torp)

    def game_conditions(self):
        """this func checks the status of winning \\ losing or a quit request from the user"""
        if not self.__asteroids:
            self.__screen.show_message(self.WIN_TITLE, self.WIN_MSG)
            self.__screen.end_game()
            sys.exit()
        if not self.__lives:
            self.__screen.show_message(self.LOSE_TITLE, self.LOSE_MSG)
            self.__screen.end_game()
            sys.exit()
        if self.__screen.should_end():
            self.__screen.show_message(self.QUIT_TITLE, self.QUIT_MSG)
            self.__screen.end_game()
            sys.exit()

    def add_points(self, asteroid):
        """this func adds score to the user with respect to asteroid size"""
        if asteroid.size == 3:
            self.__score += 20
        elif asteroid.size == 2:
            self.__score += 50
        elif asteroid.size == 1:
            self.__score += 100

    def split_asteroid(self, asteroid, torp):
        """
        this function splits the asteroids after collision and removes the desist asteroid
        :param asteroid: the asteroid that was hit
        :param torp: the hitting torpedo is needed to calculate the speed of the new asteroids
        """
        if asteroid.size == 3:
            baby_one = Asteroid((asteroid.x_coord, asteroid.y_coord), self.new_asteroid_speed(asteroid, torp), 2)
            baby_two = Asteroid((asteroid.x_coord, asteroid.y_coord), self.new_asteroid_speed(asteroid, torp), 2)
            self.split_asteroid_helper(asteroid, baby_one, baby_two)
        elif asteroid.size == 2:
            baby_one = Asteroid((asteroid.x_coord, asteroid.y_coord), self.new_asteroid_speed(asteroid, torp), 1)
            baby_two = Asteroid((asteroid.x_coord, asteroid.y_coord), self.new_asteroid_speed(asteroid, torp), 1)
            self.split_asteroid_helper(asteroid, baby_one, baby_two)
        elif asteroid.size == 1:
            self.asteroid_remove(asteroid)

    def split_asteroid_helper(self, asteroid, baby_one, baby_two):
        """this func helps the split asteroid by setting the speed of the second asteroid
        and registering the smaller asteroids and removing the old asteroid"""
        baby_two.set_speed(baby_two.x_speed * -1, baby_two.y_speed * -1)
        self.new_asteroid(baby_one)
        self.new_asteroid(baby_two)
        self.asteroid_remove(asteroid)

    def asteroid_remove(self, asteroid):
        """this func removes an asteroid from the game and from the screen"""
        self.__asteroids.remove(asteroid)
        self.__screen.unregister_asteroid(asteroid)

    def torpedo_remove(self, torpedo):
        """this func removes a torpedo from the game and from the screen"""
        self.__torpedos.remove(torpedo)
        self.__screen.unregister_torpedo(torpedo)

    def new_asteroid(self, asteroid):
        """this func adds an asteroid from the game and from the screen"""
        self.__screen.register_asteroid(asteroid, asteroid.size)
        self.__asteroids.append(asteroid)

    @staticmethod
    def new_asteroid_speed(asteroid, torpedo):
        """this func calculates the speed that will be delivered to the new asteroid"""
        new_x_speed = ((torpedo.x_speed + asteroid.x_speed) /
                       (asteroid.x_speed ** 2 + asteroid.y_speed ** 2) ** 0.5)
        new_y_speed = ((torpedo.y_speed + asteroid.y_speed) /
                       (asteroid.x_speed ** 2 + asteroid.y_speed ** 2) ** 0.5)
        return new_x_speed, new_y_speed

    def update_coords(self, obj):
        """this func gets an object of the game calculates its new coordinates and sets them"""
        coords = self.get_new_coords(obj)
        self.set_new_coords(obj, coords)

    def torpedo_check(self):
        """this function is responsible for integrating torpedo objects to the game"""
        if self.__screen.is_space_pressed():
            reg_torps = [torp for torp in self.__torpedos if not torp.special]
            if len(reg_torps) < self.TORPEDO_LIM:
                self.make_torpedo()
        elif self.__screen.is_special_pressed():
            all_specials = [torp for torp in self.__torpedos if torp.special]
            if len(all_specials) < self.SPECIAL_TORPEDO_LIM:
                self.make_torpedo(True)
        if self.__torpedos:
            for torp in self.__torpedos:
                if self.torpedo_time(torp):
                    self.__screen.unregister_torpedo(torp)
                    self.__torpedos.remove(torp)
                    continue
                self.__screen.draw_torpedo(torp, torp.x_coord, torp.y_coord, torp.heading)
                self.update_coords(torp)

    def make_torpedo(self, val=False):
        """
        this function creates a torpedo object and adds it to the screen
        :param val: a boolean value deciding whether it is a special torpedo
        """
        torp = Torpedo((self.__ship.x_coord, self.__ship.y_coord), (self.__ship.x_speed, self.__ship.y_speed),
                       self.__ship.heading, self.__game_loop_counter, val)
        torp.set_speed()
        self.__screen.register_torpedo(torp)
        self.__torpedos.append(torp)

    def torpedo_time(self, torpedo):
        """
        this func checks the number of rounds a torpedo object is in the game
        :param torpedo: an object of class Torpedo
        :return: True if torpedo life is over, False otherwise
        """
        if not torpedo.special:
            if self.__game_loop_counter - torpedo.game_time == 200:
                return True
        elif torpedo.special:
            if self.__game_loop_counter - torpedo.game_time == 150:
                return True
        return False


def main(amount):
    """this is the main function that creates an object of the class GameRunner"""
    runner = GameRunner(amount)
    runner.run()


if __name__ == "__main__":
    if len(sys.argv) > 1:
        main(int(sys.argv[1]))
    else:
        main(DEFAULT_ASTEROIDS_NUM)


