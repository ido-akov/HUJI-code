from math import *


class Ship:

    CIRCLE = 2 * pi
    RADIUS = 1

    def __init__(self, x_y_coords, x_y_speeds=(0, 0), heading=0):

        self.x_coord, self.y_coord = x_y_coords
        self.x_speed, self.y_speed = x_y_speeds
        self.heading = heading

    def get_x_coord(self):
        """this func returns the x axis coordinate"""
        return self.x_coord

    def get_y_coord(self):
        """this func returns the y axis coordinate"""
        return self.y_coord

    def get_x_speed(self):
        """this func returns the speed in the x axis"""
        return self.x_speed

    def get_y_speed(self):
        """this func returns the speed in the y axis"""
        return self.y_speed

    def get_heading(self):
        """this func returns the heading of the ship"""
        return self.heading

    def move(self, new_coordinates):
        """this func gets new coordinates and apply's the change in the object"""
        self.x_coord = int(new_coordinates[0])
        self.y_coord = int(new_coordinates[1])

    def set_heading(self, degree):
        """this function updates the heading angle"""
        self.heading += degree

    def convert_deg_to_rad(self):
        """this func converts degrees to radians"""
        return self.get_heading() * (pi / 180.0)

    def set_speed(self):
        """this func, when called sets new speed to the ship according to the speed and heading parameters"""
        self.x_speed = self.get_x_speed() + cos(self.convert_deg_to_rad())
        self.y_speed = self.get_y_speed() + sin(self.convert_deg_to_rad())
