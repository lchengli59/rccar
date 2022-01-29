# The full tutorial is available here: http://rmgi.blog./pygame-2d-car-tutorial.html
# The code is available here: https://github.com/rasmaxim/pygame-car-tutorial
#
# Added code to prevent the car from going out of screen
#
import os
import pygame
from math import sin, radians, degrees, copysign, cos
from pygame.math import Vector2


class Car:
    def __init__(self, x, y, angle=0.0, length=4, max_steering=30, max_acceleration=5.0):
        self.position = Vector2(x, y)
        self.velocity = Vector2(0.0, 0.0)
        self.angle = angle
        self.length = length
        self.max_acceleration = max_acceleration
        self.max_steering = max_steering
        self.max_velocity = 20
        self.brake_deceleration = 10
        self.free_deceleration = 2

        self.acceleration = 0.0
        self.steering = 0.0
        
        self.tl = Vector2(x+2,y-1)
        self.tr = Vector2(x+2,y+1)
        self.bl = Vector2(x-2,y-1)
        self.br = Vector2(x-2,y+1)
        self.border = Vector2(0.0, 0.0)

    def forward(self, dt):
        if self.velocity.x < 0:
            self.acceleration = self.brake_deceleration
        else:
            self.acceleration += 1 * dt
        self.acceleration = max(-self.max_acceleration, min(self.acceleration, self.max_acceleration))

    def backward(self, dt):
        if self.velocity.x > 0:
            self.acceleration = -self.brake_deceleration
        else:
            self.acceleration -= 1 * dt
        self.acceleration = max(-self.max_acceleration, min(self.acceleration, self.max_acceleration))

    def brake(self, dt):
        if abs(self.velocity.x) > dt * self.brake_deceleration:
            self.acceleration = -copysign(self.brake_deceleration, self.velocity.x)
        else:
            self.acceleration = -self.velocity.x / dt
        self.acceleration = max(-self.max_acceleration, min(self.acceleration, self.max_acceleration))

    def handsoff(self, dt):
        if abs(self.velocity.x) > dt * self.free_deceleration:
            self.acceleration = -copysign(self.free_deceleration, self.velocity.x)
        else:
            if dt != 0:
                self.acceleration = -self.velocity.x / dt
        self.acceleration = max(-self.max_acceleration, min(self.acceleration, self.max_acceleration))

    def steerwheel(self, type, dt):
        if (type=="RIGHT"):
            self.steering -= 30 * dt
        elif (type=="LEFT"):
            self.steering += 30 * dt
        else:
            self.steering = 0
        self.steering = max(-self.max_steering, min(self.steering, self.max_steering))

    def xborder(self):
        if (self.position[0] > self.border.x or self.position[0] < 0 or
            self.position[1] > self.border.y or self.position[1] < 0):
            return 1
        else:
            return 0
            
    def update(self, dt):
        self.velocity += (self.acceleration * dt, 0)
        self.velocity.x = max(-self.max_velocity, min(self.velocity.x, self.max_velocity))

        if self.steering:
            turning_radius = self.length / sin(radians(self.steering))
            angular_velocity = self.velocity.x / turning_radius
        else:
            angular_velocity = 0

        self.angle += degrees(angular_velocity) * dt
        self.position += self.velocity.rotate(-self.angle) * dt
        

class Game:
    def __init__(self):
        pygame.init()
        pygame.display.set_caption("My Remote Control Car")
        width = 1280
        height = 720
        self.screen = pygame.display.set_mode((width, height))
        self.clock = pygame.time.Clock()
        self.ticks = 60
        self.exit = False

    def run(self):
        current_dir = os.path.dirname(os.path.abspath(__file__))
        image_path = os.path.join(current_dir, "car.png")
        car_image = pygame.image.load(image_path).convert()
        car_image.set_colorkey((255,255,255))
        car = Car(10, 10)
        ppu = 32

        car.border[0] = 1280 / 32
        car.border[1] = 720 / 32

        while not self.exit:
            dt = self.clock.get_time() / 1000

            # Event queue
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.exit = True

            # User input
            pressed = pygame.key.get_pressed()

            if pressed[pygame.K_UP]:
                car.forward(dt)
            elif pressed[pygame.K_DOWN]:
                car.backward(dt)
            elif pressed[pygame.K_SPACE]:
                car.brake(dt)
            else:
                car.handsoff(dt)

            if pressed[pygame.K_RIGHT]:
                car.steerwheel("RIGHT", dt)
            elif pressed[pygame.K_LEFT]:
                car.steerwheel("LEFT", dt)
            else:
                car.steerwheel("NONE", dt)

            # Logic
            car.update(dt)
            
            # Drawing
            self.screen.fill((0, 0, 0))
            rotated = pygame.transform.rotate(car_image, car.angle)
            rect = rotated.get_rect()
            self.screen.blit(rotated, car.position * ppu - (rect.width / 2, rect.height / 2))
            pygame.display.flip()
            
            for i in range(10):
                if (car.xborder()==1):
                    car.backward(dt)
                    car.update(dt)
                else:
                    break

            self.clock.tick(self.ticks)
        pygame.quit()


if __name__ == '__main__':
    game = Game()
    game.run()
