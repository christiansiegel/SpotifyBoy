import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import random

CMD_3POINTS = 247

def scramble_list(orig):
    dest = orig[:]
    random.shuffle(dest)
    return dest

def reduce_colors(rgb):
    res = np.dot(rgb[...,:3], [0.299, 0.587, 0.114])
    res = np.divide(res, 256/3)
    res = np.round(res)
    return 3 - res

def print_commands(image, start_x, start_y):
    cmd = ""
    width = image.shape[1]
    height = image.shape[0]
    for y in scramble_list(list(range(0, height))):
        cmd += str(CMD_3POINTS) + ","
        cmd += str(start_x) + ","
        cmd += str(start_y + y) + ","
        pixels_value = 0
        pixels_count = 0
        for x in range(0, width):
            color = int(image[y][x]);
            pixels_value |= (color << (pixels_count * 2))
            pixels_count += 1
            if pixels_count == 3:
                cmd += str(pixels_value) + ","
                pixels_value = 0
                pixels_count = 0
        if pixels_count > 0:
            cmd += str(pixels_value) + ","
        cmd += "\n"
    print(cmd)

img = mpimg.imread('test.jpeg')     
img = reduce_colors(img)
print_commands(img, 30, 30)

