#!/usr/bin/env python
import numpy as np
import re
import time
import matplotlib.pyplot as plt
    
file = open("benchmark.txt", "r")
lines = file.readlines();

trail_len = int(re.search(r'\d+', lines[0]).group())
particle_size = int(re.search(r'\d+', lines[1]).group())
particle_type = int(re.search(r'\d+', lines[2]).group())

if particle_type == 0:
    particle_type = "CUBE-%s-" % particle_size
elif particle_type == 1:
    particle_type = "POINT-"
else:
    particle_type = "LINE-%s-" % trail_len

# FPS versus particles per frame
points = []

for line in lines[4:]:
    numbers = line.split("\t\t")
    points.append([round(float(numbers[0]), 1), round(float(numbers[1]), 1)])

points = sorted(points, key=lambda x:x[1])
points = np.array(points)

plot = plt.plot(points[:,1], points[:,0])
plt.setp(plot, color='r', linewidth=2.0)
plt.ylabel('frames per second')
plt.xlabel('particles per frame')
currTime = time.strftime("%H:%M:%S")
fileName = particle_type + currTime
plt.savefig("img/%s.png" % fileName)
plt.show()