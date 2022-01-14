class Asteroid:
    RADIUS_MULT = 10
    RADIUS_ADD = - 5

    def __init__(self, x_y_coords, x_y_speeds, size=3):
        """this func initialises coordinates speed and size for the asteroid"""
        self.x_coord, self.y_coord = x_y_coords
        self.x_speed, self.y_speed = x_y_speeds
        self.size = size

    def move(self, new_coordinates):
        """this func gets new coordinates and apply's the change in the object"""
        self.x_coord = int(new_coordinates[0])
        self.y_coord = int(new_coordinates[1])

    def set_speed(self, new_speed_x, new_speed_y):
        """
        this func sets new speed to the asteroid
        :param new_speed_x: speed in the x axis
        :param new_speed_y: speed in the y axis
        """
        self.x_speed = new_speed_x
        self.y_speed = new_speed_y

    def get_x_coord(self):
        """this func returns the x axis coordinate"""
        return self.x_coord

    def get_y_coord(self):
        """this func returns the x axis coordinate"""
        return self.y_coord

    def has_intersection(self, obj):
        """this func gets to object of the game and checks if they collide"""
        dist = ((obj.x_coord - self.x_coord) ** 2 +
                (obj.y_coord - self.y_coord) ** 2) ** 0.5
        return dist <= (self.size * self.RADIUS_MULT - self.RADIUS_ADD) + obj.RADIUS
