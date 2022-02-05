# This is inspired from Tech With Tim on Pygame car racer
# 
# Car roam freely
# Uses up, down, left, right keys
#

import pygame
import time
import math

pygame.init()
RED_CAR = pygame.image.load("imgs/red-car.png")
WIDTH = 1280
HEIGHT = 720
WIN = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Robo Game!")

MAIN_FONT = pygame.font.SysFont("comicsans", 44)

FPS = 60

class GameInfo:
    LEVELS = 10

    def __init__(self, level=1):
        self.level = level
        self.started = False
        self.level_start_time = 0

    def next_level(self):
        self.level += 1
        self.started = False

    def reset(self):
        self.level = 1
        self.started = False
        self.level_start_time = 0

    def game_finished(self):
        return self.level > self.LEVELS

    def start_level(self):
        self.started = True
        self.level_start_time = time.time()

    def get_level_time(self):
        if not self.started:
            return 0
        return round(time.time() - self.level_start_time)


class AbstractCar:
    def __init__(self, max_vel, rotation_vel):
        self.img = self.IMG
        self.max_vel = max_vel
        self.vel = 0
        self.rotation_vel = rotation_vel
        self.angle = 0
        self.x, self.y = self.START_POS
        self.acceleration = 0.1

    def rotate(self, left=False, right=False):
        if left:
            self.angle += self.rotation_vel
        elif right:
            self.angle -= self.rotation_vel

    def draw(self, win):
        blit_rotate_center(win, self.img, (self.x, self.y), self.angle)

    def forward(self):
        self.vel = min(self.vel + self.acceleration, self.max_vel)
        self.move()

    def backward(self):
        #self.vel = max(self.vel - self.acceleration, -self.max_vel/2)
        self.vel = max(self.vel - self.acceleration, -self.max_vel)
        self.move()

    def move(self):
        radians = math.radians(self.angle)
        vertical = math.cos(radians) * self.vel
        horizontal = math.sin(radians) * self.vel

        self.y -= vertical
        self.x -= horizontal

    def collide(self, mask, x=0, y=0):
        car_mask = pygame.mask.from_surface(self.img)
        offset = (int(self.x - x), int(self.y - y))
        poi = mask.overlap(car_mask, offset)
        return poi

    def xborder(self):
        if (self.x > WIDTH - 50 or self.y > HEIGHT - 50):
            self.bounce()
        if (self.x < 0 or self.y < 0):
            self.bounce()

    def reset(self):
        self.angle = 0
        self.vel = 0
        self.x, self.y = self.START_POS

    def bounce(self):
        self.vel = -self.vel
        self.move()

class PlayerCar(AbstractCar):
    IMG = RED_CAR
    START_POS = (180, 200)

    def reduce_speed(self):
        self.vel = max(self.vel - self.acceleration / 2, 0)
        self.move()


def scale_image(img, factor):
    size = round(img.get_width() * factor), round(img.get_height() * factor)
    return pygame.transform.scale(img, size)


def blit_rotate_center(win, image, top_left, angle):
    rotated_image = pygame.transform.rotate(image, angle)
    new_rect = rotated_image.get_rect(
        center=rotated_image.get_rect(topleft=top_left).center)
    win.blit(rotated_image, new_rect.topleft)


def blit_text_center(win, font, text):
    render = font.render(text, 1, (200, 200, 200))
    win.blit(render, (win.get_width()/2 - render.get_width() /
                      2, win.get_height()/2 - render.get_height()/2))

def draw(win, player_car, game_info):

    win.fill((255, 255, 255))

    time_text = MAIN_FONT.render(
        f"Time: {game_info.get_level_time()}s", 1, (0, 0, 0))
    win.blit(time_text, (10, HEIGHT - time_text.get_height() - 40))

    vel_text = MAIN_FONT.render(
        f"Vel: {round(player_car.vel, 1)}px/s", 1, (0, 0, 0))
    win.blit(vel_text, (10, HEIGHT - vel_text.get_height() - 10))

    player_car.draw(win)
    pygame.display.update()


def move_player(player_car):
    keys = pygame.key.get_pressed()
    moved = False

    if keys[pygame.K_a]:
        player_car.rotate(left=True)
    if keys[pygame.K_d]:
        player_car.rotate(right=True)
    if keys[pygame.K_w]:
        moved = True
        player_car.forward()
    if keys[pygame.K_s]:
        moved = True
        player_car.backward()

    if keys[pygame.K_LEFT]:
        player_car.rotate(left=True)
    if keys[pygame.K_RIGHT]:
        player_car.rotate(right=True)
    if keys[pygame.K_UP]:
        moved = True
        player_car.forward()
    if keys[pygame.K_DOWN]:
        moved = True
        player_car.backward()

    if not moved:
        player_car.reduce_speed()


def handle_collision(player_car, game_info):
    player_car.xborder()

run = True
clock = pygame.time.Clock()
player_car = PlayerCar(8, 2)
game_info = GameInfo()
game_info.start_level()
counter = 0

while run:
    clock.tick(FPS) 

    draw(WIN, player_car, game_info)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False
            break

    move_player(player_car)

    handle_collision(player_car, game_info)



pygame.quit()
