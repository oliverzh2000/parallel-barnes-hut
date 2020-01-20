import time

import pygame
import os.path

width = 500

os.environ['SDL_VIDEO_WINDOW_POS'] = "0,40"
pygame.init()
pygame.display.init()
screen = pygame.display.set_mode((width, width))


clock = pygame.time.Clock()

sim_folder = os.path.abspath(r"C:\Users\Oliver\CLionProjects\3d-barnes-hut\sim_data\sim5")
out_files = list(filter(lambda file: file.startswith("out-"), os.listdir(sim_folder)))
out_files.sort(key=lambda file: int(file.lstrip("out-").rstrip(".txt")))

frames = []
for out_file in out_files:
    frames.append([list(map(float, star_state.split(" ")))
                   for star_state in list(open(os.path.join(sim_folder, out_file)))[1:]])

max_x = max([max(star[0] for star in frame) for frame in frames])
max_y = max([max(star[1] for star in frame) for frame in frames])
max_z = max([max(star[2] for star in frame) for frame in frames])

min_x = min([min(star[0] for star in frame) for frame in frames])
min_y = min([min(star[1] for star in frame) for frame in frames])
min_z = min([min(star[2] for star in frame) for frame in frames])

def map_to_range(x, old_min, old_max, new_min, new_max):
    scale_factor = (new_max - new_min) / (old_max - old_min)
    return (x - old_min) * scale_factor + new_min

back_color = (255, 255, 255)
star_color = (0, 0, 0)
x_axis_color = (255, 100, 0)
y_axis_color = (100, 255, 0)

radius = 2
done = False
fps = 30
margin = 10
draw_star_as_rect = True
auto_size = True
scale = 300
center_x, center_y = 0, 0

screen.fill((255, 255, 255))
for index, frame in enumerate(frames):
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True


    if done:
        exit()

    print("progress: {}/{}".format(index, len(frames)))

    screen.fill((255, 255, 255))
    pygame.draw.line(screen, x_axis_color, (0, width / 2), (width, width / 2))
    pygame.draw.line(screen, y_axis_color, (width / 2, 0), (width / 2, width))

    for star in frame:
        if auto_size:
            old_min, old_max = min(min_x, min_y), max(max_x, max_y)
            center = (map_to_range(star[0], old_min, old_max, margin, width - margin),
                      map_to_range(star[1], old_min, old_max, margin, width - margin))
        else:
            center = (width / 2 + (star[0] - center_x) * scale,
                      width / 2 + (star[1] - center_y) * scale)
        if draw_star_as_rect:
            pygame.draw.rect(screen, (0, 0, 0), pygame.Rect(list(map(int, center)), (radius, radius)))
        else:
            pygame.draw.rect(screen, (0, 0, 0), list(map(int, center)), radius)


    pygame.display.flip()
    clock.tick(fps)

while not done:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True
    clock.tick(30)